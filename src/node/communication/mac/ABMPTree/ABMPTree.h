#ifndef _ABMPTree_H_
#define _ABMPTree_H_

#include <omnetpp.h>
#include "VirtualMac.h"
#include "ABMPTreePacket.h"
#include <deque>
#include <vector>

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
#define UPDATE_CHANNEL_CFP 161

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
	ABMPTreePacket *packet;
	int cont;
	int slot;
} packet_ret;

typedef struct {
	long long id;
	int slot;
} packet_information;


class ABMPTree: public VirtualMac
{
	private:
		deque<packet*> buffer;
		deque<packet_ret*> buffer_ret;
		
		bool isCoordinator;
		double slotSize;
		string allocatedSlots;
		//TODO: alocação dinamica para slots e idSlots e ack_bitmap
		bool slots[100];
		int numberOfAttempts;
		int beacon_cont;
		int cfp_cont;
		string nodeIdSlot;
		int idSlots[200];
		int SF_offset;
		//long long ack_bitmap;
		int ack_bitmap[100];
		
		int channel[MAXNODES];
		long long channel_bitmap;
		int ch_beacon;
		
		int beacon_id;
		int blacklist;
		int blacklist_aux[8];
		int map_chtobeacon[16];
		
		int first_ch;
		float ch_quality[16];
		int cont_dup[16];
		int cont_dup_pernode[16][MAXNODES];
		int cont_packet_pernode[16][MAXNODES];
		int blacklist_size;
		bool waiting_beacon;
		bool waiting_b0;
		bool beaconReceived;
		int cont_restart[MAXNODES];
		bool switch_channel[MAXNODES];
		int b0_error;
		int cont_packet_total[16];
		double beaconThreshold;
		bool start_change;
		int bad_ch;
		int old_blacklist;
		int old_first_ch;
		
		long long id_ant[MAXNODES];
		
		int fail_cont[MAXNODES];
		int window[MAXNODES];
		double prrThreshold;
		
		int macBufferSize;
		
		int cont_interval[MAXNODES];
		int cont_interval_tx;
		
		//ABMPTree
		int cluster_id;
		int QTDNODES;
		int slotframesize;
		bool isClusterHead;
		deque<ABMPTreePacket *> buffer_ch;
		bool slots_rx[MAXNODES];
		int deep_backward_ch = false;
		//deque<int> deep_backward_ch_list;
		bool deep_beacon0;
		bool channels_adapt[16];
		bool treeTopology;
		
		//Lq
		int contann[MAXNODES] = {0};
		int node_addr;
		double rssi_values[ESTWINDOW][MAXNODES];
		double rssi_values_avg[ESTWINDOW][MAXNODES];
		double rssi_median[3][MAXNODES];
		double avg_dup[MAXNODES];
		double d_v[MAXNODES];
		double avg_s[MAXNODES];
		double prr_d[MAXNODES],prr_u[MAXNODES];
		int ind;
		double somat;
		int last_id = -1;
		bool startedEstimation[MAXNODES];
		bool new_value[MAXNODES];
		vector<int> nodes_cluster;
				
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
