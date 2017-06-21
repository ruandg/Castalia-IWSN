#ifndef _SimpleDSME_H_
#define _SimpleDSME_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "SimpleDSMEPacket.h"
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

#define MAX_NODES 10
#define WINDOW 10
#define SLOT_1 0
#define SLOT_2 1
#define ALPHA_PRR 0.3


typedef struct {
	cPacket *pkt;
	int dest;
	bool ack;
} packet;

typedef struct{
	SimpleDSMEPacket *packet;
	int cont;
} packet_ret;

typedef struct {
	long long id;
	int slot;
} packet_information;

class SimpleDSME: public VirtualMac
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
		
		int channel[MAX_NODES];
		int channel_bitmap;
		int ch_beacon;
		
		double opt_flqe[MAX_NODES];
		double PRR[MAX_NODES];
		bool slot_attempt;
		long long id_ant[MAX_NODES];
		packet_information packet_list[MAX_NODES][WINDOW];
		
		int fail_cont[MAX_NODES];
		int window[MAX_NODES];
		double prrThreshold;
		
		int macBufferSize;
		
		int cont_interval[MAX_NODES];
		int cont_interval_tx;
		
		
	protected:
		void startup();
		void fromRadioLayer(cPacket *, double, double);
		void fromNetworkLayer(cPacket *, int, bool);
		void fromNetworkLayer(cPacket *, int);
		void timerFiredCallback(int);
		
		//bool calculatePRR(int addr, int sequenceNumber);
		bool calculatePRR(int addr, packet_information packet_list[MAX_NODES][WINDOW], long long first);
		
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
