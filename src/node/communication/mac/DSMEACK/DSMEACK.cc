#include "DSMEACK.h"
#include <vector>
#include <string.h>

Define_Module(DSMEACK);

void inline DSMEACK::medianValue(uint8_t node_addr) {
    int temp;
    if(ind == 1) rssi_median[1][node_addr] = rssi_median[0][node_addr];
    else if(ind == 2) rssi_median[1][node_addr] = (rssi_median[0][node_addr]+rssi_median[1][node_addr])/2;
    else{
        for(int i2 = 0; i2 < 3; i2++) {
            for(int j2 = 0; j2 < 2; j2++) {
                if(rssi_median[j2][node_addr] > rssi_median[j2+1][node_addr]) {
                    temp = rssi_median[j2][node_addr];
                    rssi_median[j2][node_addr] = rssi_median[j2+1][node_addr];
                    rssi_median[j2+1][node_addr] = temp;
                }
            }
        }
    }
}

void inline DSMEACK::mediana(uint8_t node_addr) {
    for (int i1 = 0; i1 < ESTWINDOW; i1++) {
        ind = 0;
        for (int j1 = (i1 - 1); j1 <= (i1 + 1); j1++) {
            if (j1 >= 0 && j1 < ESTWINDOW) {
                rssi_median[ind++][node_addr] = rssi_values[j1][node_addr];
            }
        }
         medianValue(node_addr);
         rssi_values_avg[i1][node_addr] = rssi_median[1][node_addr];
         avg_s[node_addr] += rssi_values_avg[i1][node_addr];
    }
    avg_s[node_addr] = (double)avg_s[node_addr]/ESTWINDOW;
    avg_s[node_addr] = avg_s[node_addr]/255.0;
}

void DSMEACK::calculatePRRu(uint8_t node_addr) { 
    if(avg_s[node_addr] > 0.5) {
        prr_u[node_addr] = 1.0;
    }
    else{
        prr_u[node_addr] = 526.56*pow(avg_s[node_addr],6) - 1333.2*pow(avg_s[node_addr],5) + 1315.5*pow(avg_s[node_addr],4) - 639.22*pow(avg_s[node_addr],3)+159.31*avg_s[node_addr]*avg_s[node_addr] - 19.251*avg_s[node_addr] + 0.9287;
        prr_u[node_addr] = 1.0-prr_u[node_addr];
    }
}

void DSMEACK::calculatePRRd(uint8_t node_addr) {
    prr_d[node_addr] = prr_d[node_addr]*0.6 +  (0.0014*avg_dup[node_addr]*avg_dup[node_addr] - 0.0742*avg_dup[node_addr] + 0.9889)*0.4;
}


/*bool DSMEACK::calculatePRR(int addr, int sequenceNumber) {
	window[addr]++;
	if(exp_id[addr] <= sequenceNumber){
		if(slot_attempt == SLOT_1) {
			fail_cont[addr] += 2*(sequenceNumber-exp_id[addr]);
		}
		else {
			fail_cont[addr] += 1 + 2*(sequenceNumber-exp_id[addr]);
		} 
	}
	/*else if (exp_id[addr] == sequenceNumber && slot_attempt == SLOT_2) { 
		fail_cont[addr]++;
	}
	else if(exp_id[addr] > sequenceNumber) {
		trace() << "Duplication";
	}
	if(window[addr] == WINDOW) {
		PRR[addr] = ALPHA_PRR*PRR[addr] + (1-ALPHA_PRR)*((double)(WINDOW/(WINDOW+fail_cont[addr])));
		trace() << "PRR " << addr << " " << PRR[addr]; 
		window[addr] = 0;
		fail_cont[addr] = 0;
		return true;
	}
	return false;
}*/
bool DSMEACK::calculatePRR(int addr, packet_information packet_list[QTDNODES][WINDOW], long long first) {
	double fail_cont = 0;
	long long exp_id = packet_list[addr][first].id;
	for(int i = first; i < (first+WINDOW); i++) {
		if(exp_id <= packet_list[addr][i%WINDOW].id){
			if(packet_list[addr][i%WINDOW].slot == SLOT_1) {
				fail_cont += 2*(packet_list[addr][i%WINDOW].id-exp_id);
			}
			else {
				fail_cont += 1 + 2*(packet_list[addr][i%WINDOW].id-exp_id);
			} 
		}
		exp_id = packet_list[addr][i%WINDOW].id+1;
	}
	PRR[addr] = ALPHA_PRR*PRR[addr] + (1-ALPHA_PRR)*((double)(WINDOW/(WINDOW+fail_cont)));
	//trace() << "PRR " << addr << " " << PRR[addr]; 
	return true;
}

