#include "SimpleTSCHTree.h"
#include <vector>
#include <string.h>

Define_Module(SimpleTSCHTree);

void SimpleTSCHTree::startup() {
	setTimerDrift(0.0001);
	double time_offset = simTime().dbl();
	this->isCoordinator = par("isCoordinator");
	this->isClusterHead = par("isClusterHead");
	this->cluster_id = par("clusterID");
	this->slotSize = par("slotSize").doubleValue();
	this->slotFrameSize = par("slotFrameSize");
	this->nodeIdSlot = par("nodeIdSlot").stdstringValue();
	this->slots = new bool[this->slotFrameSize];
	this->numberOfAttempts = par("numberOfAttempts").longValue();
	memset(this->slots,0,this->slotFrameSize*sizeof(bool));
	memset(this->slots_rx,0,this->slotFrameSize*sizeof(bool));
	
	int k = 0;
	
	for(int i = 0; i < this->slotFrameSize; i++) {
		//trace () << ((int)(this->nodeIdSlot[k]-'0')*10 + (int)(this->nodeIdSlot[k+1]-'0'));
		this->slots[i] = (SELF_MAC_ADDRESS == ((int)(this->nodeIdSlot[k]-'0')*10 + (int)(this->nodeIdSlot[k+1]-'0')));
    	idSlots[i] = (int)(nodeIdSlot[k]-'0')*10 + (int)(nodeIdSlot[k+1]-'0');
		k = k+2;
		if (idSlots[i] != SELF_MAC_ADDRESS) {
			if(isClusterHead || isCoordinator) {
				if(isClusterHead && (idSlots[i] != cluster_id)) {
					this->slots_rx[i] = true;
				}
				else if(!isClusterHead){
					this->slots_rx[i] = true;
				}
			}
			
		}
	}
	/*for(int i = 0; i < this->slotFrameSize; i++) {
		trace() << this->slots[i];
		trace() << slots_rx[i];
	}*/
	for(int i = 0; i < MAXNODES; i++) {
		this->id_ant[i] = 0;
	}
	
	setTimer(SWITCH_CHANNEL, (1.0-time_offset)/0.0001); //switch the channel every 10mss
	this->slotCont = 0;
	this->ASN = 0;
	if(isCoordinator) {	
		setTimer(WC_UP, 600000);
	}
}


/* Handle packet received from upper (network) layer.
 * We need to create a MAC packet, (here it can just be the generic MacPacket)
 * and encapsulate the received network packet before forwarding it to RadioLayer
 */

void SimpleTSCHTree::fromNetworkLayer(cPacket * pkt, int destination)
{
	//Put the packet on the buffer. In this implementation the buffer has no limit
	packet *p = new packet;
	p->pkt = pkt;
	p->dest = destination;
	p->ack = true;
	buffer.push_back(p);
}


void SimpleTSCHTree::fromNetworkLayer(cPacket * pkt, int destination, bool ack)
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
void SimpleTSCHTree::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	SimpleTSCHTreePacket *macPkt = dynamic_cast <SimpleTSCHTreePacket*>(pkt);
	if (macPkt == NULL)
		return;
	/*trace() << "Kind: " << macPkt->getKindTSCH();
	trace() << "Dest: " << macPkt->getDestination();
	trace() << "Src: " << macPkt->getSource();
	trace() << "Self: " << SELF_MAC_ADDRESS;
	trace() << "Slot_rx: " << slots_rx[slotCont];
	trace() << "Slot_cont: " << slotCont;*/
	if(macPkt->getKindTSCH() == ACK_PACKET && macPkt->getDestination() == SELF_MAC_ADDRESS) {
		//verifica qual o ack que chegou...		
		//trace() << "Received ack for packet " << macPkt->getSequenceNumber();
		if(buffer_ret.size() > 0 && macPkt->getSequenceNumber() == buffer_ret.front()->packet->getSequenceNumber()) {
			//packet_ret *pr = buffer_ret.front();
			packet_ret *paux = buffer_ret.front();
			delete paux->packet;
			buffer_ret.pop_front();
			//trace() << "Retirando pacote para retransmissão...";
			//delete pr;
		}
		/*else if(buffer_ret.size() > 0 && macPkt->getSequenceNumber() > buffer_ret.front()->packet->getSequenceNumber()) {
			//trace() << "chegou ack da frente...";
			while(buffer_ret.size() > 0 && macPkt->getSequenceNumber() > buffer_ret.front()->packet->getSequenceNumber()) {
				//trace() << "Retirando pacote para retransmissão... " << buffer_ret.front()->packet->getSequenceNumber();
				buffer_ret.pop_front();
			}
		}*/
	}
	else if (macPkt->getDestination() == cluster_id && isClusterHead && slots_rx[slotCont]==true) {
		if(macPkt->getKindTSCH() == DATA_PACKET) { //DATA PACKET	
			trace() << "Received packet at cluster head: " << macPkt->getSequenceNumber() << "  from: " << macPkt->getSource() << " Power: " << rssi;
			
			if(macPkt->getSequenceNumber() == id_ant[macPkt->getSource()]) {							
			//	trace() << "Duplicado";
			}
			else { //only put the non duplicate packets on the buffer
				buffer_ch.push_back(macPkt->dup());
			//	trace() << "Colocando pacote " << macPkt->getSequenceNumber() << " from " << macPkt->getSource() << " on the buffer";
				if(buffer_ch.size() > macBufferSize) {
				//	trace() << "Buffer cheio, retirando pacote " << buffer_ch.front()->getSequenceNumber() << " from " << buffer_ch.front()->getSource() << " of the buffer";
					buffer_ch.pop_front();
				}
			}
			id_ant[macPkt->getSource()] = macPkt->getSequenceNumber();
		}	
		macFrameAck = new SimpleTSCHTreePacket("SimpleTSCHTree ACK packet", MAC_LAYER_PACKET);
		macFrameAck->setSource(SELF_MAC_ADDRESS);
		macFrameAck->setDestination(macPkt->getSource());
		macFrameAck->setSequenceNumber(macPkt->getSequenceNumber());
		macFrameAck->setKindTSCH(ACK_PACKET);
		//trace() << "Enviado para camada de rede o pacote...";
		//send an ACK_PACKET
		setTimer(SEND_ACK, 10); //wait 1ms to transmitt ACK
		toNetworkLayer(decapsulatePacket(macPkt));
	}
	else if (macPkt->getDestination() == SELF_MAC_ADDRESS && slots_rx[slotCont]==true) {		
		
		//trace() << "Received packet: " << macPkt->getSequenceNumber() << "  from: " << macPkt->getSource() << " Power: " << rssi;
		trace() << "Received packet at sink node: " << macPkt->getSequenceNumber() << " Through:  " << idSlots[slotCont] << " Power: " << rssi << " from: " << macPkt->getSource(); 
		
		macFrameAck = new SimpleTSCHTreePacket("SimpleTSCHTree ACK packet", MAC_LAYER_PACKET);
		macFrameAck->setSource(SELF_MAC_ADDRESS);
		macFrameAck->setDestination(idSlots[slotCont]);
		macFrameAck->setSequenceNumber(macPkt->getSequenceNumber());
		macFrameAck->setKindTSCH(ACK_PACKET);
		//trace() << "Enviado para camada de rede o pacote...";
		//send an ACK_PACKET
		setTimer(SEND_ACK, 10); //wait 1ms to transmitt ACK
		toNetworkLayer(decapsulatePacket(macPkt));
	
	}
}

