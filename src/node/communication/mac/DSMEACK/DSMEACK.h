#ifndef _DSMEACK_H_
#define _DSMEACK_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "DSMEACKPacket.h"
#include <deque>

using namespace std;

#define SWITCH_CHANNEL 150
#define SEND_ACK 151
#define SEND_BEACON 152
#define BEGIN_CFP 153
#define TX_ON 154
#define BEACON_TIMEOUT 155
#define WC_UP 156
#define WC_UPD 157
#define SEND_ACK 224

#define WINDOW 10
#define SLOT_1 0
#define SLOT_2 1
#define ALPHA_PRR 0.3

#define QTDNODES 17
#define ESTWINDOW 10


typedef struct {
	cPacket *pkt;
	int dest;
	bool ack;
} packet;

typedef struct{
	DSMEACKPacket *packet;
	int cont;
} packet_ret;

typedef struct {
	long long id;
	int slot;
} packet_information;

class DSMEACK: public VirtualMac
{
	private:
		deque<packet*> buffer;
		deque<packet_ret*> buffer_ret;
		
		int superFrameOrder;
		int multiSuperFrameOrder;
		int beaconOrder;
		bool isCoordinator;
		double slotSize;
		string allocatedSlots;
		bool slots[32];
		int numberOfAttempts;
		int beacon_cont;
		int cfp_cont;
		int cfp_cont_relative;
		int SDIndex;
		int channelDiversity;
		string nodeIdSlot;
		int idSlots[32];
		int SF_offset;
		string gackSlots;
		int ack_bitmap;
		bool ackSlots[32];
		int firstAck;
		bool channelbm_recv;
		
		int channel[QTDNODES];
		int channel_bitmap;
		int ch_beacon;
		
		double opt_flqe[QTDNODES];
		double PRR[QTDNODES];
		bool slot_attempt;
		long long id_ant[QTDNODES];
		packet_information packet_list[QTDNODES][WINDOW];
		
		int fail_cont[QTDNODES];
		int window[QTDNODES];
		double prrThreshold;
		
		int macBufferSize;
		
		int cont_interval[QTDNODES];
		int cont_interval_tx;
		
		//Lq
		int contann[QTDNODES] = {0};
		int node_addr;
		double rssi_values[ESTWINDOW][QTDNODES];
		double rssi_values_avg[ESTWINDOW][QTDNODES];
		double rssi_median[3][QTDNODES];
		double avg_dup[QTDNODES];
		double d_v[QTDNODES];
		double avg_s[QTDNODES] = {0};
		double prr_d[QTDNODES] = {1,1},prr_u[QTDNODES] = {1,1};
		int ind;
		double somat;
		bool startedEstimation = false;
		
		DSMEACKPacket *macFrameAckDSME; 
		
	protected:
		void startup();
		void fromRadioLayer(cPacket *, double, double);
		void fromNetworkLayer(cPacket *, int, bool);
		void fromNetworkLayer(cPacket *, int);
		void timerFiredCallback(int);
		
		//bool calculatePRR(int addr, int sequenceNumber);
		bool calculatePRR(int addr, packet_information packet_list[QTDNODES][WINDOW], long long first);
		
		void medianValue(uint8_t node_addr);
		
		void mediana(uint8_t node_addr);

		void calculatePRRu(uint8_t node_addr);

		void calculatePRRd(uint8_t node_addr);
		
		//adherence functions
		double mSPRR(double SPRR);

		double mSF(double SF);

		double mASL(double ASL);

		double mSNR(double SNR);

		double mSRNP(double SRNP);

		//fuzzy functions
		double mean(double sprr, double sf, double asl, double snr);
		
		double min2(double a, double b);

		double min(double sprr, double sf, double asl, double snr);

		double prr_variance(double *prr, int n);
 
};

#endif