//adherence functions
double DSMEACK::mSPRR(double SPRR) {
	if(SPRR <= 0.25) return 0;
	else if(SPRR >= 0.95) return 1;
	else return (4.0*SPRR-1)/3.0;
}

double DSMEACK::mSF(double SF) {
	if(SF >= 0.7) return 0;
	else return (-10.0*SF+7)/7.0;
}

double DSMEACK::mASL(double ASL) {
	if (ASL >= 0.5) return 0;
	else if(ASL <= 0.01) return 1;
	else return (-100.0*ASL+50)/49.0;
}

double DSMEACK::mSNR(double SNR) {
	if(SNR <= 1) return 0;
	else if(SNR >=8) return 1;
	else return (SNR-1)/7.0;
}

double DSMEACK::mSRNP(double SRNP) {
	if(SRNP >= 4) return 0;
	else if(SRNP <= 1) return 1;
	else return (4-SRNP)/3.0;
}

//fuzzy functions
double DSMEACK::mean(double sprr, double sf, double asl, double snr) {
	double avg = 0;
	int cont = 0;
	if(sprr != -1) {
		avg += sprr;
		cont++;
	}
	if(sf != -1) {
		avg += sf;
		cont++;
	}
	if(asl != -1) {
		avg += asl;
		cont++;
	}
	if(snr != -1) {
		avg += snr;
		cont++;
	}
	return avg/cont;
}

double DSMEACK::min2(double a, double b) {
	if(a == -1 && b != -1) return b;
	else if(b == -1 && a!= -1) return a;
	else if(a == -1 && b == -1) return 1e9;
	else if(a <= b) return a;
	else return b;
}

double DSMEACK::min(double sprr, double sf, double asl, double snr) {
	return min2(sprr,min2(sf,min2(asl,snr)));
}

double DSMEACK::prr_variance(double *prr, int n) {
	if(n <= 1) return -1;
	double avg = 0;
	double var = 0;
	for(int i = 0; i < n; i++) avg+=prr[i];
	avg /= n;
	for(int i = 0; i < n; i++) {
		var += (prr[i] - avg)*(prr[i] - avg);
	}
	return sqrt(var/(n-1))/avg;
}
 

void DSMEACK::startup() {
	
	setTimerDrift(0.0001);
	double time_offset = simTime().dbl();

	this->allocatedSlots = par("allocatedSlots").stdstringValue();
	//this->slots = new bool[16];
	this->numberOfAttempts = par("numberOfAttempts").longValue();
	memset(this->slots,0,32*sizeof(bool));
	
	for(int i = 0; i < allocatedSlots.size(); i++) {
		this->slots[i] = this->allocatedSlots[i]-'0';
	}

	this->superFrameOrder = par("superFrameOrder");
	this->multiSuperFrameOrder = par("multiSuperFrameOrder");
	this->beaconOrder = par("beaconOrder");
	this->isCoordinator = par("isCoordinator");
	this->channelDiversity = par("channelDiversity");
	
	this->slotSize = 960*pow(2,this->superFrameOrder)*0.001; //in ms

	this->nodeIdSlot = par("nodeIdSlot").stdstringValue();
	int k = 0;
    for(int i = 0; i < nodeIdSlot.size()/2; i++) {
    	idSlots[i] = (int)(nodeIdSlot[k]-'0')*10 + (int)(nodeIdSlot[k+1]-'0');
		k = k+2;
    }	
	this->gackSlots = par("gackSlots").stdstringValue();
	memset(this->ackSlots,0,32*sizeof(bool));
	
	if(isCoordinator) {	
		setTimer(SEND_BEACON, (1.0-time_offset)/0.0001); //initialoffset for sending a beacon
		setTimer(WC_UP, 600000);
	}
	this->beacon_cont = 0;
	this->cfp_cont = 0;
	
	this->slot_attempt = SLOT_1;
	for(int i = 0; i < QTDNODES; i++) {
		this->fail_cont[i] = 0;
		this->window[i] = 0;
		this->channel[i] = 0;
		this->PRR[i] = 1;
		this->id_ant[i] = -1;
		this->cont_interval[i] = 0;
		this->prr_u[i] = this->prr_d[i] = 1;
		this->avg_dup[i] = 0;
	}
	this->prrThreshold = par("prrThreshold").doubleValue();
	this->channel_bitmap = 0;
	this->channelbm_recv = false;
	this->ch_beacon = 0;
	this->SDIndex = 0;
	this->cfp_cont_relative = 0;
	
	this->macBufferSize = par("macBufferSize").longValue();
	this->cont_interval_tx = 0;
	
} 	

