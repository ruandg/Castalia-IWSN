/*
 * @author Ugo Colesanti
 * @author Silvia Santini
 * @version 1.0 (January 27, 2011)
 *
 * Acknowledgment: This code is based upon the implementation of CC2420Transmit TinyOS component
 * written by Jonathan Hui, David Moss, and Jung Il Choi.
 */

/*
 * Copyright (c) 2011 Sapienza University of Rome.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Sapienza University of Rome nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SAPIENZA
 * UNIVERSITY OF ROME OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 2011 ETH Zurich.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of ETH Zurich nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ETH
 * ZURICH OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 2005-2006 Arch Rock Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the Arch Rock Corporation nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * ARCHED ROCK OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE
 */

#ifndef _CC2420MAC_H_
#define _CC2420MAC_H_

#include "VirtualMac.h"
#include "CC2420Packet_m.h"
#include "TosEnvironment.h"
#include <queue>

using namespace std;

//////////////////////////// CC2420TransmitP.nc /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
	S_STOPPED = 1,
	S_STARTED = 2,
	S_LOAD = 3,
	S_SAMPLE_CCA = 4,
	S_BEGIN_TRANSMIT = 5,
	S_SFD = 6,
	S_EFD = 7,
	S_ACK_WAIT = 8,
	S_CANCEL = 9,
} cc2420_transmit_state_t;

// This specifies how many jiffies the stack should wait after a
// TXACTIVE to receive an SFD interrupt before assuming something is
// wrong and aborting the send. There seems to be a condition
// on the micaZ where the SFD interrupt is never handled.
enum {
	CC2420_ABORT_PERIOD = 320
};

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Custom Values //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

enum TMacTimers {
	BACKOFF = 1,
	TXFIFO_WRITE = 2,
	ACK_SENT = 3,
	CAPTURE_SFD = 4,
	CAPTURE_EFD = 5,
	SPI_RX_GRANTED = 6,
	SPI_TX_GRANTED = 7,
};


enum CC2420{
	CC2420_TIME_ACK_TURNAROUND = 7,
	CC2420_TIME_SYMBOL = 2,         // 2 symbols / jiffy
	CC2420_BACKOFF_PERIOD = ( 20 / CC2420_TIME_SYMBOL ), // symbols
	CC2420_MIN_BACKOFF = ( 20 / CC2420_TIME_SYMBOL ),  // platform specific?
	CC2420_ACK_WAIT_DELAY = 256,    // jiffies

	DEFAULT_FCS = 0 ,
	DEFAULT_FCF = 0 ,
	DEFAULT_TOS_AM_GROUP = 0x22 ,
	DEFAULT_AM_TYPE = 0x3F ,

};

enum ieee154_fcf_enums {
  IEEE154_FCF_FRAME_TYPE = 0,
  IEEE154_FCF_SECURITY_ENABLED = 3,
  IEEE154_FCF_FRAME_PENDING = 4,
  IEEE154_FCF_ACK_REQ = 5,
  IEEE154_FCF_INTRAPAN = 6,
  IEEE154_FCF_DEST_ADDR_MODE = 10,
  IEEE154_FCF_SRC_ADDR_MODE = 14,
};

enum ieee154_fcf_type_enums {
  IEEE154_TYPE_BEACON = 0,
  IEEE154_TYPE_DATA = 1,
  IEEE154_TYPE_ACK = 2,
  IEEE154_TYPE_MAC_CMD = 3,
};


// SpiResource implementation
typedef enum{
	SPI_NULL ,
	SPI_TX ,
	SPI_RX ,
} spi_request_t;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class CC2420Mac: public VirtualMac {

	//////////////////////////// CC2420TransmitP.nc variables ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	CC2420Packet*  m_msg;

	bool m_cca;

	uint8_t m_tx_power;

	cc2420_transmit_state_t m_state ;//= S_STOPPED;

	bool m_receiving ; //= FALSE;

	uint16_t m_prev_time;

	/** Byte reception/transmission indicator */
	bool sfdHigh;

	/** Let the CC2420 driver keep a lock on the SPI while waiting for an ack */
	bool abortSpiRelease;

	/** Total CCA checks that showed no activity before the NoAck LPL send */
	int8_t totalCcaChecks;

	/** The initial backoff period */
	uint16_t myInitialBackoff;

	/** The congestion backoff period */
	uint16_t myCongestionBackoff;

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////// Custom variables /////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	// Data sequence number for IEEE15.4 packets
	uint8_t dsn ;
	bool spiResourceRequest ;  // emulates the spi resource
	bool ackPending ; // custom variable to manage CC2420Receive.receive and CC2420Transmit.receive at the same time.
	simtime_t txFifoWriteTimeout ;


	// omnetpp config
	bool ackEnabled ; // enable auto acknowledgement on unicast packets
	double datarate ; // trick: must be the same datarate value than the radio module one.
	int PhyFrameOverhead ; // trick: Physical frame overhead must be the same than the radio module one.
	int MACAckOverhead ; // Mac overhead (default 5 bytes )


	// SpiResource implementation
	queue<spi_request_t> spiRequests;
	spi_request_t spiResource ;
	bool spiRelease ;

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////// Castalia functions /////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	void startup();
	virtual ~CC2420Mac() ;
	void finishSpecific();

	void timerFiredCallback(int timer) ;
	int handleControlCommand(cMessage * msg) ;

	virtual void fromNetworkLayer(cPacket *, int);
	virtual void fromRadioLayer(cPacket *, double, double);

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////// CC2420TransmitP.nc functions ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	error_t command_StdControl_start() ;
	error_t command_StdControl_stop() ;
	error_t command_Send_send( CC2420Packet* p_msg, bool useCca ) ;
	error_t command_Send_resend(bool useCca ) ;
	error_t command_Send_cancel() ;
	void signal_RadioBackoff_requestInitialBackoff() ;
	void signal_RadioBackoff_requestCongestionBackoff() ;
	void event_CaptureSFD_captured() ;
	void event_ChipSpiResource_releasing() ;
	void event_CC2420Receive_receive(cPacket * msg) ;
	void event_SpiResource_granted() ;
	void event_TXFIFO_writeDone( /*uint8_t* tx_buf, uint8_t tx_len, uint8_t error */) ;
	void event_BackoffTimer_fired() ;
	error_t send( CC2420Packet* p_msg, bool cca );
	error_t resend( bool cca );
	void attemptSend();
	void congestionBackoff();
	error_t acquireSpiResource();
	error_t releaseSpiResource();
	void loadTXFIFO();
	void signalDone( error_t err );

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////// Custom functions /////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	void command_BackoffTimer_start(uint32_t t) ;
	void command_BackoffTimer_stop() ;
	void signal_Send_sendDone(error_t error) ;
	void return_Send_send(error_t err) ;

	// SpiResource implementation -------------------------------------
	error_t command_SpiResource_request(spi_request_t sr);
	error_t command_SpiResource_immediateRequest(spi_request_t sr);
	error_t command_SpiResource_release(spi_request_t sr);
	error_t command_ChipSpiResource_attemptRelease() ;
	void command_ChipSpiResource_abortRelease();
	error_t attemptRelease();
	void signal_ChipSpiResource_releasing();
	void signal_SpiResource_granted(spi_request_t sr);
	// ----------------------------------------------------------------

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////



};
#endif
