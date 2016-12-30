#ifndef _SimpleTSCH_H_
#define _SimpleTSCH_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "SimpleTSCHPacket.h"
#include <deque>

using namespace std;

#define SWITCH_CHANNEL 223
#define SEND_ACK 224
#define WC_UP 156
#define WC_UPD 157

typedef struct {
	cPacket *pkt;
	int dest;
	bool ack;
} packet;

typedef struct{
	SimpleTSCHPacket *packet;
	int cont;
} packet_ret;

class SimpleTSCH: public VirtualMac
{
	private:
		deque<packet*> buffer;
		double slotSize;
		int superFrameSize;
		string allocatedSlots;
		bool *slots;
		int slotCont;
		unsigned long long ASN;
		int numberOfAttempts;
		deque<packet_ret*> buffer_ret;
		bool isCoordinator;
		
		SimpleTSCHPacket *macFrameAck; 
		
	protected:
		void startup();
		void fromRadioLayer(cPacket *, double, double);
		void fromNetworkLayer(cPacket *, int, bool);
		void fromNetworkLayer(cPacket *, int);
		void timerFiredCallback(int);
};

#endif