/* Handle packet received from upper (network) layer.
 * We need to create a MAC packet, (here it can just be the generic MacPacket)
 * and encapsulate the received network packet before forwarding it to RadioLayer
 */

void DSMEACK::fromNetworkLayer(cPacket * pkt, int destination)
{
	//Put the packet on the buffer. In this implementation the buffer has no limit
	packet *p = new packet;
	p->pkt = pkt;
	p->dest = destination;
	p->ack = true;
	buffer.push_back(p);
	if(buffer.size() > macBufferSize) {
		buffer.pop_front();
	}
}


void DSMEACK::fromNetworkLayer(cPacket * pkt, int destination, bool ack)
{
	//Put the packet on the buffer. In this implementation the buffer has no limit
	packet *p = new packet;
	p->pkt = pkt;
	p->dest = destination;
	p->ack = ack;
	buffer.push_back(p);
	if(buffer.size() > macBufferSize) {
		buffer.pop_front();
	}
}

/* Handle packet received from lower (radio) layer.
 * We accept packets from all MAC protocols (cast to the base class MacPacket)
 * Then we filter by the destination field. By default we set the generic
 * destination field to broadcast when we encapsulate a NET packet. If a
 * specific protocol does not change that field then bypassMAC will be
 * operating in a promiscuous mode.
 */
