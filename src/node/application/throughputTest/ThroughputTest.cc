/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2011                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#include "ThroughputTest.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Define_Module(ThroughputTest);

//adherence functions
double mSPRR(double SPRR) {
	if(SPRR <= 0.25) return 0;
	else if(SPRR >= 0.95) return 1;
	else return (4.0*SPRR-1)/3.0;
}

double mSF(double SF) {
	if(SF >= 0.7) return 0;
	else return (-10.0*SF+7)/7.0;
}

double mASL(double ASL) {
	if (ASL >= 0.5) return 0;
	else if(ASL <= 0.01) return 1;
	else return (-100.0*ASL+50)/49.0;
}

double mSNR(double SNR) {
	if(SNR <= 1) return 0;
	else if(SNR >=8) return 1;
	else return (SNR-1)/7.0;
}

double mSRNP(double SRNP) {
	if(SRNP >= 4) return 0;
	else if(SRNP <= 1) return 1;
	else return (4-SRNP)/3.0;
}

//fuzzy functions
double mean(double sprr, double sf, double asl, double snr) {
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

double min2(double a, double b) {
	if(a == -1 && b != -1) return b;
	else if(b == -1 && a!= -1) return a;
	else if(a == -1 && b == -1) return 1e9;
	else if(a <= b) return a;
	else return b;
}

double min(double sprr, double sf, double asl, double snr) {
	return min2(sprr,min2(sf,min2(asl,snr)));
}

double prr_variance(double *prr, int n) {
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

void ThroughputTest::startup()
{
//	srand(time(NULL));
	packet_rate = par("packet_rate");
	recipientAddress = par("nextRecipient").stringValue();
	startupDelay = par("startupDelay");
		
	//setTimer(SWITCH_CHANNEL, slot_len); //switch the channel every 10ms
	double startupDelay_rand = 0;//rand()%10;
	
	packet_spacing = packet_rate > 0 ? 1.0 / float (packet_rate) : -1;
	dataSN = 1;
	
	int nodeId = abs(recipientAddress.compare(SELF_NETWORK_ADDRESS));
	
	if (packet_spacing > 0) {
		setTimer(SEND_PACKET, packet_spacing + startupDelay_rand + 1.5);
	}
	else
		// Diego
		trace() << "Not sending packets";

	declareOutput("Packets received per node");
	for(int i = 0; i < 5; i++)new_value_flqe[i]=false;
}

void ThroughputTest::fromNetworkLayer(DataPacket * rcvPacket,
		const char *source, double rssi, double lqi)
{
	int sequenceNumber = rcvPacket->getSequenceNumber();
	double* data = rcvPacket->getDataPacket();

	//SINK NODE
	string addr(SELF_NETWORK_ADDRESS,1);
	if (addr.compare("0") == 0) {
		//data packet received
		if(data[0] == 0) {
			trace() << "Received packet " << sequenceNumber << " from node " << source << ": " << rssi << " " << lqi << " fromNetworkLayer";
			packets_received++;
			
			prr_back_flqe = data[1];
			prr_back = true;
			SRNP_flqe = data[2];
			new_value_flqe[4] = true;
			SNR = rssi + 100.0; //noise floor equal to -100
			new_value_flqe[3] = true;

			//trace() << "Packets received: " << packets_received;
			
			/*//LQ calculation
			if(sequenceNumber == last_id) {
			    avg_dup[node_addr] += 1; //duplicacao de pacote
			}
			last_id = sequenceNumber;
			
		    node_addr = int(source[0] - '0');
			trace() << "Node_addr: " << node_addr;
		    rssi_values[contann[node_addr]][node_addr] = (double)((rssi+94.0)/0.25);
		    contann[node_addr]++;
		    if(contann[node_addr] == ESTWINDOW) {
				avg_s[node_addr] = 0;
			    mediana(node_addr);
			    calculatePRR(node_addr);
			    avg_dup[node_addr] = avg_dup[node_addr]/(ESTWINDOW/10);
			    calculatePRRd(node_addr);
			    contann[node_addr] = 0;
			    avg_dup[node_addr] = 0;
			    trace() << "LQu: " << (prr_u[node_addr]*100.0);
			    trace() << "LQd: " << (prr_d[node_addr]*100.0);
			}*/
		}
		//beacon received
		else {
			trace() << "Received beacon " << sequenceNumber << " from node " << source << ": " << rssi << " " << lqi << " fromNetworkLayer";
			beacon_rx_count_sink++;
			trace() << "Beacons received: " << beacon_rx_count_sink;
			if(beacon_rx_count_sink%5 == 0 && beacon_rx_count_sink != 0) {
				sprr_beacon_sink = sprr_beacon_sink*0.6 + 0.4*(5.0/(sequenceNumber-first_beacon_sink+1));
				prr_beacon_sink = (5.0/(sequenceNumber-first_beacon_sink+1));
				first_beacon_sink = sequenceNumber+1;
				new_value_flqe[0] = true;
				//trace() << "SPRR SINK: " << sprr_beacon_sink;
				prr_var[end_var] = prr_beacon_sink;
				end_var = (end_var+1)%30;
				if(cont_var < 30) cont_var++;
				if(cont_var > 4) {
					SF = prr_variance(prr_var,cont_var);
				//	trace() << "SF: " << SF;
					new_value_flqe[1] = true;
				}
				ASL_flqe = abs(prr_back_flqe - prr_beacon_sink);
				new_value_flqe[2] = prr_back;
				prr_back = false;
				
				/*ETX*/
				if((prr_beacon_sink*prr_back_flqe) > 0) {
					ETX = 1.0/(prr_beacon_sink*prr_back_flqe);
					trace() << "ETX: " << ETX;

				}
				
			}
			//have at least SPRR and SNR
			if(new_value_flqe[0] && new_value_flqe[3]) {
				/*F-LQE*/
				FLQE = alpha_flqe*FLQE + (1-alpha_flqe)*(0.6*min(new_value_flqe[0]?mSPRR(sprr_beacon_sink):-1, new_value_flqe[1]?mSF(SF):-1,
				 new_value_flqe[2]?mASL(ASL_flqe):-1, new_value_flqe[3]?mSNR(SNR):-1) + 
				0.4*mean(new_value_flqe[0]?mSPRR(sprr_beacon_sink):-1, new_value_flqe[1]?mSF(SF):-1, new_value_flqe[2]?mASL(ASL_flqe):-1,
			  	new_value_flqe[3]?mSNR(SNR):-1));
			
				trace() << "F-LQE: " << FLQE;
				
				
				/*Opt-FLQE*/
				OptFLQE = alpha_flqe*OptFLQE + (1-alpha_flqe)*(0.6*min(new_value_flqe[0]?mSPRR(sprr_beacon_sink):-1, new_value_flqe[4]?mSRNP(SRNP_flqe):-1,
				 new_value_flqe[2]?mASL(ASL_flqe):-1, new_value_flqe[3]?mSNR(SNR):-1) + 
				0.4*mean(new_value_flqe[0]?mSPRR(sprr_beacon_sink):-1, new_value_flqe[4]?mSRNP(SRNP_flqe):-1, new_value_flqe[2]?mASL(ASL_flqe):-1,
			  	new_value_flqe[3]?mSNR(SNR):-1));
				
				trace() << "Opt-FLQE: " << OptFLQE;
			
				for(int i = 0; i < 5; i++)new_value_flqe[i]=false;
			}
			
		}
	} 
	//END NODE
	else {
		double idAck = rcvPacket->getDataPacket()[0];
		//beacon
		if(idAck == -1) {
			trace() << "Received beacon " << sequenceNumber << " from node " << source << ": " << rssi << " " << lqi << " fromNetworkLayer";
			beacon_rx_count_end++;
			trace() << "Beacons received: " << beacon_rx_count_end;
			id_packets[end_4b] = sequenceNumber;
			if((ini_4b < end_4b) && ((end_4b-ini_4b) == 4)) {
				WMEWMA = alpha_4b*WMEWMA + (1-alpha_4b)*(5.0/(id_packets[end_4b]-id_packets[ini_4b]+1));
				end_4b = (end_4b+1)%5;
				ini_4b = (ini_4b+1)%5;
				if(WMEWMA > 0) estRNP_back = 1.0/WMEWMA;
			//	trace() << "estRNP_back: " << estRNP_back;
				four_bit = alpha_4b*four_bit + (1-alpha_4b)*estRNP_back;
				trace() << "Four-Bit: " << four_bit;
			}
			else if((end_4b < ini_4b) && ((end_4b+5)-ini_4b == 4)) {
				WMEWMA = alpha_4b*WMEWMA + (1-alpha_4b)*(5.0/(id_packets[end_4b]-id_packets[ini_4b]+1));
				end_4b = (end_4b+1)%5;
				ini_4b = (ini_4b+1)%5;
				if(WMEWMA > 0) estRNP_back = 1.0/WMEWMA;
			//	trace() << "estRNP_back: " << estRNP_back;
				four_bit = alpha_4b*four_bit + (1-alpha_4b)*estRNP_back;
				trace() << "Four-Bit: " << four_bit;
			}
			else {
				end_4b = (end_4b+1)%5;
			}
			if(beacon_rx_count_end%5 == 0 && beacon_rx_count_end != 0) {
				prr_beacon_end = (5.0/(sequenceNumber-first_beacon_end+1));
				first_beacon_end = sequenceNumber+1;
			}
		}
		//ack
		else{
			//trace() << "Received ack " << idAck << ": " << rssi << " " << lqi << " fromNetworkLayer";;
			acks_received++; 		
			//trace() << "Acks received: " << acks_received;
			acks_recv_ext++;
		
			if((int)idAck == dataSN){
				 dataSN++;
				 try_again = false;
				 attempts = 0;
	 			 // << "Recebeu ack que estava esperando...";	
			}
		}
	}
	//free(data);
}

void ThroughputTest::timerFiredCallback(int index)
{
	
	switch (index){
		case SEND_PACKET: {
			//SINK NODE
			string addr(SELF_NETWORK_ADDRESS,1);
			/*
			if (addr.compare("0") == 0) {
				if((cont_tx_sink+50)%(5*(int)(packet_rate)) == 0) {
					trace() << "Sending beacon #" << beacon_count_sink;
			
				    double *data = new double[10];
					data[0] = -1;
					DataPacket* data_packet = createDataPacket(data, beacon_count_sink, 5);
		    
					toNetworkLayer(data_packet, (const char*)("1"));
					
					beacon_count_sink++;
					trace() << "Beacons sent: " << beacon_count_sink;
				}
				setTimer(SEND_PACKET, packet_spacing);
				cont_tx_sink++;
			}*/
			//END NODE
		//	else {
			if (addr.compare("0") != 0) {
				//send data packet
			//	if(cont_tx%((int)(packet_rate)) == 0) {
					switch (index) {
						case SEND_PACKET:{
							trace() << "Sending packet " << dataSN;
							packets_sent++;
							//trace() << "Packets sent: " << packets_sent;							
							/*RNP*/
							/*packets_sent_ext++;
				
							if(packets_sent_ext == WINDOW) {
								if(acks_recv_ext == 0) {
									RNP = 0.6*RNP + 0.4*packets_sent_ext;
									estRNP_fowr = alpha_4b*estRNP_fowr + (double)(1-alpha_4b)*packets_sent_ext;
								}
								else {
									RNP = 0.6*RNP + (double)0.4*packets_sent_ext/acks_recv_ext;
									estRNP_fowr = alpha_4b*estRNP_fowr + (double)(1-alpha_4b)*packets_sent_ext/acks_recv_ext;
								}
								four_bit = alpha_4b*four_bit + (1-alpha_4b)*estRNP_fowr;
								//trace() << "Four-Bit: " << four_bit;
							//	trace() << "SRNP: " << RNP;
								packets_sent_ext = acks_recv_ext = 0;
							}
							/*end RNP
							*/
						    double *data = new double[10];
							data[0] = 0;
							data[1] = prr_beacon_end;
							data[2] = RNP;
							DataPacket* data_packet = createDataPacket(data, dataSN, 80);
				    
							toNetworkLayer(data_packet, (const char*)("0"));
							dataSN++;
							//try_again = true;
							//setTimer(SEND_PACKET, packet_spacing);
							break;
						}
					}
					cont_tx++;
					//}
				//send beacon
				/*if((cont_tx-1+2)%(5*(int)packet_rate) == 0) {
					trace() << "Sending beacon #" << beacon_count;
			
				    double *data = new double[10];
					data[0] = 1;
					DataPacket* data_packet = createDataPacket(data, beacon_count, 5);
		    
					toNetworkLayer(data_packet, (const char*)("0"));

					beacon_count++;
					trace() << "Beacons sent: " << beacon_count;
		   
				}*/
			}
			setTimer(SEND_PACKET, packet_spacing);
			break;
		}
	/*	case SWITCH_CHANNEL: { 
			string addr(SELF_NETWORK_ADDRESS,1);
			cont_ch[(int)(addr[0]-'0')]++;
			double new_channel = 2405.0 + (cont_ch[(int)(addr[0]-'0')]%16)*5;
			//trace() << "Novo canal: " << new_channel;
						
			RadioControlCommand * radioCmd = new RadioControlCommand();
			radioCmd->setKind(RADIO_CONTROL_COMMAND);
			radioCmd->setRadioControlCommandKind(SET_CARRIER_FREQ);
			radioCmd->setParameter(new_channel);
	 		send(radioCmd, "toCommunicationModule");
					
			setTimer(SWITCH_CHANNEL, slot_len);
		 
			break;
		}*/
	}
}

void ThroughputTest::sendAck(int packet_id)
{
	//trace() << "Sending ack for packet " << packet_id;
    double *data = new double[10];
	data[0] = packet_id;
	DataPacket* data_packet = createDataPacket(data, ack_seq, 5);  
	toNetworkLayer(data_packet, (const char*)("1"));
	acks_sent++;
	//trace() << "Acks sent: " << acks_sent;
}


// This method processes a received carrier sense interupt. Used only for demo purposes
// in some simulations. Feel free to comment out the trace command.
void ThroughputTest::handleRadioControlMessage(RadioControlMessage *radioMsg)
{
	switch (radioMsg->getRadioControlMessageKind()) {
		case CARRIER_SENSE_INTERRUPT:
			trace() << "CS Interrupt received! current RSSI value is: " << radioModule->readRSSI();
                        break;
	}
}

