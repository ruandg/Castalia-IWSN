#ifndef _SimpleTSCHTree_H_
#define _SimpleTSCHTree_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "SimpleTSCHTreePacket.h"
#include <deque>

using namespace std;

#define SWITCH_CHANNEL 223
#define SEND_ACK 224
#define WC_UP 156
#define WC_UPD 157
#define END_FRAME 158

#define MAXNODES 100

typedef struct {
	cPacket *pkt;
	int dest;
	bool ack;
} packet;

typedef struct{
	SimpleTSCHTreePacket *packet;
	int cont;
} packet_ret;

class SimpleTSCHTree: public VirtualMac
{
	private:
		deque<packet*> buffer;
		double slotSize;
		int slotFrameSize;
		string nodeIdSlot;
		bool *slots;
		int slotCont;
		unsigned long long ASN;
		int numberOfAttempts;
		deque<packet_ret*> buffer_ret;
		bool isCoordinator;
		
		SimpleTSCHTreePacket *macFrameAck; 
		
		//TSCHTree
		bool isClusterHead;
		int cluster_id;
		bool slots_rx[MAXNODES];
		long long id_ant[MAXNODES];
		deque<SimpleTSCHTreePacket *> buffer_ch;
		int idSlots[100];
		
	protected:
		void startup();
		void fromRadioLayer(cPacket *, double, double);
		void fromNetworkLayer(cPacket *, int, bool);
		void fromNetworkLayer(cPacket *, int);
		void timerFiredCallback(int);
};

#endif
