#ifndef _ABMP_H_
#define _ABMP_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "ABMPPacket.h"
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
#define LQ_EST 158
#define BEACON_TIMEOUT_RESTART 159
#define END_FRAME 160

#define WINDOW 10
#define SLOT_1 0
#define SLOT_2 1
#define ALPHA_PRR 0.3

#define ESTWINDOW 10

#define MAX_RESTART 16


typedef struct {
	cPacket *pkt;
	int dest;
	bool ack;
} packet;

typedef struct{
	ABMPPacket *packet;
	int cont;
} packet_ret;

typedef struct {
	long long id;
	int slot;
} packet_information;

class ABMP: public VirtualMac
{
	private:
		deque<packet*> buffer;
		deque<packet_ret*> buffer_ret;
		
		bool isCoordinator;
		double slotSize;
		string allocatedSlots;
		bool slots[100];
		int numberOfAttempts;
		int beacon_cont;
		int cfp_cont;
		int channelDiversity;
		string nodeIdSlot;
		int idSlots[100];
		int SF_offset;
		int ack_bitmap;
		
		int channel[QTDNODES];
		int channel_bitmap;
		int ch_beacon;
		
		int beacon_id;
		int blacklist;
		int blacklist_aux[8];
		int map_chtobeacon[16];
		
		int first_ch;
		float ch_quality[16];
		int cont_dup[16];
		int cont_dup_pernode[16][QTDNODES];
		int cont_packet_pernode[16][QTDNODES];
		int blacklist_size;
		bool waiting_beacon;
		bool waiting_b0;
		bool beaconReceived;
		int cont_restart[QTDNODES];
		bool switch_channel[QTDNODES];
		int b0_error;
		int cont_packet_total[16];
		double beaconThreshold;
		bool start_change;
		int bad_ch;
		int old_blacklist;
		int old_first_ch;
		
		long long id_ant[QTDNODES];
		packet_information packet_list[QTDNODES][WINDOW];
		
		int fail_cont[QTDNODES];
		int window[QTDNODES];
		double prrThreshold;
		bool new_value = false;
		
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
		int last_id = -1;
		bool startedEstimation = false;
				
	protected:
		void startup();
		void fromRadioLayer(cPacket *, double, double);
		void fromNetworkLayer(cPacket *, int, bool);
		void fromNetworkLayer(cPacket *, int);
		void timerFiredCallback(int);
				
		void medianValue(uint8_t node_addr);
		
		void mediana(uint8_t node_addr);

		void calculatePRRu(uint8_t node_addr);

		void calculatePRRd(uint8_t node_addr);
 
};

#endif
