#include "SimpleTSCH.h"
#include <vector>
#include <string.h>

Define_Module(SimpleTSCH);

void SimpleTSCH::startup() {
	setTimerDrift(0.0001);
	double time_offset = simTime().dbl();
	this->slotSize = par("slotSize").doubleValue()/1000.0;
	this->superFrameSize = par("superFrameSize");
	this->allocatedSlots = par("allocatedSlots").stdstringValue();
	this->slots = new bool[this->superFrameSize];
	this->numberOfAttempts = par("numberOfAttempts").longValue();
	memset(this->slots,0,this->superFrameSize*sizeof(bool));
	int k = 0;
	for(int i = 0; i < this->superFrameSize; i++) {
		this->slots[i] = (SELF_MAC_ADDRESS == ((int)(this->allocatedSlots[k]-'0')*10 + (int)(this->allocatedSlots[k+1]-'0')));
		k = k+2;
	}
	setTimer(SWITCH_CHANNEL, (1.0-time_offset)/0.0001); //switch the channel every 10mss
	this->slotCont = 0;
	this->ASN = 0;
	this->isCoordinator = par("isCoordinator");
	if(isCoordinator) {	
		setTimer(WC_UP, 600000);
	}
}


/* Handle packet received from upper (network) layer.
 * We need to create a MAC packet, (here it can just be the generic MacPacket)
 * and encapsulate the received network packet before forwarding it to RadioLayer
 */

void SimpleTSCH::fromNetworkLayer(cPacket * pkt, int destination)
{
	//Put the packet on the buffer. In this implementation the buffer has no limit
	packet *p = new packet;
	p->pkt = pkt;
	p->dest = destination;
	p->ack = true;
	buffer.push_back(p);
}


void SimpleTSCH::fromNetworkLayer(cPacket * pkt, int destination, bool ack)
{
	//Put the packet on the buffer. In this implementation the buffer has no limit
	packet *p = new packet;
	p->pkt = pkt;
	p->dest = destination;
	p->ack = ack;
	buffer.push_back(p);
}

/* Handle packet received from lower (radio) layer.
 * We accept packets from all MAC protocols (cast to the base class MacPacket)
 * Then we filter by the destination field. By default we set the generic
 * destination field to broadcast when we encapsulate a NET packet. If a
 * specific protocol does not change that field then bypassMAC will be
 * operating in a promiscuous mode.
 */
void SimpleTSCH::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	SimpleTSCHPacket *macPkt = dynamic_cast <SimpleTSCHPacket*>(pkt);
	if (macPkt == NULL)
		return;
	
	if(macPkt->getKindTSCH() == ACK_PACKET && macPkt->getDestination() == SELF_MAC_ADDRESS) {
		//verifica qual o ack que chegou...		
		//trace() << "Received ack for packet " << macPkt->getSequenceNumber();
		if(buffer_ret.size() > 0 && macPkt->getSequenceNumber() == buffer_ret.front()->packet->getSequenceNumber()) {
			//packet_ret *pr = buffer_ret.front();
			buffer_ret.pop_front();
		//	trace() << "Retirando pacote para retransmissão...";
			//delete pr;
		}
		else if(buffer_ret.size() > 0 && macPkt->getSequenceNumber() > buffer_ret.front()->packet->getSequenceNumber()) {
			//trace() << "chegou ack da frente...";
			while(buffer_ret.size() > 0 && macPkt->getSequenceNumber() > buffer_ret.front()->packet->getSequenceNumber()) {
				//trace() << "Retirando pacote para retransmissão... " << buffer_ret.front()->packet->getSequenceNumber();
				buffer_ret.pop_front();
			}
		}
	}
	else if (macPkt->getDestination() == SELF_MAC_ADDRESS) //||
	    //macPkt->getDestination() == BROADCAST_MAC_ADDRESS)
	{		
		
		//trace() << "Received packet: " << macPkt->getSequenceNumber() << " through " <<macPkt->getSource() << " Power: " << rssi << " " << macPkt->getSource();
		trace() << "Received packet at sink node: " << macPkt->getSequenceNumber() << " Through:  " << macPkt->getSource() << " Power: " << rssi << " from: " << macPkt->getSource(); 
		
		macFrameAck = new SimpleTSCHPacket("SimpleTSCH ACK packet", MAC_LAYER_PACKET);
		macFrameAck->setSource(SELF_MAC_ADDRESS);
		macFrameAck->setDestination(macPkt->getSource());
		macFrameAck->setSequenceNumber(macPkt->getSequenceNumber());
		macFrameAck->setKindTSCH(ACK_PACKET);
		//trace() << "Enviado para camada de rede o pacote...";
		//send an ACK_PACKET
		setTimer(SEND_ACK, 10); //wait 1ms to transmitt ACK
		toNetworkLayer(decapsulatePacket(macPkt));
	
	}
}