void DSMEACK::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
	DSMEACKPacket *macPkt = dynamic_cast <DSMEACKPacket*>(pkt);
	if (macPkt == NULL)
		return;

	if(macPkt->getKindDSME() == BEACON_PACKET) {
		trace() << "Beacon received " << macPkt->getSequenceNumber();
		cfp_cont = 0;
		cfp_cont_relative = 0;
		SF_offset = macPkt->getSequenceNumber();
	    if(channelDiversity != 0) {
		//	trace () << "Channel bitmap: " << macPkt->getChannelBitmap();
			if((macPkt->getChannelBitmap() >> (SELF_MAC_ADDRESS-1)) & 1) {
				//trace() << "Entrou aqui " << channelbm_recv;
				if(!channelbm_recv) {
					channel[SELF_MAC_ADDRESS] = (channel[SELF_MAC_ADDRESS]+1)%16;
					trace() << "Change channel of " << SELF_MAC_ADDRESS << ": " << channel[SELF_MAC_ADDRESS];
					channelbm_recv = true;		
					cont_interval_tx = 0;
				}
				else if(cont_interval_tx < 10){
					cont_interval_tx++;
				}
				else {
					channel[SELF_MAC_ADDRESS] = (channel[SELF_MAC_ADDRESS]+1)%16;
					trace() << "Change channel of " << SELF_MAC_ADDRESS << ": " << channel[SELF_MAC_ADDRESS] << " due to a deep fading";
					channelbm_recv = true;		
					cont_interval_tx = 0;
				}
			}
			else {
				channelbm_recv = false;
			}
	    }
		setTimer(BEGIN_CFP, (((8*slotSize)-0.212)/0.1)); //in this implementation the capReduction is enabled, and the nodes go directly to the CFP.
		
		if(channelDiversity == 2)
			setTimer(BEACON_TIMEOUT, ((33*slotSize)/0.1));
				
	}
	else if (macPkt->getDestination() == SELF_MAC_ADDRESS ||
	    macPkt->getDestination() == BROADCAST_MAC_ADDRESS)
	{	
		if(macPkt->getKindDSME() == ACK_PACKET) {
			//verifica qual o ack que chegou...		
			//trace() << "Received ack for packet " << macPkt->getSequenceNumber();
			if(buffer_ret.size() > 0) {
				buffer_ret.pop_front();
			//	trace() << "Retirando pacote para retransmissão...";
			}
			else {
			//	trace() << "Retirando pacote para retransmissão 2...";
			}
		}
		else { //DATA PACKET
			
			this->cont_interval[macPkt->getSource()] = 0;
			
			ack_bitmap = ack_bitmap | (1 << (cfp_cont-1));
		
			//trace() << "Received packet: " << macPkt->getSequenceNumber() << " through "<< macPkt->getSource() << " Power: " << rssi << " "<< macPkt->getSource();
			trace() << "Received packet at sink node: " << macPkt->getSequenceNumber() << " Through:  " << macPkt->getSource() << " Power: " << rssi << " from: " << macPkt->getSource(); 
			
			bool new_value = false;
		/*	if(id_ant[macPkt->getSource()] != macPkt->getSequenceNumber()) {
			 	packet_list[macPkt->getSource()][window[macPkt->getSource()]%WINDOW].id = macPkt->getSequenceNumber();
				packet_list[macPkt->getSource()][window[macPkt->getSource()]%WINDOW].slot = slot_attempt;
				trace() << "Adicionando pacote: " << packet_list[macPkt->getSource()][window[macPkt->getSource()]%WINDOW].id << " em: " << window[macPkt->getSource()]%WINDOW;
				window[macPkt->getSource()]++;
				
				if(window[macPkt->getSource()] >= WINDOW) {
					long long first = (window[macPkt->getSource()])%WINDOW;
					new_value = calculatePRR(macPkt->getSource(), packet_list, first);
				}
			}
			else {
				trace() << "Duplicado";
			}*/
			//LQ calculation
			if(macPkt->getSequenceNumber() == id_ant[macPkt->getSource()]) {
			    avg_dup[node_addr] += 1; //duplicacao de pacote
			}
			id_ant[macPkt->getSource()] = macPkt->getSequenceNumber();
			
		    node_addr = macPkt->getSource();
			//trace() << "Node_addr: " << node_addr;
		    rssi_values[contann[node_addr]%ESTWINDOW][node_addr] = (double)((rssi+94.0)/0.25);
		    contann[node_addr]++;
		    if(contann[node_addr] == ESTWINDOW || startedEstimation) {
				avg_s[node_addr] = 0;
			    mediana(node_addr);
			    calculatePRRu(node_addr);
			    avg_dup[node_addr] = avg_dup[node_addr]/(ESTWINDOW/10);
			    calculatePRRd(node_addr);
			   // contann[node_addr] = 0;
			    avg_dup[node_addr] = 0;
			    trace() << "LQu: " << (prr_u[node_addr]);
				startedEstimation = true;
				new_value = true;
			}
		    if(contann[node_addr]%ESTWINDOW == 0) {
			    avg_dup[node_addr] = avg_dup[node_addr]/(ESTWINDOW/10);
			    calculatePRRd(node_addr);
			    avg_dup[node_addr] = 0;
			    trace() << "LQd: " << (prr_d[node_addr]);
				new_value = true;
			}			
			if(channelDiversity != 0) {//if using channel adaptation
				if(channel_bitmap != 0) {
					if((channel_bitmap >> (macPkt->getSource()-1)) & 1) {
						channel_bitmap = (channel_bitmap) ^ (1 << (macPkt->getSource()-1));
						//trace() << "Retirando channelbit map para " << macPkt->getSource() << " bitmap: " << channel_bitmap;
					}
				}
				
				double lq = prr_u[macPkt->getSource()]*prr_d[macPkt->getSource()];
				if(new_value && lq < prrThreshold) {
				//if(new_value && PRR[macPkt->getSource()] < prrThreshold) {
					channel_bitmap = (channel_bitmap) | (1 << (macPkt->getSource()-1));
					channel[macPkt->getSource()] = (channel[macPkt->getSource()]+1)%16;
					trace() << "Change channel of " << macPkt->getSource() << ": " << channel[macPkt->getSource()];
					prr_u[macPkt->getSource()] = prr_d[macPkt->getSource()] = 1; //reset the PRR value
					window[macPkt->getSource()] = 0;
					new_value = false;
					startedEstimation = false;
					contann[macPkt->getSource()] = 0;
		 		}
				else if(new_value) {
					new_value = false;
				}
		    }
		//	trace() << "Ack_bitmap: " << ack_bitmap;
			macFrameAckDSME = new DSMEACKPacket("DSME ACK Packet", MAC_LAYER_PACKET);
			macFrameAckDSME->setSource(SELF_MAC_ADDRESS);
			macFrameAckDSME->setDestination(macPkt->getSource());
			macFrameAckDSME->setSequenceNumber(macPkt->getSequenceNumber());
			macFrameAckDSME->setKindDSME(ACK_PACKET);
			setTimer(SEND_ACK, 10); //wait 1ms to transmitt ACK
			
			toNetworkLayer(decapsulatePacket(macPkt));
	    }
	
	}
}

