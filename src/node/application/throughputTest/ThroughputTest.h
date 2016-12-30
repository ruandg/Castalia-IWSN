/****************************************************************************
 *  Copyright: National ICT Australia,  2007 - 2010                         *
 *  Developed at the ATP lab, Networked Systems research theme              *
 *  Author(s): Athanassios Boulis, Yuriy Tselishchev                        *
 *  This file is distributed under the terms in the attached LICENSE file.  *
 *  If you do not find this file, copies can be found by writing to:        *
 *                                                                          *
 *      NICTA, Locked Bag 9013, Alexandria, NSW 1435, Australia             *
 *      Attention:  License Inquiry.                                        *
 *                                                                          *
 ****************************************************************************/

#ifndef _THROUGHPUTTEST_H_
#define _THROUGHPUTTEST_H_

#include "VirtualApplication.h"

using namespace std;

#define WINDOW 5
#define alpha 0.9
#define ATTEMPTS 0
#define QTDNODES 10
#define ESTWINDOW 20


enum ThroughputTestTimers {
	SEND_PACKET = 1
};

class ThroughputTest: public VirtualApplication {
 private:
	double packet_rate;
	string recipientAddress;
	double startupDelay;

	float packet_spacing;
	int dataSN;
	
	double FBIT = 0;
	double estETXd = 0;
	double estETXu = 0;
	double PRR = 0;
	int packets_sent_ext = 0;
	int ext_cont = 0;
	int acks_recv_ext = 0;
	double RNP = 1;

	//end node
	int cont_tx = 0;
	int attempts = 0;
	bool try_again = false;
	int acks_received = 0;
	int packets_sent = 0;

	int beacon_count = 0;
	int beacon_rx_count_end = 0;
	int first_beacon_end = 0;
	int cont_prr_beacon = 0;
	double prr_beacon_end = 0;
	int id_packets[5];
	int ini_4b=0, end_4b=0;
	double WMEWMA=1;
	double alpha_4b = 0.9;
	double estRNP_back = 1;
	double estRNP_fowr = 1;
	double four_bit = 1;
	int cont_tx_end = 0;

	//sink node
	int last_packet_recv;
	int ack_seq =0;
	int packets_received = 0;
	int acks_sent = 0;

	unsigned long long cont_tx_sink = 0;
	int beacon_count_sink = 0;
	int beacon_rx_count_sink = 0;
	int first_beacon_sink = 0;
	double sprr_beacon_sink = 1;
	double prr_beacon_sink = 0;
	double ASL_flqe = 0;
	double prr_back_flqe=0;
	double SRNP_flqe = 1;
	double SNR = 0;
	double FLQE = 1;
	bool new_value_flqe[5];
	bool prr_back = false;
	double prr_var[30];
	int end_var=0, cont_var = 0;
	double SF = 0;
	double OptFLQE = 1;
	double alpha_flqe = 0.6;
	double ETX = 0;


 protected:
	void startup();
	void fromNetworkLayer(DataPacket *, const char *, double, double);
	void handleRadioControlMessage(RadioControlMessage *);
	void timerFiredCallback(int);
	void sendAck(int);
	
};

#endif				// _THROUGHPUTTEST_APPLICATIONMODULE_H_