void SimpleTSCH::timerFiredCallback(int index)
{
	
	switch (index){
		case WC_UP: { //only in the mode with channel hopping for the beacons		
			RadioControlCommand *radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(WC_UPD);
	 		send(radioCmd, "toRadioModule");	
			setTimer(WC_UP, 600000);
			break;
	    }
		case SEND_ACK: {
			//trace() << "Sending ack for packet: " << macFrameAck->getSequenceNumber();
			toRadioLayer(macFrameAck);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			//delete macFrameAck;
			break;
		}
		case SWITCH_CHANNEL: { 
			//string addr(SELF_NETWORK_ADDRESS,1);
			//cont_ch[(int)(addr[0]-'0')]++;
			//double new_channel = 2405.0 + (cont_ch[(int)(addr[0]-'0')]%16)*5;
			//trace() << "Sim Time: " << simTime().dbl();
			//trace() << "Clock: " << getClock().dbl();						
			RadioControlCommand *radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			
			//int indch = ((int)(allocatedSlots[slotCont]-'0') + (ASN/superFrameSize))%16;
			//for star netowks only the Coordinator (node 0) receive packets. If a node with other ID receives the packet, it is necessary to sum the ID too.
			//(ASN/superFrameSize) = SLOTFRAMECOUNTER
			int indch = (ASN + (ASN/superFrameSize))%16;
			
			//double new_channel = 2405.0 + (ASN%16)*5;
			double new_channel = 2405.0 + indch*5;
							
			radioCmd->setParameter(new_channel);
	 		send(radioCmd, "toRadioModule");
			
			setTimer(SWITCH_CHANNEL, (slotSize/0.0001));
			
			//buffer of packets to be retransmitted
			if(buffer_ret.size() > 0 && slots[slotCont]) {
				packet_ret *pr = buffer_ret.front();
				buffer_ret.pop_front();
	
				if(pr->cont < this->numberOfAttempts) {
					packet_ret *pr2 = new packet_ret;
					pr2->cont = pr->cont+1;
			
					pr2->packet = pr->packet->dup();
					pr2->packet->setSource(pr->packet->getSource());
					pr2->packet->setDestination(pr->packet->getDestination());
					pr2->packet->setAckReq(pr->packet->getAckReq());
					pr2->packet->setKindTSCH(DATA_PACKET);
					buffer_ret.push_back(pr2);
					trace () << "Listening to ack";
					//trace() << "Colocando pacote para retransmissão depois da ret...";
										
				}	
				
				trace() << "Retransmitting packet " << pr->packet->getSequenceNumber() << " on: " << indch << "  slot: " << slotCont;
			//	pr->packet->setAckReq(false);
				toRadioLayer(pr->packet);
				toRadioLayer(createRadioCommand(SET_STATE, TX));
				
			}
			//buffer of new packets
			else if(buffer.size() > 0 && slots[slotCont]) {
				SimpleTSCHPacket *macFrame = new SimpleTSCHPacket("TDMAMAC packet", MAC_LAYER_PACKET);
				packet *p = buffer.front();
				cPacket *pkt = p->pkt;
				int destination = p->dest;
				buffer.pop_front();
				encapsulatePacket(macFrame, pkt);
				macFrame->setSource(SELF_MAC_ADDRESS);
				macFrame->setDestination(destination);
				macFrame->setAckReq(p->ack);
				macFrame->setKindTSCH(DATA_PACKET);
				
				if(p->ack && this->numberOfAttempts > 0) {
					packet_ret *pr = new packet_ret;
					pr->packet = macFrame->dup();
					pr->packet->setSource(SELF_MAC_ADDRESS);
					pr->packet->setDestination(destination);
					pr->packet->setAckReq(p->ack);
					pr->packet->setKindTSCH(DATA_PACKET);
					pr->cont = 1;
					buffer_ret.push_back(pr);
					trace () << "Listening to ack";
					//trace() << "Colocando pacote para retransmissão...";
				}
				trace() << "Transmitting packet " << macFrame->getSequenceNumber() << " on: " << indch << "  slot: " << slotCont;
				toRadioLayer(macFrame);
				toRadioLayer(createRadioCommand(SET_STATE, TX));
			}
			slotCont++;
			if(slotCont == superFrameSize){
				slotCont = 0;
			}
			ASN++;
			break;
		}
	}
}