void DSMEACK::timerFiredCallback(int index)
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
			//trace() << "Sending ack for packet: " << macFrameAckDSME->getSequenceNumber();
			toRadioLayer(macFrameAckDSME);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			//delete macFrameAck;
			break;
		}
		case BEACON_TIMEOUT: { //only in the mode with channel hopping for the beacons
			trace() << "Timeout expired, lost beacon";
			trace () << "Listening to beacon";
			RadioControlCommand *radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			ch_beacon = (ch_beacon+1)%16;
			channel[0] = ch_beacon; //TODO: customize beacon ID -> here is set to 0
			double new_channel = 2405.0 + 5*ch_beacon;
			radioCmd->setParameter(new_channel);
	 		send(radioCmd, "toRadioModule");			
			setTimer(BEACON_TIMEOUT, ((32*slotSize)/0.1));
			break;
		}
		case SEND_BEACON: {		
			if(channelDiversity == 2) { //channel hopping for the beacons
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
				double new_channel = 2405.0 + 5*ch_beacon;
				channel[0] = ch_beacon; //TODO: customize beacon ID -> here is set to 0
				ch_beacon = (ch_beacon+1)%16;
				radioCmd->setParameter(new_channel);			
		 		send(radioCmd, "toRadioModule");
			}
			else {	//always use channel 11 for beacons (modes 0 and 1)
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
				double new_channel = 2405.0; //CHANNEL 11
				channel[0] = 0; //TODO: customize beacon ID -> here is set to 0
				radioCmd->setParameter(new_channel);
		 		send(radioCmd, "toRadioModule");
			}
			
			DSMEACKPacket *macBeacon; 
			macBeacon = new DSMEACKPacket("DSMEACK beacon", MAC_LAYER_PACKET);
			macBeacon->setSource(SELF_MAC_ADDRESS);
			macBeacon->setDestination(BROADCAST_MAC_ADDRESS);
			macBeacon->setSequenceNumber(beacon_cont++);
			macBeacon->setKindDSME(BEACON_PACKET);
			
		    if(channelDiversity != 0) {//if using channel adaptation			
			/*	if(channel_bitmap != 0 && !channelbm_sent) {
					channelbm_sent = true;
				}
				else {
					channelbm_sent = false;
					channel_bitmap = 0;
				}*/
				for(int i = 1; i < QTDNODES; i++) {
					cont_interval[i]++;
					if(cont_interval[i] >= 10) {
						channel_bitmap = (channel_bitmap) | (1 << (i-1));
						channel[i] = (channel[i]+1)%16;
						trace() << "Change channel of " << i << ": " << channel[i] << " due to deep fading";
						PRR[i] = 1; //reset the PRR value
						window[i] = 0;
						cont_interval[i] = 0;
					}
				}
				macBeacon->setChannelBitmap(channel_bitmap);
			}
			
			trace() << "Transmitting beacon " << macBeacon->getSequenceNumber() << " on channel: " << channel[0];
			SF_offset = macBeacon->getSequenceNumber();
				
			toRadioLayer(macBeacon);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			
			setTimer(SEND_BEACON, ((32*slotSize)/0.1));	//32 -> two superframes
			
			setTimer(BEGIN_CFP, (8*(slotSize)/0.1)); //goes directly to the CFP period
			cfp_cont = 0;
			cfp_cont_relative = 0;
			ack_bitmap = 0;
			slot_attempt = SLOT_1;
			
			break;
		}
		case BEGIN_CFP: {
			if(channelDiversity == 0) { //channel hoping	
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
							
				int node_id = 0;//TODO: correct to configurations where nodes different from 0 receive the packets idSlots[cfp_cont];			
				int l;
				if(SDIndex == 0) {
					l = 7;
				}
				else {
					l = 15;
				}
			//	trace() << "l: " << l << " SSind: " << SDIndex << " node_id: " << node_id << " cfpcont: " << cfp_cont_relative << " SF_offset: " << SF_offset;
				int id_ch = (l*SDIndex + cfp_cont_relative + node_id + SF_offset)%16;
				channel[idSlots[cfp_cont]] = id_ch;
				double new_channel = 2405.0 + (id_ch%16)*5;
				radioCmd->setParameter(new_channel);
			//	trace() << "New channel: " << new_channel;
		 		send(radioCmd, "toRadioModule");	
				if(cfp_cont == 6) {
					SDIndex++;
				}
				else if(cfp_cont == 21) {
					SDIndex = 0;
				}
			}
			else { //channel adaptation
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
								
				int node_id = idSlots[cfp_cont];			
											
				double new_channel = 2405.0 + channel[node_id]*5;
				radioCmd->setParameter(new_channel);
			//	trace() << "New channel: " << new_channel;
		 		send(radioCmd, "toRadioModule");
			//	trace() << "Slot " << cfp_cont << " channel " << channel[node_id];
			}
			//buffer of packets to be retransmitted
			if(buffer_ret.size() > 0 && slots[cfp_cont] && (cfp_cont > firstAck)) {
				packet_ret *pr = buffer_ret.front();
				buffer_ret.pop_front();
	
				if(pr->cont < this->numberOfAttempts) {
					packet_ret *pr2 = new packet_ret;
					pr2->cont = pr->cont+1;
			
					pr2->packet = pr->packet->dup();
					pr2->packet->setSource(pr->packet->getSource());
					pr2->packet->setDestination(pr->packet->getDestination());
					pr2->packet->setAckReq(pr->packet->getAckReq());
					pr2->packet->setKindDSME(DATA_PACKET);
					buffer_ret.push_back(pr2);
					trace () << "Listening to ack";
				//	trace() << "Colocando pacote para retransmissão depois da ret...";						
				}	
				trace() << "Retransmitting packet " << pr->packet->getSequenceNumber() << " slot: " << cfp_cont << " on channel: " << channel[pr->packet->getSource()];
				toRadioLayer(pr->packet);
				toRadioLayer(createRadioCommand(SET_STATE, TX));	
			}
			//buffer of new packets
			else if(slots[cfp_cont] && buffer.size() > 0) {
				DSMEACKPacket *macFrame = new DSMEACKPacket("DSME packet", MAC_LAYER_PACKET);
				packet *p = buffer.front();
				cPacket *pkt = p->pkt;
				int destination = p->dest;
				buffer.pop_front();
				encapsulatePacket(macFrame, pkt);
				macFrame->setSource(SELF_MAC_ADDRESS);
				macFrame->setDestination(destination);
				macFrame->setAckReq(p->ack);
				macFrame->setKindDSME(DATA_PACKET);
				
				if(p->ack && this->numberOfAttempts > 0) {
					packet_ret *pr = new packet_ret;
					pr->packet = macFrame->dup();
					pr->packet->setSource(SELF_MAC_ADDRESS);
					pr->packet->setDestination(destination);
					pr->packet->setAckReq(p->ack);
					pr->packet->setKindDSME(DATA_PACKET);
					pr->cont = 1;
					buffer_ret.push_back(pr);
					trace () << "Listening to ack";
				//	trace() << "Colocando pacote para retransmissão...";
				}
				
				trace() << "Transmitting packet " << macFrame->getSequenceNumber() << " slot: " << cfp_cont << " on channel: " << channel[macFrame->getSource()];
				
				toRadioLayer(macFrame);
				toRadioLayer(createRadioCommand(SET_STATE, TX));
			}
			
			cfp_cont++;
			
			if(cfp_cont != 8)
				cfp_cont_relative++;
			
			if(cfp_cont == 7) {
				cfp_cont_relative = 0;
			//	cfp_cont++; //goes directly to after the beacon timeslot
			}
			
			if(cfp_cont < 22) { //31 -> two superframes (extract the 8 from the CAP)
				setTimer(BEGIN_CFP, ((slotSize)/0.1));
			}
			else if(!isCoordinator) {
				if(channelDiversity == 2) { //channel hopping for the beacons
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
					ch_beacon = (ch_beacon+1)%16;
					channel[0] = ch_beacon; //TODO: customize beacon ID -> here is set to 0
					double new_channel = 2405.0 + 5*ch_beacon;
					radioCmd->setParameter(new_channel);
			 		send(radioCmd, "toRadioModule");
				}
				else {	//always use channel 11 for beacons (modes 0 and 1)
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
					double new_channel = 2405.0; //CHANNEL 11
					channel[0] = 0; //TODO: customize beacon ID -> here is set to 0
					radioCmd->setParameter(new_channel);
			 		send(radioCmd, "toRadioModule");
				}
				trace () << "Listening to beacon";
			}
			break;
		}
	}
}