void SimpleTSCHTree::timerFiredCallback(int index)
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
			
			int id_rx = 0;
			if(slots[slotCont]) {
				id_rx = cluster_id;
			}
			else if(slots_rx[slotCont]) {
				id_rx = SELF_MAC_ADDRESS;
			}
			
			//int indch = ((int)(idSlots[slotCont]-'0') + (ASN/slotFrameSize))%16;
			int indch = (id_rx + ASN + (ASN/slotFrameSize))%16;
			//double new_channel = 2405.0 + (ASN%16)*5;
			double new_channel = 2405.0 + indch*5;
							
			radioCmd->setParameter(new_channel);
	 		send(radioCmd, "toRadioModule");
						
			//trace() << "Slot " << slotCont << "  Channel " << indch;
			
			if(!isClusterHead && !isCoordinator) {
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
					SimpleTSCHTreePacket *macFrame = new SimpleTSCHTreePacket("TDMAMAC packet", MAC_LAYER_PACKET);
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
			}
			else if(isClusterHead) {
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
					
						//trace() << "Colocando pacote para retransmissão depois da ret...";
										
					}	
				
					trace() << "Retransmitting packet " << pr->packet->getSequenceNumber() << " on: " << indch << "  slot: " << slotCont;
				//	pr->packet->setAckReq(false);
					toRadioLayer(pr->packet);
					toRadioLayer(createRadioCommand(SET_STATE, TX));
				
				}
				//buffer of new packets
				else if(buffer_ch.size() > 0 && slots[slotCont]) {
					SimpleTSCHTreePacket *macFrame = buffer_ch.front();//new ABMPTreePacket("DSME packet", MAC_LAYER_PACKET);
					//packet *p = buffer.front();
				//	cPacket *pkt = p->pkt;
				//	int destination = p->dest;
					buffer_ch.pop_front();
				//	encapsulatePacket(macFrame, pkt);
				//	macFrame->setSource(SELF_MAC_ADDRESS);
				//	macFrame->setDestination(destination);
				//	macFrame->setAckReq(p->ack);
				//	macFrame->setKindDSME(DATA_PACKET);
					if(macFrame->getAckReq() && this->numberOfAttempts > 0) {
						packet_ret *pr = new packet_ret;
						pr->packet = macFrame->dup();
						pr->packet->setSource(macFrame->getSource());
						pr->packet->setDestination(macFrame->getDestination());
						pr->packet->setAckReq(macFrame->getAckReq());
						pr->packet->setKindTSCH(DATA_PACKET);
						pr->cont = 1;
						buffer_ret.push_back(pr);
					}
					
					trace() << "Transmitting packet " << macFrame->getSequenceNumber() << " on: " << indch << "  slot: " << slotCont;
				
					toRadioLayer(macFrame);
					toRadioLayer(createRadioCommand(SET_STATE, TX));
				}
			}
			setTimer(END_FRAME, ((slotSize-1)/0.1));
			//setTimer(SWITCH_CHANNEL, (1/0.0001));
			break;
			
		}
		case END_FRAME: {	
			slotCont++;
			if(slotCont == slotFrameSize){
				slotCont = 0;
			}
			ASN++;
			setTimer(SWITCH_CHANNEL, (1/0.1));
			break;
		}
	}
}



