#include "ABMPTree.h"
#include <vector>
#include <string.h>

Define_Module(ABMPTree);

void inline ABMPTree::medianValue(uint8_t node_addr) {
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

void inline ABMPTree::mediana(uint8_t node_addr) {
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

void ABMPTree::calculatePRRu(uint8_t node_addr) { 
    if(avg_s[node_addr] > 0.5) {
        prr_u[node_addr] = 1.0;
    }
    else{
        prr_u[node_addr] = 526.56*pow(avg_s[node_addr],6) - 1333.2*pow(avg_s[node_addr],5) + 1315.5*pow(avg_s[node_addr],4) - 639.22*pow(avg_s[node_addr],3)+159.31*avg_s[node_addr]*avg_s[node_addr] - 19.251*avg_s[node_addr] + 0.9287;
        prr_u[node_addr] = 1.0-prr_u[node_addr];
    }
}

void ABMPTree::calculatePRRd(uint8_t node_addr) {
    prr_d[node_addr] = prr_d[node_addr]*0.6 +  (0.0014*avg_dup[node_addr]*avg_dup[node_addr] - 0.0742*avg_dup[node_addr] + 0.9889)*0.4;
}

void ABMPTree::startup() {
	//if(SELF_MAC_ADDRESS!=8 && SELF_MAC_ADDRESS!=0) return;
	setTimerDrift(0.0001);
	double time_offset = simTime().dbl();

	this->treeTopology = par("treeTopology");
	this->allocatedSlots = par("allocatedSlots").stdstringValue();
	//this->slots = new bool[16];
	this->numberOfAttempts = par("numberOfAttempts").longValue();
	this->slotframesize = par("slotFrameSize");	
	
	memset(this->slots,0,this->slotframesize*sizeof(bool));
	
	for(int i = 0; i < allocatedSlots.size(); i++) {
		this->slots[i] = this->allocatedSlots[i]-'0';
	}

	this->isCoordinator = par("isCoordinator");
	
	this->slotSize = par("slotSize"); //in ms
	
	//AMBPTree
	this->QTDNODES = par("qtdNodesCluster");
	this->isClusterHead = par("isClusterHead");
	this->deep_backward_ch = -1;
	
	if(isCoordinator) {	
		setTimer(SEND_BEACON, (1.0-time_offset)/0.0001); //initialoffset for sending a beacon
		setTimer(WC_UP, 600000);
	}
	else{
		setTimer(BEACON_TIMEOUT_RESTART, ((slotframesize*100*slotSize)/0.1));	
		this->cluster_id = par("clusterID");
	}
	if(isCoordinator || isClusterHead) {
		setTimer(LQ_EST, 50010); //new value of estimation for each 5 seconds
	}

	this->nodeIdSlot = par("nodeIdSlot").stdstringValue();
	memset(this->slots_rx,0,MAXNODES*sizeof(bool));
	int k = 0;
    for(int i = 0; i < nodeIdSlot.size()/2; i++) {
    	idSlots[i] = (int)(nodeIdSlot[k]-'0')*10 + (int)(nodeIdSlot[k+1]-'0');
		k = k+2;
		if (idSlots[i] != SELF_MAC_ADDRESS) {
			if(isClusterHead || isCoordinator) {
				if(isClusterHead && (idSlots[i] != cluster_id)) {
					nodes_cluster.push_back(idSlots[i]); //list of end nodes in the cluster
					this->slots_rx[i] = true;
				}
				else if(!isClusterHead){
					nodes_cluster.push_back(idSlots[i]); //list of CH in the cluster of the sink
					this->slots_rx[i] = true;
				}
			}
			
		}
    }	

	this->beacon_cont = 0;
	this->cfp_cont = 0;
	
	//TODO: alocacao dinamica
	for(int i = 0; i < 100; i++) {
		this->ack_bitmap[i] = 0;
	}
	
	for(int i = 0; i < MAXNODES; i++) {
		this->fail_cont[i] = 0;
		this->window[i] = 0;
		this->id_ant[i] = 0;
		this->cont_interval[i] = 0;
		this->prr_u[i] = 1;
		this->avg_dup[i] = 0;
		this->prr_d[i] = 1;
		this->startedEstimation[i] = false;
		this->new_value[i] = false;
		this->channel[i] = 0;
	}
	
	if(isClusterHead || isCoordinator) {
		string channels_aux = par("channels");
		for(int i = 0; i < channels_aux.size(); i++) {
			channels_adapt[i] = (bool)(channels_aux[i] - '0'); 
		}
		int indch = 0;
		for(int i = 0; i < 16; i++) {
			if(channels_adapt[i]) {
				indch = i;
				break;
			}
		}
		for(int i = 0; i < MAXNODES; i++) { //canais iniciais dentro do cluster
			this->channel[i] = indch;
		}
	}
	
		
	this->prrThreshold = par("prrThreshold").doubleValue();
	this->beaconThreshold = par("beaconThreshold").doubleValue();
	this->channel_bitmap = 0;
	this->ch_beacon = 0;
	
	this->macBufferSize = par("macBufferSize").longValue();
	this->cont_interval_tx = 0;
	
	this->beacon_id = 0;
	this->blacklist = 0x00FF;
	for(int i = 0; i < 8; i++) {
		this->blacklist_aux[i] = i;
	}
	for(int i = 0; i < 8; i++) {
		map_chtobeacon[i] = i;
	}
	for(int i = 8; i < 16; i++) {
		map_chtobeacon[i] = -1;
	}
	this->first_ch = 0;
	for(int i = 0; i < 16; i++)
		this->ch_quality[i] = 1;
	this->blacklist_size = 8;
	this->waiting_beacon = true;
	this->beaconReceived = false;
	memset(this->cont_restart,0,MAXNODES*sizeof(int));
	memset(this->switch_channel,0,MAXNODES*sizeof(bool));
	this->waiting_b0 = true;
	this->b0_error = 0;
	memset(this->cont_packet_total,0,16*sizeof(int));
	memset(this->cont_packet_pernode,0,(16*MAXNODES)*sizeof(int));
	memset(this->cont_dup,0,16*sizeof(int));
	memset(this->cont_dup_pernode,0,(16*MAXNODES)*sizeof(int));
	this->start_change = false;
	this->deep_beacon0 = false;
	
	RadioControlCommand *radioCmd = new RadioControlCommand();
	radioCmd->setKind(RADIO_CONTROL_COMMAND);
	radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
	double new_channel = 2405.0 + 5*((first_ch+cluster_id)%16);
	radioCmd->setParameter(new_channel);
	send(radioCmd, "toRadioModule");	
	
	if(!isCoordinator){
		trace () << "Listening to beacon";
	}
		
} 	

/* Handle packet received from upper (network) layer.
 * We need to create a MAC packet, (here it can just be the generic MacPacket)
 * and encapsulate the received network packet before forwarding it to RadioLayer
 */

void ABMPTree::fromNetworkLayer(cPacket * pkt, int destination)
{
	//if(SELF_MAC_ADDRESS!=8 && SELF_MAC_ADDRESS!=0) return;
	
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


void ABMPTree::fromNetworkLayer(cPacket * pkt, int destination, bool ack)
{
	//if(SELF_MAC_ADDRESS!=8 && SELF_MAC_ADDRESS!=0) return;
	
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
void ABMPTree::fromRadioLayer(cPacket * pkt, double rssi, double lqi)
{
//	//if(SELF_MAC_ADDRESS!=8 && SELF_MAC_ADDRESS!=0) return;
	ABMPTreePacket *macPkt = dynamic_cast <ABMPTreePacket*>(pkt);
	if (macPkt == NULL)
		return;	

	if(macPkt->getKindDSME() == BEACON_PACKET && this->cluster_id == macPkt->getSource() && waiting_beacon) {
		if(!isClusterHead) {
			trace() << "Beacon received " << macPkt->getBeaconId();
			cancelTimer(BEACON_TIMEOUT_RESTART);
		
			cont_restart[SELF_MAC_ADDRESS] = 0;
			cfp_cont = 0;
			SF_offset = macPkt->getSequenceNumber();
	   
			//setTimer(BEGIN_CFP, (((8*slotSize)-0.212)/0.1)); //in this implementation the capReduction is enabled, and the nodes go directly to the CFP.
			setTimer(UPDATE_CHANNEL_CFP, ((slotSize-1.212)/0.1));
			
			setTimer(BEACON_TIMEOUT, (((slotframesize+1)*slotSize)/0.1));
		
			beacon_id = macPkt->getBeaconId();
		
			/*if(beacon_id%blacklist_size == 0) {
				b0_error = 0;
			*/
		///	trace() << "Change1: " << macPkt->getChangebl();
		//	trace() << "Change: " << start_change;
			if(!start_change && macPkt->getChangebl()) {
				//começa a mudar na próxima
				start_change = true;
				old_first_ch = first_ch;
				old_blacklist = blacklist;		
			}
		
			blacklist = macPkt->getBlacklist();
			channel[SELF_MAC_ADDRESS] = macPkt->getChannel()[SELF_MAC_ADDRESS];
			first_ch = macPkt->getFirstch();

			//trace() << "Received beacon " << beacon_id << " Channel: " << channel[SELF_MAC_ADDRESS] << " Firstch: " << first_ch << " Blacklist: " << blacklist;
		
			waiting_beacon = false;
			beaconReceived = true;
		
			//trace() << "Received ack, bitmap: " << macPkt->getAckBitmap();
			int *ackbm = macPkt->getAckBitmap();
		
			bool ackRecv = false;
		
		    for(int i = 0; i < slotframesize; i++) {
		    	//if(slots[i] && (ackbm & (1 << i))) {
				if(slots[i] && ackbm[i] == 1) {
		    		for(int j = 0; j < buffer_ret.size(); j++) {
		    			if(buffer_ret[j]->slot == i) {
						//	trace() << "Recebido o ack para o pacote " << buffer_ret[j]->packet->getSequenceNumber();
		    				packet_ret *paux = buffer_ret[j];
							delete paux->packet;
														
							buffer_ret.erase(buffer_ret.begin()+j);
							
							ackRecv = true;
		    			}
		    		}
					if(!ackRecv) {
					//	trace() << "Recebido ack para pacote que já retransmitido";
					}
		    	}
		    }
		/*	if((ackbm & (1 << slot_tx))) {
				txSuccess = true;
			}
		
			if(txSuccess && buffer_ret.size() > 0) {
				trace() << "Received ack for the last packet";
				buffer_ret.pop_front();
			}
			else if(txSuccess){
				//trace() << "Received ack for the last packet - G2 ";
			}*/
		}
		else { //is ClusterHead
		//	trace() << "Beacon received - CH... " << macPkt->getBeaconId();
			
			cancelTimer(BEACON_TIMEOUT_RESTART);
		
			cont_restart[SELF_MAC_ADDRESS] = 0;
			cfp_cont = 0;
			SF_offset = macPkt->getSequenceNumber();
	   
			//setTimer(BEGIN_CFP, (((8*slotSize)-0.212)/0.1)); //in this implementation the capReduction is enabled, and the nodes go directly to the CFP.
			setTimer(UPDATE_CHANNEL_CFP, ((slotSize-0.212)/0.1));
		
			setTimer(BEACON_TIMEOUT, (((slotframesize+1)*slotSize)/0.1));
		
			beacon_id = macPkt->getBeaconId();
		
			/*if(beacon_id%blacklist_size == 0) {
				b0_error = 0;
			*/
		///	trace() << "Change1: " << macPkt->getChangebl();
		//	trace() << "Change: " << start_change;			
			if(!start_change && macPkt->getChangebl()) {
				//começa a mudar na próxima
				start_change = true;
				old_first_ch = first_ch;
				old_blacklist = blacklist;		
			}
		
			blacklist = macPkt->getBlacklist();
			channel[SELF_MAC_ADDRESS] = macPkt->getChannel()[SELF_MAC_ADDRESS];
			first_ch = macPkt->getFirstch();
		
		//	trace() << "Received beacon " << beacon_id << " Channel: " << channel[SELF_MAC_ADDRESS] << " Firstch: " << first_ch << " Blacklist: " << blacklist;
		
			waiting_beacon = false;
			beaconReceived = true;
		
			//trace() << "Received ack, bitmap: " << macPkt->getAckBitmap();
				
			int *ackbm = macPkt->getAckBitmap();
		
			bool ackRecv = false;
		
		    for(int i = 0; i < slotframesize; i++) {
		    	//if(slots[i] && (ackbm & (1 << i))) {
		    	if(slots[i] && ackbm[i] == 1) {
		    		for(int j = 0; j < buffer_ret.size(); j++) {
		    			if(buffer_ret[j]->slot == i) {
						//	trace() << "Recebido o ack para o pacote " << buffer_ret[j]->packet->getSequenceNumber();
		    				buffer_ret.erase(buffer_ret.begin()+j);
							ackRecv = true;
		    			}
		    		}
					if(!ackRecv) {
					//	trace() << "Recebido ack para pacote que já retransmitido";
					}
		    	}
		    }
		
			/*bool txSuccess = false;
		
			if((ackbm & (1 << SELF_MAC_ADDRESS))) {
				txSuccess = true;
			}
		
			if(txSuccess && buffer_ret.size() > 0) {
				trace() << "Received ack for the last packet";
				buffer_ret.pop_front();
			}
			else if(txSuccess){
				//trace() << "Received ack for the last packet - G2 ";
			}*/
		}
				
	}
	else if (macPkt->getDestination() == cluster_id && isClusterHead && slots_rx[cfp_cont]) {
		if(macPkt->getKindDSME() == DATA_PACKET) { //DATA PACKET	
			trace() << "Received packet at cluster head: " << macPkt->getSequenceNumber() << "  from: " << macPkt->getSource() << " Power: " << rssi;
			
			//TODO: integrate the link quality estimation
			
			int p_id = beacon_id%blacklist_size;
			if(p_id != 0) {
				cont_packet_total[p_id]++;
				cont_packet_pernode[p_id][macPkt->getSource()]++;
				//trace() << "Received packet beacon " << p_id << " Total: " << cont_packet_total[p_id];
			}
			
			if(macPkt->getB0error()) {
				cont_dup[0]++;
				cont_dup_pernode[0][macPkt->getSource()]++;
				//trace() << "Perdeu b0, canal " << first_ch;
			}
			
			this->cont_interval[macPkt->getSource()] = 0;
			
			//ack_bitmap = ack_bitmap | (1 << (macPkt->getSource())); //bitmap de acordo com a fonte
			
			//trace() << "Ack para o slot " << (cfp_cont-1);
			
			ack_bitmap[cfp_cont-1] = 1;// = ack_bitmap | (1 << (cfp_cont-1)); //bitmap de acordo com a fonte
		
			//TODO: integrate the link quality estimation
			//LQ calculation
			if(macPkt->getSequenceNumber() <= id_ant[macPkt->getSource()]) {				
				int ant = beacon_id-2;
				if(ant == -1) ant = blacklist_size-1;
				else if(ant == - 2) ant = blacklist_size-2;
				ant = ant%blacklist_size;
				//trace() << "Duplicado - anterior " <<  ant;
				if(ant != 0) {
					cont_dup[ant]++;
					cont_dup_pernode[ant][macPkt->getSource()]++;
				}
			}
			else { //only put the non duplicate packets on the buffer
				ABMPTreePacket *pkt = macPkt->dup();
				pkt->setSource(macPkt->getSource());
				pkt->setDestination(macPkt->getDestination());
				pkt->setSequenceNumber(macPkt->getSequenceNumber());
				buffer_ch.push_back(pkt);
				//trace() << "Colocando pacote " << pkt->getSequenceNumber() << " from " << pkt->getSource() << " on the buffer";
				if(buffer_ch.size() > macBufferSize) {
				//	trace() << "Buffer cheio, retirando pacote " << buffer_ch.front()->getSequenceNumber() << " from " << buffer_ch.front()->getSource() << " of the buffer";
					buffer_ch.pop_front();
				}
			}
			id_ant[macPkt->getSource()] = macPkt->getSequenceNumber();
			
			
		  	node_addr = macPkt->getSource();
			
		//	trace() << "Node_addr: " << node_addr;
			rssi_values[contann[node_addr]%ESTWINDOW][node_addr] = (double)((rssi+94.0)/0.25);
			contann[node_addr]++;
			
		    if(contann[node_addr] == ESTWINDOW || startedEstimation[node_addr]) {
				avg_s[node_addr] = 0;
			    mediana(node_addr);
			    calculatePRRu(node_addr);
			 //   trace() << "Calc LQu " << node_addr <<" "<< (prr_u[node_addr]);
				new_value[node_addr] = true;
				startedEstimation[node_addr] = true;
			}
			
		}	
	}
	else if ((macPkt->getDestination() == SELF_MAC_ADDRESS ||
	    macPkt->getDestination() == BROADCAST_MAC_ADDRESS) && slots_rx[cfp_cont])
	{	
		if(macPkt->getKindDSME() == DATA_PACKET) { //DATA PACKET
			int p_id = beacon_id%blacklist_size;
			cont_packet_total[p_id]++;
			cont_packet_pernode[p_id][idSlots[cfp_cont]]++;
			//trace() << "Received packet beacon " << p_id << " Total: " << cont_packet_total[p_id];
			
			if(macPkt->getB0error()) {
				cont_dup[0]++;
				cont_dup_pernode[0][idSlots[cfp_cont]]++;
			//	trace() << "Perdeu b0, canal " << first_ch;
			}
			
			this->cont_interval[idSlots[cfp_cont]] = 0; //zera o contador do clusterhead que encaminhou
			
			ack_bitmap[cfp_cont] = 1;// = ack_bitmap | (1 << cfp_cont);
			//trace() << "Ack para o slot " << cfp_cont;
		
			trace() << "Received packet at sink node: " << macPkt->getSequenceNumber() << " Through:  " << idSlots[cfp_cont] << " Power: " << rssi << " from: " << macPkt->getSource(); 
			deep_backward_ch = macPkt->getBeaconDup();
			if(deep_backward_ch != -1) {
				if(blacklist_aux[0] == deep_backward_ch) {
					deep_beacon0 = true;
				//	trace() << "Tirar canal " << deep_backward_ch;
				}
			}
		
			//LQ calculation
			if(macPkt->getSequenceNumber() <= id_ant[macPkt->getSource()]) {				
				int ant = beacon_id-2;
				if(ant == -1) ant = blacklist_size-1;
				else if(ant == - 2) ant = blacklist_size-2;
				ant = ant%blacklist_size;
				//trace() << "Duplicado - anterior " <<  ant;
				if(ant != 0) {
					cont_dup[ant]++;
					cont_dup_pernode[ant][idSlots[cfp_cont]]++;
				}
			}
			else {
				toNetworkLayer(decapsulatePacket(macPkt));
			}
			id_ant[macPkt->getSource()] = macPkt->getSequenceNumber();
						
		    node_addr = idSlots[cfp_cont];
		//	trace() << "Node_addr: " << node_addr;
		    rssi_values[contann[node_addr]%ESTWINDOW][node_addr] = (double)((rssi+94.0)/0.25);
		    contann[node_addr]++;
		    if(contann[node_addr] == ESTWINDOW || startedEstimation[node_addr]) {
				avg_s[node_addr] = 0;
			    mediana(node_addr);
			    calculatePRRu(node_addr);
			 //   trace() << "Calc LQu " << node_addr <<" "<< (prr_u[node_addr]);
				new_value[node_addr] = true;
				startedEstimation[node_addr] = true;
			}
			//trace() << "Ack_bitmap: " << ack_bitmap;
	    }
	}
}

void ABMPTree::timerFiredCallback(int index)
{
	////if(SELF_MAC_ADDRESS!=8 && SELF_MAC_ADDRESS!=0) return;
	
	switch (index){
		case WC_UP: { //only in the mode with channel hopping for the beacons		
			RadioControlCommand *radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(WC_UPD);
	 		send(radioCmd, "toRadioModule");	
			setTimer(WC_UP, 600000);
	    }
		case LQ_EST: { //performs a new Link Quality Estimation
			//trace() << "Verifying the estimations...";
			for(int i = 0; i < nodes_cluster.size(); i++) {		
				trace() << "LQu " << nodes_cluster[i] <<" "<< (prr_u[nodes_cluster[i]]);
				if(new_value[nodes_cluster[i]] && prr_u[nodes_cluster[i]] < prrThreshold) {
					trace() << "1 - Change channel of " <<  nodes_cluster[i] << ": " << channel[nodes_cluster[i]];
					switch_channel[nodes_cluster[i]] = true;
					contann[nodes_cluster[i]] = 0;
					prr_u[nodes_cluster[i]] = 1;
					startedEstimation[nodes_cluster[i]] = false;
		 		}
			}
			setTimer(LQ_EST, 20000);
			break;	
		}
		case BEACON_TIMEOUT_RESTART: { 
			RadioControlCommand *radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			first_ch = (first_ch+1)%16;
			channel[cluster_id] = first_ch; //TODO: customize beacon ID -> here is set to 0
			double new_channel = 2405.0 + 5*first_ch;
			radioCmd->setParameter(new_channel);
	 		send(radioCmd, "toRadioModule");	
			trace() << "Restart state, waiting on channel " << first_ch;
			setTimer(BEACON_TIMEOUT_RESTART, ((slotframesize*10*slotSize)/0.1));	
			break;
		}
		case BEACON_TIMEOUT: { //only in the mode with channel hopping for the beacons
			trace() << "Timeout expired, lost beacon";
			
			if(beacon_id%blacklist_size == 0) {
				if(buffer_ret.size() > 0) {
					buffer_ret.pop_front();
				}
				b0_error = 1;
			//	trace() << "Erro na transmissão, não recebeu beacon 0";
			}
			
			cont_restart[SELF_MAC_ADDRESS]++;
			
			if(cont_restart[SELF_MAC_ADDRESS] == MAX_RESTART) {
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
				double new_channel = 2405.0 + 5*first_ch;
				channel[cluster_id] = first_ch; //TODO: customize beacon ID -> here is set to 0
				radioCmd->setParameter(new_channel);
		 		send(radioCmd, "toRadioModule");	
				setTimer(BEACON_TIMEOUT_RESTART, ((slotframesize*10*slotSize)/0.1));	
				trace() << "Restart state, waiting on channel " << first_ch;
				break;
			}
			
			if(start_change) {
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			
				beacon_id++;
			
				int ch_beacon = beacon_id%blacklist_size;
			
				int indch = old_first_ch;
				for(int i = 0; i < ch_beacon; i++) {
					indch = (indch+1)%16;
					while(!((old_blacklist >> indch)&1))
						indch = (indch+1)%16;
				}
			
				double new_channel = 2405.0 + 5*indch;
			
			
				channel[cluster_id] = indch; //TODO: customize beacon ID -> here is set to 0
				radioCmd->setParameter(new_channel);
	 		
				send(radioCmd, "toRadioModule");			
				setTimer(BEACON_TIMEOUT, (((slotframesize)*slotSize)/0.1));	
			}
			else{
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			
				beacon_id++;
			
				int ch_beacon = (beacon_id+cluster_id)%blacklist_size;
			
				int indch = first_ch;
				for(int i = 0; i < ch_beacon; i++) {
					indch = (indch+1)%16;
					while(!((blacklist >> indch)&1))
						indch = (indch+1)%16;
				}
			
				double new_channel = 2405.0 + 5*indch;
			
			
				//channel[0] = indch; //TODO: customize beacon ID -> here is set to 0
				radioCmd->setParameter(new_channel);
	 		
				send(radioCmd, "toRadioModule");			
				setTimer(BEACON_TIMEOUT, (((slotframesize)*slotSize)/0.1));
			}
			
			break;
		}
		case SEND_BEACON: {	
			
			if(beacon_id%blacklist_size == 0) {
				
				cont_packet_total[0] += nodes_cluster.size();
				for(int i = 0; i < nodes_cluster.size();i++) 
					cont_packet_pernode[0][nodes_cluster[i]]++;
				
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
						
				double new_channel = 2405.0 + 5*first_ch;
			
				channel[SELF_MAC_ADDRESS] = first_ch; //TODO: customize beacon ID -> here is set to 0
				
				radioCmd->setParameter(new_channel);			
		 		send(radioCmd, "toRadioModule");
				//trace() << "channel beacon: " << new_channel;
				if(start_change) {
					start_change = false;
				}
				else {
					
					//INCLUDE ANALYSIS OF DEEP FADING PER NODE
					for(int i = 0; i < blacklist_size; i++) {
						float N = (float)cont_packet_total[i];
						if((N + cont_dup[i]) > (nodes_cluster.size()*10)) { //10*QTDNODES (pacotes recebidos + duplicações notadas)
							bad_ch = blacklist_aux[i];
							if(N > 0) {
								ch_quality[bad_ch] = 0.4*((N+cont_dup[i])/N) + 0.6*ch_quality[bad_ch];
							}
							else {
								ch_quality[bad_ch] = 0.4*numberOfAttempts + 0.6*ch_quality[bad_ch];
							}
							//trace() << "Average number of ret beacon " << i << " and ch: " << bad_ch << " for : " << N << " packets and " << cont_dup[i] << " dup: " << ch_quality[bad_ch] ;
					
							float v = 1.0/ch_quality[bad_ch];
							
						 /*   avg_dup[node_addr] = cont_dup[i]/(N/10.0);
						    calculatePRRd(node_addr);
						    avg_dup[node_addr] = 0;
						    trace() << "LQd: " << (prr_d[node_addr]);
							trace() << "v: " << v;*/

							bool deep_backward = false;
							if(i == 0) {
								
								for(int l = 0; l < nodes_cluster.size(); l++) {
									float N_b = (float)cont_packet_pernode[i][nodes_cluster[l]];
									if(N_b > 10) {
										N_b = (float)(N_b+cont_dup_pernode[i][nodes_cluster[l]])/N_b;
										N_b = 1.0/N_b;
									//	trace() << "No " << nodes_cluster[l] << " beacon " << i << " Nb " << N_b;
										if(N_b < (beaconThreshold/2.0)) {
										//	trace() << "A qualidade para o no " << nodes_cluster[l] << " e beacon " << i << " esta ruim " << N_b;
											deep_backward = true;
										}
										cont_dup_pernode[i][nodes_cluster[l]] = cont_packet_pernode[i][nodes_cluster[l]] = 0;
									}		
								}
							}
							
							if(v < beaconThreshold || deep_backward) {
								//COMO FAZER A MUDANCA SO NO OUTRO CICLO?
								start_change = true;
								//ARMAZENAR UM ESTADO NO TRANSMISSOR E NO RECEPTOR
								old_blacklist = blacklist;
								old_first_ch = first_ch;
							
								float min_v = 10;
								int j = (bad_ch+1)%16;
								int best_ch = j;
							//	trace() << "Blacklist " << blacklist;
								while(1) {
									//trace() << "Analisando canal " << j << " " << (blacklist >> j) << " " << ch_quality[j];	
									if(((blacklist >> j)&1) == 0 && ch_quality[j] < min_v) {
										min_v = ch_quality[j];
										best_ch = j;
									//	trace() << "Melhor por enquanto " << j;
										
									}
									j = (j+1)%16;
									if(min_v == 1 || j == bad_ch) {
										break;
									}
								}
								trace() << "Saiu " << bad_ch << " e entrou " << best_ch;
								
					
								if(bad_ch == first_ch) {
									deep_beacon0 = false;
									int nc = (first_ch+1)%16;
									int best_1ch = nc;
									min_v = 10;
									while(1) {
										if(((blacklist >> nc)&1) == 1 && ch_quality[nc] < min_v) {
											min_v = ch_quality[nc];
											best_1ch = nc;
										}
										if(min_v == 1 || nc == first_ch) {
											break;
										}
										nc = (nc+1)%16;
									}
									first_ch = best_1ch;
									trace() << "Novo first_channel " << first_ch;
								}
								blacklist = blacklist ^ (1 << bad_ch);
								blacklist = blacklist | (1 << best_ch);
								blacklist_aux[0] = first_ch; //fica de fora durante um tempo
								int indch = (first_ch+1)%16;
								int k = 1;
								while(1) {
									if((blacklist>>indch)&1) {
										blacklist_aux[k] = indch;
										k++;
									}
									if(k == 16) {
										break;
									}
									indch = (indch+1)%16;
								}							
								ch_quality[best_ch] = 1;
				
								for(int k = 0; k < blacklist_size; k++) { 
									cont_packet_total[k] = cont_dup[k] = 0;
								}
								break;
							}
							else if(deep_beacon0) {
								deep_beacon0 = false;
								start_change = true;
								old_blacklist = blacklist;
								old_first_ch = first_ch;
								
								bad_ch = blacklist_aux[0];
								
								float min_v = 10;
								int j = (bad_ch+1)%16;
								int best_ch = j;
							//	trace() << "Blacklist " << blacklist;
								while(1) {
									//trace() << "Analisando canal " << j << " " << (blacklist >> j) << " " << ch_quality[j];	
									if(((blacklist >> j)&1) == 0 && ch_quality[j] < min_v) {
										min_v = ch_quality[j];
										best_ch = j;
									//	trace() << "Melhor por enquanto " << j;
										
									}
									j = (j+1)%16;
									if(min_v == 1 || j == bad_ch) {
										break;
									}
								}
								trace() << "CH - Saiu " << bad_ch << " e entrou " << best_ch;
								if(bad_ch == first_ch) {
									int nc = (first_ch+1)%16;
									int best_1ch = nc;
									min_v = 10;
									while(1) {
										if(((blacklist >> nc)&1) == 1 && ch_quality[nc] < min_v) {
											min_v = ch_quality[nc];
											best_1ch = nc;
										}
										if(min_v == 1 || nc == first_ch) {
											break;
										}
										nc = (nc+1)%16;
									}
									first_ch = best_1ch;
									trace() << "CH - Novo first_channel " << first_ch;
								}
								blacklist = blacklist ^ (1 << bad_ch);
								blacklist = blacklist | (1 << best_ch);
								blacklist_aux[0] = first_ch; //fica de fora durante um tempo
								int indch = (first_ch+1)%16;
								int k = 1;
								while(1) {
									if((blacklist>>indch)&1) {
										blacklist_aux[k] = indch;
										k++;
									}
									if(k == 16) {
										break;
									}
									indch = (indch+1)%16;
								}							
								ch_quality[best_ch] = 1;
				
								for(int k = 0; k < blacklist_size; k++) { 
									cont_packet_total[k] = cont_dup[k] = 0;
								}
								break;
								deep_backward_ch = -1;
							}
							cont_packet_total[i] = cont_dup[i] = 0;
						}
					}
				}	
			}
			else {
				if(!start_change) {
				
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			
					int ch_beacon = blacklist_aux[beacon_id%blacklist_size];
			
					/*int indch = first_ch;
					for(int i = 0; i < ch_beacon; i++) {
						indch = (indch+1)%16;
						while(!((blacklist >> indch)&1))
							indch = (indch+1)%16;
					}*/
			
					double new_channel = 2405.0 + 5*ch_beacon;
			
					channel[SELF_MAC_ADDRESS] = ch_beacon; //TODO: customize beacon ID -> here is set to 0
			
					radioCmd->setParameter(new_channel);			
			 		send(radioCmd, "toRadioModule");
				}
				else{
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			
					int ch_beacon = beacon_id%blacklist_size;
			
					int indch = old_first_ch;
					for(int i = 0; i < ch_beacon; i++) {
						indch = (indch+1)%16;
						while(!((old_blacklist >> indch)&1))
							indch = (indch+1)%16;
					}
			
					double new_channel = 2405.0 + 5*indch;
			
					channel[SELF_MAC_ADDRESS] = indch; //TODO: customize beacon ID -> here is set to 0
			
					radioCmd->setParameter(new_channel);			
			 		send(radioCmd, "toRadioModule");
				}
			}
			ABMPTreePacket *macBeacon; 
			macBeacon = new ABMPTreePacket("ABMPTree beacon", MAC_LAYER_PACKET);
			macBeacon->setSource(SELF_MAC_ADDRESS);
			macBeacon->setDestination(BROADCAST_MAC_ADDRESS);
			macBeacon->setSequenceNumber(beacon_cont++);
			macBeacon->setKindDSME(BEACON_PACKET);
			macBeacon->setBlacklist(blacklist);
			macBeacon->setFirstch(first_ch);
			
			macBeacon->setChangebl(start_change);
		    /*if(start_change) {
				trace() << "enviando o antigo...";
				macBeacon->setBlacklist(old_blacklist);
				macBeacon->setFirstch(old_first_ch);
			}*/
					
			if(beacon_id%blacklist_size == 0) {
				for(int i = 0; i < nodes_cluster.size(); i++) {
					if(switch_channel[nodes_cluster[i]]) {
						int change_aux = channel[nodes_cluster[i]];
						while(true) {
							change_aux = (change_aux+1)%16;
							if(channels_adapt[change_aux]) {
								break;
							}
						}
						channel[nodes_cluster[i]] = change_aux;
						trace() << "Change channel of " << nodes_cluster[i] << ": " << channel[nodes_cluster[i]];
						switch_channel[nodes_cluster[i]] = false;	
						window[nodes_cluster[i]] = 0;
						new_value[nodes_cluster[i]] = false;
					}
				}
			}
		
			macBeacon->setChannel(channel);	
			macBeacon->setBeaconId(beacon_id);
			macBeacon->setAckBitmap(ack_bitmap);
			
			for(int i = 0; i < nodes_cluster.size(); i++) {
				cont_interval[nodes_cluster[i]]++;
				if(cont_interval[nodes_cluster[i]] >= 40) { //TODO: tornar parametrizavel (vou colocar duas vezes o periodo da APP -> 40 para 1 packet/s)
					trace() << "Change channel of " << nodes_cluster[i] << ": " << channel[nodes_cluster[i]] << " due to deep fading";
					switch_channel[nodes_cluster[i]] = true;
					cont_interval[nodes_cluster[i]] = 0;
				}
			}		
			
			trace() << "Transmitting beacon " << macBeacon->getSequenceNumber() << " on channel: " << channel[SELF_MAC_ADDRESS];
			SF_offset = macBeacon->getSequenceNumber();
				
			toRadioLayer(macBeacon);
			toRadioLayer(createRadioCommand(SET_STATE, TX));
			beacon_id++;
			
			setTimer(SEND_BEACON, ((slotframesize*slotSize)/0.1));
			
			setTimer(UPDATE_CHANNEL_CFP, (slotSize/0.1)); //goes to time slot 1
			
			cfp_cont = 0;
			//TODO: alocacao dinamica
			for(int i = 0; i < 100; i++) {
				ack_bitmap[i] = 0;
			}
			
			break;
		}
		case UPDATE_CHANNEL_CFP: {
			cfp_cont++;
			if(!isClusterHead && !isCoordinator) {
				if(slots[cfp_cont]) {
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
													
					double new_channel = 2405.0 + channel[SELF_MAC_ADDRESS]*5;
					radioCmd->setParameter(new_channel);
			 		send(radioCmd, "toRadioModule");
					//trace() << "Indo para o canal " << channel[SELF_MAC_ADDRESS] << " tx do no " << SELF_MAC_ADDRESS << "slot " << cfp_cont;
				}
			}
			else if(isClusterHead && cfp_cont == 1) {
				int indch;
				if(!start_change) {
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
		
					//int ch_beacon = blacklist_aux[beacon_id%blacklist_size];
					int channel_ch = (beacon_id + SELF_MAC_ADDRESS)%blacklist_size;
					indch = first_ch;
					for(int i = 0; i < channel_ch; i++) {
						indch = (indch+1)%16;
						while(!((blacklist >> indch)&1))
							indch = (indch+1)%16;
					}
		
					double new_channel = 2405.0 + 5*indch;
		
					//channel[SELF_MAC_ADDRESS] = indch; //TODO: customize beacon ID -> here is set to 0
								
					radioCmd->setParameter(new_channel);			
			 		send(radioCmd, "toRadioModule");
				}
				else{
					RadioControlCommand *radioCmd = new RadioControlCommand();
					radioCmd->setKind(RADIO_CONTROL_COMMAND);
					radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
		
					int ch_beacon = (beacon_id+SELF_MAC_ADDRESS)%blacklist_size;
		
					indch = old_first_ch;
					for(int i = 0; i < ch_beacon; i++) {
						indch = (indch+1)%16;
						while(!((old_blacklist >> indch)&1))
							indch = (indch+1)%16;
					}
					
					double new_channel = 2405.0 + 5*indch;
		
					//channel[SELF_MAC_ADDRESS] = indch; //TODO: customize beacon ID -> here is set to 0
		
					radioCmd->setParameter(new_channel);			
			 		send(radioCmd, "toRadioModule");
				}
				//trace() << "Indo para o canal " << indch << " tx do no " << SELF_MAC_ADDRESS << "slot " << cfp_cont;
				
			}
			else {
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
					
				int node_id = idSlots[cfp_cont];	
								
				double new_channel = 2405.0 + channel[node_id]*5;
				radioCmd->setParameter(new_channel);
		 		send(radioCmd, "toRadioModule");
				//trace() << "Indo para o canal " << channel[node_id] << " tx do no " << node_id << "slot " << cfp_cont;
			}
			
			setTimer(BEGIN_CFP, (10)); //goes to time slot 1 (1ms)
			
			break;
		}
		case BEGIN_CFP: {
			if(!isCoordinator && !beaconReceived) {
				break;
			}
			if(!isClusterHead) {
				if(buffer_ret.size() > 0 && slots[cfp_cont] && !waiting_beacon) {// && (cfp_cont > firstAck)) {
					packet_ret *pr = buffer_ret.front();
					buffer_ret.pop_front();
	
					if(pr->cont < this->numberOfAttempts) {
						packet_ret *pr2 = new packet_ret;
						pr2->cont = pr->cont+1;
						pr->slot = cfp_cont;
			
						pr2->packet = pr->packet->dup();
						pr2->packet->setSource(pr->packet->getSource());
						pr2->packet->setDestination(pr->packet->getDestination());
						pr2->packet->setAckReq(pr->packet->getAckReq());
						pr2->packet->setKindDSME(DATA_PACKET);
						buffer_ret.push_back(pr2);
						waiting_beacon = true;
						//trace () << "Listening to beacon";
					//	trace() << "Colocando pacote para retransmissão depois da ret...";						
					}	
					trace() << "Retransmitting packet " << pr->packet->getSequenceNumber() << " slot: " << cfp_cont << " on channel: " << channel[pr->packet->getSource()];
					toRadioLayer(pr->packet);
					toRadioLayer(createRadioCommand(SET_STATE, TX));	
				}
				//buffer of new packets
				else if(slots[cfp_cont] && buffer.size() > 0) { // && (cfp_cont < firstAck)) {
					ABMPTreePacket *macFrame = new ABMPTreePacket("DSME packet", MAC_LAYER_PACKET);
					packet *p = buffer.front();
					cPacket *pkt = p->pkt;
					int destination = p->dest;
					buffer.pop_front();
					encapsulatePacket(macFrame, pkt);
					macFrame->setSource(SELF_MAC_ADDRESS);
					macFrame->setDestination(destination);
					macFrame->setAckReq(p->ack);
					macFrame->setKindDSME(DATA_PACKET);
				
					macFrame->setB0error(b0_error);
					b0_error = 0;
				
					if(p->ack && this->numberOfAttempts > 0) {
						packet_ret *pr = new packet_ret;
						pr->packet = macFrame->dup();
						pr->packet->setSource(SELF_MAC_ADDRESS);
						pr->packet->setDestination(destination);
						pr->packet->setAckReq(p->ack);
						pr->packet->setKindDSME(DATA_PACKET);
						pr->cont = 1;
						pr->slot = cfp_cont;
						buffer_ret.push_back(pr);
						waiting_beacon = true;
						//trace () << "Listening to beacon";
					//	trace() << "Colocando pacote para retransmissão...";
					}
				
					trace() << "Transmitting packet " << macFrame->getSequenceNumber() << " slot: " << cfp_cont << " on channel: " << channel[macFrame->getSource()];
				
					toRadioLayer(macFrame);
					toRadioLayer(createRadioCommand(SET_STATE, TX));
				}
			}
			else if(isClusterHead){
				if(cfp_cont == 1) { //send_the_beacon
					
					if(beacon_id%blacklist_size == 0) {
						
						cont_packet_total[0] += nodes_cluster.size();
						for(int i = 0; i < nodes_cluster.size();i++) 
							cont_packet_pernode[0][nodes_cluster[i]]++;
						
						//for(int i = 0; i < blacklist_size; i++) {	
						float N = (float)cont_packet_total[0];
						if((N + cont_dup[0]) > (nodes_cluster.size()*10)) { //10*QTDNODES (pacotes recebidos + duplicações notadas)
							bad_ch = first_ch;
							if(N > 0) {
								ch_quality[bad_ch] = 0.4*((N+cont_dup[0])/N) + 0.6*ch_quality[bad_ch];
							}
							else {
								ch_quality[bad_ch] = 0.4*numberOfAttempts + 0.6*ch_quality[bad_ch];
							}
							//trace() << "CH - Average number of ret beacon " << 0 << " and ch: " << bad_ch << " for : " << N << " packets and " << cont_dup[0] << " dup: " << ch_quality[bad_ch] ;
				
							float v = 1.0/ch_quality[bad_ch];
							
							cont_packet_total[0] = cont_dup[0] = 0;
							
							if(v < beaconThreshold) {
								//trace() << "CH - Bad quality for beacon 0: " << v;
								deep_backward_ch = bad_ch;
							}	
							/*else{
								for(int l = 0; l < nodes_cluster.size(); l++) {
									float N_b = (float)cont_packet_pernode[0][nodes_cluster[l]];
									if(N_b > 10) {
										N_b = (float)(N_b+cont_dup_pernode[0][nodes_cluster[l]])/N_b;
										N_b = 1.0/N_b;
										trace() << "No " << nodes_cluster[l] << " beacon " << 0 << " Nb " << N_b;
										if(N_b < (beaconThreshold/2.0)) {
											trace() << "CH - A qualidade para o no " << nodes_cluster[l] << " e beacon " << 0 << " esta ruim " << N_b;
											deep_backward_ch = first_ch;
											ch_quality[first_ch] = 1;
										}
										cont_dup_pernode[0][nodes_cluster[l]] = cont_packet_pernode[0][nodes_cluster[l]] = 0;
									}		
								}
							}*/
						}
						//}
					}
					
					
					ABMPTreePacket *macBeacon; 
					macBeacon = new ABMPTreePacket("ABMPTree beacon", MAC_LAYER_PACKET);
					macBeacon->setSource(SELF_MAC_ADDRESS);
					macBeacon->setDestination(BROADCAST_MAC_ADDRESS);
					macBeacon->setSequenceNumber(beacon_id);
					macBeacon->setKindDSME(BEACON_PACKET);
					macBeacon->setBlacklist(blacklist);
					macBeacon->setFirstch(first_ch);
			
					macBeacon->setChangebl(start_change);
				    /*if(start_change) {
						trace() << "enviando o antigo...";
						macBeacon->setBlacklist(old_blacklist);
						macBeacon->setFirstch(old_first_ch);
					}*/
		
					if(beacon_id%blacklist_size == 0) {
						for(int i = 0; i < nodes_cluster.size(); i++) {
							if(switch_channel[nodes_cluster[i]]) {
								int change_aux = channel[nodes_cluster[i]];
								while(true) {
									change_aux = (change_aux+1)%16;
									if(channels_adapt[change_aux]) {
										break;
									}
								}
								channel[nodes_cluster[i]] = change_aux;
								trace() << "Change channel of " << nodes_cluster[i] << ": " << channel[nodes_cluster[i]];
								switch_channel[nodes_cluster[i]] = false;	
								window[nodes_cluster[i]] = 0;
								new_value[nodes_cluster[i]] = false;
							}
						}
					}
					
		
					macBeacon->setChannel(channel);	
					macBeacon->setBeaconId(beacon_id);
					macBeacon->setAckBitmap(ack_bitmap);
			
					for(int i = 0; i < nodes_cluster.size(); i++) {
						cont_interval[nodes_cluster[i]]++;
						if(cont_interval[nodes_cluster[i]] >= 40) { //TODO: tornar parametrizavel (vou colocar duas vezes o periodo da APP -> 40 para 1 packet/s)
							trace() << "Change channel of " << nodes_cluster[i] << ": " << channel[nodes_cluster[i]] << " due to deep fading";
							switch_channel[nodes_cluster[i]] = true;
							cont_interval[nodes_cluster[i]] = 0;
						}
					}
			
					trace() << "Transmitting beacon  - Cluster Head " << beacon_id;
					SF_offset = beacon_id;
				
					toRadioLayer(macBeacon);
					toRadioLayer(createRadioCommand(SET_STATE, TX));											
					//ack_bitmap = 0;
					//TODO: alocacao dinamica
					for(int i = 0; i < 100; i++) {
						ack_bitmap[i] = 0;
					}
			
				}
				else if(buffer_ret.size() > 0 && slots[cfp_cont] && !waiting_beacon) { //retransmitt the packet 
					packet_ret *pr = buffer_ret.front();
					buffer_ret.pop_front();
	
					if(pr->cont < this->numberOfAttempts) {
						packet_ret *pr2 = new packet_ret;
						pr2->cont = pr->cont+1;
						pr->slot = cfp_cont;
			
						pr2->packet = pr->packet->dup();
						pr2->packet->setSource(pr->packet->getSource());
						pr2->packet->setDestination(pr->packet->getDestination());
						pr2->packet->setAckReq(pr->packet->getAckReq());
						pr2->packet->setKindDSME(DATA_PACKET);
						buffer_ret.push_back(pr2);
						waiting_beacon = true;
						//trace () << "Listening to beacon";
					//	trace() << "Colocando pacote para retransmissão depois da ret...";						
					}	
					pr->packet->setBeaconDup(deep_backward_ch);
					if(deep_backward_ch != -1) {
						deep_backward_ch = -1;
					}
					
					trace() << "Retransmitting packet " << pr->packet->getSequenceNumber() << " from: " << pr->packet->getSource() << " slot: " << cfp_cont << " on channel: " << channel[pr->packet->getSource()];
					toRadioLayer(pr->packet);
					toRadioLayer(createRadioCommand(SET_STATE, TX));	
				}
				else if(slots[cfp_cont] && buffer_ch.size() > 0) { //transmitt the aggregated packet
					ABMPTreePacket *macFrame = buffer_ch.front();//new ABMPTreePacket("DSME packet", MAC_LAYER_PACKET);
					//packet *p = buffer.front();
				//	cPacket *pkt = p->pkt;
				//	int destination = p->dest;
					buffer_ch.pop_front();
				//	encapsulatePacket(macFrame, pkt);
				//	macFrame->setSource(SELF_MAC_ADDRESS);
				//	macFrame->setDestination(destination);
				//	macFrame->setAckReq(p->ack);
				//	macFrame->setKindDSME(DATA_PACKET);
				
					macFrame->setB0error(b0_error);
					b0_error = 0;
				
					if(macFrame->getAckReq() && this->numberOfAttempts > 0) {
						packet_ret *pr = new packet_ret;
						pr->packet = macFrame->dup();
						pr->packet->setSource(macFrame->getSource());
						pr->packet->setDestination(macFrame->getDestination());
						pr->packet->setAckReq(macFrame->getAckReq());
						pr->packet->setKindDSME(DATA_PACKET);
						pr->cont = 1;
						pr->slot = cfp_cont;
						buffer_ret.push_back(pr);
						waiting_beacon = true;
						//trace () << "Listening to beacon";
						/*if(buffer_ret.size() > macBufferSize()) {
							buffer_ret.pop_front();
							
						}*/
					//	trace() << "Colocando pacote para retransmissão...";
					}
					macFrame->setBeaconDup(deep_backward_ch);
					if(deep_backward_ch != -1) {
						deep_backward_ch = -1;
					}
					trace() << "Transmitting packet " << macFrame->getSequenceNumber() << " from: " << macFrame->getSource() << "slot: " << cfp_cont << " on channel: " << channel[SELF_MAC_ADDRESS];
				
					toRadioLayer(macFrame);
					toRadioLayer(createRadioCommand(SET_STATE, TX));
				}
			}
			if(treeTopology && !(isCoordinator || isClusterHead) && cfp_cont < (slotframesize-2)) { //31 -> two superframes (extract the 8 from the CAP)
				//trace() << "Indo para o update channel, final do slot - end node " << cfp_cont;
				setTimer(UPDATE_CHANNEL_CFP, ((slotSize-1)/0.1));
			}
			else if(cfp_cont < (slotframesize-1)) { //31 -> two superframes (extract the 8 from the CAP)
				//trace() << "Indo para o update channel, final do slot " << cfp_cont;
				setTimer(UPDATE_CHANNEL_CFP, ((slotSize-1)/0.1));
			}
			else if(!isCoordinator) {
				//trace() << "Indo para o endfrae, final do slot " << cfp_cont;
				setTimer(END_FRAME, ((slotSize-2)/0.1));
			}
			/*if((isCoordinator || isClusterHead) && cfp_cont < (slotframesize-1)) { //31 -> two superframes (extract the 8 from the CAP)
				//trace() << "Indo para o update channel, final do slot " << cfp_cont;
				setTimer(UPDATE_CHANNEL_CFP, ((slotSize-1)/0.1));
			}
			else if(!(isCoordinator || isClusterHead) && cfp_cont < (slotframesize-2)) { //31 -> two superframes (extract the 8 from the CAP)
				//trace() << "Indo para o update channel, final do slot - end node " << cfp_cont;
				setTimer(UPDATE_CHANNEL_CFP, ((slotSize-1)/0.1));
			}
			else if(!isCoordinator) {
				//trace() << "Indo para o endfrae, final do slot " << cfp_cont;
				setTimer(END_FRAME, ((slotSize-2)/0.1));
			}*/
			break;
		}
		case END_FRAME: {	
			beacon_id++;
			if(beacon_id%blacklist_size == 0) {
				waiting_beacon = true;
				trace () << "Listening to beacon";
				beaconReceived = false;
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
				
				int channel_ch = (beacon_id+cluster_id)%blacklist_size;
				int indch = first_ch;
				for(int i = 0; i < channel_ch; i++) {
					indch = (indch+1)%16;
					while(!((blacklist >> indch)&1))
						indch = (indch+1)%16;
				}
	
				double new_channel = 2405.0 + 5*indch;
				
				radioCmd->setParameter(new_channel);
		 		send(radioCmd, "toRadioModule");	
				//trace() << "Going to first beacon, channel " << new_channel;	
				if(start_change) {
					//usar a nova configuração
					start_change = false;
				}
			}
			else if(waiting_beacon){
				beaconReceived = false;
				RadioControlCommand *radioCmd = new RadioControlCommand();
				radioCmd->setKind(RADIO_CONTROL_COMMAND);
				radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
				
			/*	int ch_beacon = beacon_id%blacklist_size;
		
				int indch = first_ch;
				for(int i = 0; i < ch_beacon; i++) {
					indch = (indch+1)%16;
					while(!((blacklist >> indch)&1))
						indch = (indch+1)%16;
				}
		
				double new_channel = 2405.0 + 5*indch;
		
				channel[0] = indch; //TODO: customize beacon ID -> here is set to 0
				
				radioCmd->setParameter(new_channel);*/
				if(start_change) {	
					int ch_beacon = (beacon_id+cluster_id)%blacklist_size;
					int indch = old_first_ch;
					for(int i = 0; i < ch_beacon; i++) {
						indch = (indch+1)%16;
						while(!((old_blacklist >> indch)&1)){
							indch = (indch+1)%16;
						}
							
					}
					double new_channel = 2405.0 + 5*indch;
					channel[cluster_id] = indch; //TODO: customize beacon ID -> here is set to 0
					radioCmd->setParameter(new_channel);
		
				}
				else{
					int ch_beacon = (beacon_id+cluster_id)%blacklist_size;	
					int indch = first_ch;
					for(int i = 0; i < ch_beacon; i++) {
						indch = (indch+1)%16;
						while(!((blacklist >> indch)&1))
							indch = (indch+1)%16;
					}
					double new_channel = 2405.0 + 5*indch;
					channel[cluster_id] = indch; //TODO: customize beacon ID -> here is set to 0
					radioCmd->setParameter(new_channel);
				}
		 		send(radioCmd, "toRadioModule");	
				//trace() << "Going to receive the ack through the beacon, channel " << channel[cluster_id];			
				
				/*if(isClusterHead){
					setTimer(UPDATE_CHANNEL_CFP, ((slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				else {
					setTimer(UPDATE_CHANNEL_CFP, ((2*slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((2*slotSize+1) + slotframesize*slotSize)/0.1));	
				}*/
				if(treeTopology && !isClusterHead) {
					setTimer(UPDATE_CHANNEL_CFP, ((2*slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((2*slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				else {
					setTimer(UPDATE_CHANNEL_CFP, ((slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				
				//waiting_beacon = false;
				trace () << "Listening to beacon";
				beaconReceived = true;
				cfp_cont = 0;
				/*if(isClusterHead){
					setTimer(UPDATE_CHANNEL_CFP, ((slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				else {
					setTimer(UPDATE_CHANNEL_CFP, ((2*slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((2*slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				cfp_cont = 0;*/
				
			}
			else {
				//trace() << "Skip beacon " << beacon_id;
				
				if(treeTopology && !isClusterHead) {
					setTimer(UPDATE_CHANNEL_CFP, ((2*slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((2*slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				else {
					setTimer(UPDATE_CHANNEL_CFP, ((slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				
				/*if(isClusterHead){
					setTimer(UPDATE_CHANNEL_CFP, ((slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((slotSize+1) + slotframesize*slotSize)/0.1));	
				}
				else {
					setTimer(UPDATE_CHANNEL_CFP, ((2*slotSize+1)/0.1));
					setTimer(BEACON_TIMEOUT, ( ((2*slotSize+1) + slotframesize*slotSize)/0.1));	
				}*/
				
				waiting_beacon = false;
				beaconReceived = true;
				cfp_cont = 0;
			}
			break;
		}			
	}
}