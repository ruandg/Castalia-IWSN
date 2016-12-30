/*
 * @author Ugo Colesanti
 * @author Silvia Santini
 * @version 1.01 (January 3, 2012)
 *
 * Acknowledgment: This code is based upon the implementation of CC2420Transmit TinyOS component
 * written by Jonathan Hui, David Moss, and Jung Il Choi.
 */

/*
 * Copyright (c) 2012 Sapienza University of Rome.
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
 * Copyright (c) 2012 ETH Zurich.
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


#include "CC2420Mac.h"

Define_Module(CC2420Mac);

////////////////////////// Castalia functions /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void CC2420Mac::startup(){
	//////////////////////////// CC2420TransmitP.nc /////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	m_state = S_STOPPED ;
	m_receiving = false ;

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Castalia specific //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	txFifoWriteTimeout = par("txFifoWriteTimeout") ; // time needed by the CC2420 to write the packet on its buffer. Default 0.
	m_cca = par("enableCCA") ;	// enables/disables CCA (in tinyos it's done by the send interface).
	datarate = par("datarate") ; // trick to compute the time needed by the radio to send a packet. It must be equal to the one on the radio module.
	PhyFrameOverhead = par("phyFrameOverhead");	// trick to compute the time needed by the radio to send a packet. It must be equal to the one on the radio module.
	MACAckOverhead = par("macAckOverhead") ; // Size of ACK packets (only mac segment). Default 5 bytes.
	ackEnabled = par("ackEnabled") ; // enables / disables acknowledgement for unicast packets. Default enabled.

	spiResource = SPI_NULL ;

	command_StdControl_start() ; // calls the StdControl.start() command.

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////


}

void CC2420Mac::finishSpecific(){

}

CC2420Mac::~CC2420Mac(){
	delete m_msg ;
	m_msg = NULL ;
}

/*
 * In the CC2420Transmit module the timer is a 32Khz alarm thus, use the utility function
 * tos32KhzToSeconds() to convert the value in a Castalia-compatible time.
 */
void CC2420Mac::timerFiredCallback(int timer)
{
	trace() << "TimerFiredCallback, value: "<<timer;
	switch (timer) {

		case BACKOFF:{
			event_BackoffTimer_fired() ;
			break;
		}

		case TXFIFO_WRITE:{
			event_TXFIFO_writeDone() ;
			break;
		}

		case ACK_SENT:{
			command_SpiResource_release(SPI_RX) ;
			break ;
		}

		case CAPTURE_SFD:case CAPTURE_EFD:{
			event_CaptureSFD_captured() ;
			break;
		}

		case SPI_TX_GRANTED:{
			event_SpiResource_granted() ;
			break;
		}

		case SPI_RX_GRANTED:{
			opp_error("Should not happen") ;
			break ;
		}

		default:{
			opp_error("Unexpected message!");
		}
	}
}

/*
 * Control commands are used to call all the functions except Send.send .
 * The control packet has the following fields:
 * 	- mcmType: EVENT | COMMAND | COMMAND_RETURN
 * 	- mcmInterface: SEND
 *  - mcmCommand: CMD_RESEND | CMD_SEND | CMD_CANCEL
 *  - mcmEvent: EVT_SENDDONE
 *
 *  As an example, the command "call Send.cancel()" is replace with the following packet:
 *  mcmType: COMMAND , mcmInterface: SEND , mcmCommand: CMD_CANCEL , mcmEvent: n.d.
 *
 *  Note: The mcmType COMMAND_RETURN represents the return value when calling a command.
 */
int CC2420Mac::handleControlCommand(cMessage * msg){
	trace() <<"handleControlCommand()";
	CC2420MacControlMessage *cmd = check_and_cast<CC2420MacControlMessage*>(msg) ;

	switch (cmd->getMcmType()) {

		case COMMAND:{
			if(cmd->getMcmInterface() == SEND){
				if(cmd->getMcmCommand() == CMD_RESEND){
					resend(cmd->getUseCca()) ;
				}
				else if(cmd->getMcmCommand() == CMD_CANCEL){
					command_Send_cancel() ;
				}
				else opp_error("Unkown command name") ;
			}
			else opp_error("Unkown Interface.") ;
			break;
		}

		case EVENT:{
			opp_error("Got event but was unexpected") ;
			break;
		}
		default:{
			opp_error("Unexpected message") ;
		}
	}
	delete cmd;
	return 1;
}

void CC2420Mac::fromNetworkLayer(cPacket *netPkt, int destination){
	trace () << "Incoming message from Network layer" ;
	CC2420Packet *macPkt = new CC2420Packet("CC2420 data packet", MAC_LAYER_PACKET);

	// fills the Mac packet fields
	macPkt->setFcf(DEFAULT_FCF) ; // init with all zeroes
	macPkt->setFcf(macPkt->getFcf() | IEEE154_TYPE_DATA << IEEE154_FCF_FRAME_TYPE ) ; // sets the packet as IEEE15.4 DATA type

	// sets the ack flag only if enabled from omnetpp.ini and if it is a unicast packet
	if(ackEnabled && destination != (uint16_t) BROADCAST_MAC_ADDRESS){
		trace()<<"destination: "<<(int) destination<<" , so ack required." ;
		macPkt->setFcf(macPkt->getFcf() | ( 1 << IEEE154_FCF_ACK_REQ ) ) ;
	}
	else{
		trace()<<"destination: "<<(int) destination<<" , so ack NOT required." ;
	}

	// fills the 802.15.4 fields
	macPkt->setDest(destination) ;
	macPkt->setDsn(dsn++) ;
	macPkt->setDestpan(DEFAULT_TOS_AM_GROUP);
	macPkt->setSrc(self) ;
	macPkt->setType(DEFAULT_AM_TYPE) ;
	macPkt->setFcs(DEFAULT_FCS) ; // we don't use this field, default 0.

	encapsulatePacket(macPkt, netPkt); // encapsulate the packet using the function defined in VirtualMac (the packet size is automatically updated).
	trace()<<"Packet dsn: "<<(int) (dsn-1) ;
	// Calls the Send.send command and sends the return value to the caller.
	error_t err = command_Send_send(macPkt,m_cca) ;
	return_Send_send(err) ;

}

void CC2420Mac::fromRadioLayer(cPacket * msg, double, double){
	trace() << "Incoming Message from radio layer, state: " << (uint16_t)m_state ;
	event_CC2420Receive_receive(msg) ;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



//////////////////////////// CC2420TransmitP.nc /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

error_t CC2420Mac::command_StdControl_start(){
	trace() << "MAC2420 - StdControl.start" ;
	m_state = S_STARTED ;
	m_receiving = false;
	abortSpiRelease = false;
	m_tx_power = 0;

	// follows ieee802.15.4 specs: random init then increment by one for each transmission
	dsn = command_Random_rand8(1) ;

	// custom variable for SpiResource implementation
	spiRelease = false ;
	return SUCCESS ;
}

error_t CC2420Mac::command_StdControl_stop(){
	command_SpiResource_release(SPI_TX) ; // tagged as //REMOVE in the original implementation...
	return SUCCESS ;
}

/**************** Send Commands ****************/
error_t CC2420Mac::command_Send_send( CC2420Packet* p_msg, bool useCca ) {
		return send( p_msg, useCca );
}

error_t CC2420Mac::command_Send_resend(bool useCca) {
	return resend( useCca );
}

error_t CC2420Mac::command_Send_cancel(){
	trace()<<"send_cancel()" ;
	switch( m_state ) {
		case S_LOAD:
		case S_SAMPLE_CCA:
		case S_BEGIN_TRANSMIT:
			m_state = S_CANCEL;
			break;

		default:
			// cancel not allowed while radio is busy transmitting
			return FAIL;
	}

	return SUCCESS;
}

// Send.modify not implemented
//async command error_t Send.modify( uint8_t offset, uint8_t* buf, uint8_t len )


// EnergyIndicator and ByteIndicator not implemented
//command bool EnergyIndicator.isReceiving()
//command bool ByteIndicator.isReceiving()

// We don't signal, we directly implement the backoff here.
void CC2420Mac::signal_RadioBackoff_requestInitialBackoff(){
	trace()<<"computeInitialBackoff()";
	myInitialBackoff = command_Random_rand16(1) % (0x1F * CC2420_BACKOFF_PERIOD) + CC2420_MIN_BACKOFF + 1;
}

void CC2420Mac::signal_RadioBackoff_requestCongestionBackoff(){
	trace()<<"computeCongestionBackoff()";
	myCongestionBackoff = command_Random_rand16(1) % (0x7 * CC2420_BACKOFF_PERIOD) + CC2420_MIN_BACKOFF + 1;
}

// skipped -> not useful
// inline uint32_t time16to32(uint16_t time, uint32_t recent_time)

void CC2420Mac::event_CaptureSFD_captured() {
	switch( m_state ) {

	case S_SFD:{
		trace() << "Changing to S_EFD" ;
		m_state = S_EFD;
		sfdHigh = true;

		// packet timestamp stuffs skipped.

		if ( m_msg->getFcf() & ( 1 << IEEE154_FCF_ACK_REQ ) ) {
			// This is an ack packet, don't release the chip's SPI bus lock.
			abortSpiRelease = true;
		}
		releaseSpiResource();
		command_BackoffTimer_stop() ;

		break;
	}
	case S_EFD:{
		sfdHigh = false;

		if ( m_msg->getFcf() & ( 1 << IEEE154_FCF_ACK_REQ ) ) {
			trace() << "Ack required, changing state to S_ACK_WAIT and setting backoff timer with value: "<<tos32KhzToSeconds(CC2420_ACK_WAIT_DELAY);
			m_state = S_ACK_WAIT;
			command_BackoffTimer_start(CC2420_ACK_WAIT_DELAY) ;
		} else {
			trace()<<"No Ack required, calling signalDone(SUCCESS)";
			signalDone(SUCCESS);
		}

			break;
	}

	default:{
		ostringstream err ;
		err<<"Shouldn't happen in this implementation: "<<(int) m_state ;
		opp_error(err.str().c_str()) ;
		break;
	}

	}
}

/***************** ChipSpiResource Events ****************/
void CC2420Mac::event_ChipSpiResource_releasing() {
	if(abortSpiRelease){
		command_ChipSpiResource_abortRelease() ;
	}
}

/*
 * IMPORTANT: Here we have combined the receive event of both CC2420Transmit and CC2420Receive
 * In the former case it is an ack message while in the latter it is an incoming IEEE15.4 data packet.
 *
 * Transmit:
 * If the state is S_ACK_WAIT, the SpiResource is already reserved.
 *
 * Receive:
 * Our implementation can receive packets only if the state is S_STARTED otherwise they are dropped.
 * When receiving a packet in S_STARTED, the SpiResource is supposed not to be acquired thus,
 * the immediateRequest must always return SUCCESS. Once the packet has been received and forwarded to upper layer,
 * the SpiResource is released only if no ack is required back to the transmitter. If an ack is being transmitted, the
 * ACK_SENT event will be fired at the end of the transmission and the SpiResource released.
 *
 */
void CC2420Mac::event_CC2420Receive_receive(cPacket * incoming_msg){

	trace()<<"Receive_receive called!";

	CC2420AckPacket *ack_hdr ;
	CC2420Packet *msg_hdr ;

	CC2420Ieee802154Packet* pkt = check_and_cast<CC2420Ieee802154Packet*>(incoming_msg) ;

	//////////////////////// Receive_receive from CC2420Transmit ///////////////////
	////////////////////////////////////////////////////////////////////////////////
	if(((pkt->getFcf() >> IEEE154_FCF_FRAME_TYPE ) & 7) == IEEE154_TYPE_ACK ){ // it's an ack packet

		ack_hdr = check_and_cast<CC2420AckPacket*>(pkt) ;


		if(m_state == S_ACK_WAIT) trace()<<"Ack frame, checking for pending send. Dsn sent:"<<(int)m_msg->getDsn()<<"  , dsn rx: "<<(int)ack_hdr->getDsn() ;

		if ( m_state == S_ACK_WAIT && m_msg->getDsn() == ack_hdr->getDsn() ) {
			command_BackoffTimer_stop() ;

			//useless to set all metadata fields of tinyos. The only one which is important for us is the ack field:
			m_msg->setMetaWasAcked(true) ;
			signalDone(SUCCESS);
		}
	}

	//////////////////////// Receive_receive from CC2420Receive  ///////////////////
	////////////////////////////////////////////////////////////////////////////////
	else if(((pkt->getFcf() >> IEEE154_FCF_FRAME_TYPE ) & 7) == IEEE154_TYPE_DATA){ // it's an incoming data packet

		trace()<<"Data frame";
		msg_hdr = check_and_cast<CC2420Packet*>(pkt) ;
		if(m_state == S_STARTED){ // ok it's safe here...
			bool ackpending = false ;

			if(command_SpiResource_immediateRequest(SPI_RX) != SUCCESS) opp_error("Received msg while spi busy.") ;
			trace()<<"State S_STARTED so receiving";

			if(msg_hdr->getDest() == self || msg_hdr->getDest() == (uint16_t) BROADCAST_MAC_ADDRESS){
				trace()<<"For me! - "<<(int)msg_hdr->getDest()<<" , from: "<<(int)msg_hdr->getSrc() ;
				if(((msg_hdr->getFcf() >> IEEE154_FCF_ACK_REQ) & 0x01) == 1){
					trace()<<"Require ACK! - dsn: "<<(int)msg_hdr->getDsn();
					// require acknowledgement so can't release the SpiResource. At the end of the ack transmission
					// an ACK_SENT event will be generated and the SpiResource released there.

					CC2420AckPacket * ack_reply = new CC2420AckPacket("CC2420 ack reply", MAC_LAYER_PACKET) ;
					ack_reply->setFcf(DEFAULT_FCF) ;
					ack_reply->setFcf(ack_reply->getFcf() | (IEEE154_TYPE_ACK << IEEE154_FCF_FRAME_TYPE) ) ;
					ack_reply->setDsn(msg_hdr->getDsn()) ;
					ack_reply->setFcs(DEFAULT_FCS) ;
					ack_reply->setByteLength(MACAckOverhead) ;
					toRadioLayer(ack_reply);
					toRadioLayer(createRadioCommand(SET_STATE, TX));
					ackpending = true ;

					/* TRICK: since I don't receive a feedback from the radio related to the end of the transmission,
					 * I compute the value by using the same parameters of the radio and sets the timer to simulate the event.
					 *
					 * I suppose the transmission to begin when the SET_STATE TX command is send thus, the time to transmit
					 * the packet is given as:
					 * 		(PktLength + phyOverhead) * 8 / datarate
					 */

					double txTime = ((double)((ack_reply->getByteLength()+PhyFrameOverhead) * 8.0f)) / datarate;
					setTimer(ACK_SENT,tos32KhzToSeconds(CC2420_TIME_ACK_TURNAROUND)+txTime) ;
					trace()<<"Ack sent , turnaround and transmission set to: "<<(tos32KhzToSeconds(CC2420_TIME_ACK_TURNAROUND)+txTime);

				}
			}

			/* Sends the received packet to upper layer. I don't need to change the state of
			 * the radio: it is completely transparent to the CC2420transmit.
			 *
			 * The standard routing fields nextHop, lastHop, LQI and RSSI are used to propagate some MAC fields:
			 * 	- nextHop is the destination of the mac packet (it discriminates packets for us or snooped packets)
			 *  - lastHop is the source of the mac packet
			 *  - LQI and RSSI propagates the corresponding fields of the MAC layer.
			 */
			RoutingPacket* msg = check_and_cast<RoutingPacket*>(msg_hdr->decapsulate()) ;
			msg->getNetMacInfoExchange().nextHop = msg_hdr->getDest() ;
			msg->getNetMacInfoExchange().lastHop = msg_hdr->getSrc() ;
			msg->getNetMacInfoExchange().LQI = msg_hdr->getMacRadioInfoExchange().LQI ;
			msg->getNetMacInfoExchange().RSSI = msg_hdr->getMacRadioInfoExchange().RSSI ;
			toNetworkLayer(msg) ;

			// release SpiResource only if no ack is being transmitted
			if(!ackpending) command_SpiResource_release(SPI_RX) ;
		}
		else{
			// For debugging:
			if(m_state == S_SAMPLE_CCA || m_state == S_ACK_WAIT || m_state == S_BEGIN_TRANSMIT || m_state == S_EFD){
				trace()<<"Packet received while not in S_STARTED. State: "<<(int)m_state<<" , was for me: "<<(msg_hdr->getDest() == self || msg_hdr->getDest() == (uint16_t) BROADCAST_MAC_ADDRESS) ;
			}
			else{
				ostringstream err ;
				err<<"Unexpected state: "<<(int) m_state ;
				opp_error(err.str().c_str()) ;
			}
		}
	}
	else opp_error("Unkown Packet Type") ;
}

void CC2420Mac::event_SpiResource_granted() {
	trace()<<"SpiResource Granted - state: "<<(int)m_state ;

	uint8_t cur_state;

	cur_state = m_state;

	switch( cur_state ) {
	case S_LOAD:
		loadTXFIFO();
		break;

	case S_BEGIN_TRANSMIT:
		attemptSend();
		break;

	case S_CANCEL:
		releaseSpiResource();

		m_state = S_STARTED;

		signal_Send_sendDone(ECANCEL) ;
		break;

	default:
		releaseSpiResource();
		break;
	}
}

/***************** TXFIFO Events ****************/
/**
 * The TXFIFO is used to load packets into the transmit buffer on the
 * chip
 */
void CC2420Mac::event_TXFIFO_writeDone( /*uint8_t* tx_buf, uint8_t tx_len, uint8_t error*/ ) {
	trace() << "TXFIFO write done" ;
	if ( m_state == S_CANCEL ) {
		trace() << "state S_CANCEL, releasing SPI and changing to S_STARTED" ;
		releaseSpiResource();
		m_state = S_STARTED;
		signal_Send_sendDone(ECANCEL) ;
	} else if ( !m_cca ) {
		trace () << "cca disabled, changing to S_BEGIN_TRANSMIT" ;
		m_state = S_BEGIN_TRANSMIT;
		attemptSend();
	} else {
		trace() << "cca enabled, changing to S_SAMPLE_CCA and releasing SpiResource";
		releaseSpiResource();
		m_state = S_SAMPLE_CCA;
		signal_RadioBackoff_requestInitialBackoff() ;
		trace()<< "Initial backoff started, value: "<<tos32KhzToSeconds(myInitialBackoff);
		command_BackoffTimer_start(myInitialBackoff) ;
	}
}

// TXFIFO.readDone not implemented

/***************** Timer Events ****************/
/**
 * The backoff timer is mainly used to wait for a moment before trying
 * to send a packet again. But we also use it to timeout the wait for
 * an acknowledgement, and timeout the wait for an SFD interrupt when
 * we should have gotten one.
 */
void CC2420Mac::event_BackoffTimer_fired(){
	int stat = -1 ;
	trace() << "Backoff timer fired!" ;
	switch( m_state ) {

	case S_SAMPLE_CCA :
		// sample CCA and wait a little longer if free, just in case we
		// sampled during the ack turn-around window
		stat = radioModule->isChannelClear() ;
		if ( stat == CLEAR ) {
			trace()<<"Channel clear, changing to S_BEGIN_TRANSMIT" ;
			m_state = S_BEGIN_TRANSMIT;
			trace()<<"Backoff timer set, value: "<<tos32KhzToSeconds(CC2420_TIME_ACK_TURNAROUND) ;
			command_BackoffTimer_start(CC2420_TIME_ACK_TURNAROUND) ;
		} else {
			trace() <<"Channel busy: "<< stat <<", congestion backoff" ;
			congestionBackoff();
		}
		break;

	case S_BEGIN_TRANSMIT:
	case S_CANCEL:
		if ( acquireSpiResource() == SUCCESS ) {
			attemptSend();
		}
		break;

	case S_ACK_WAIT:
		trace() << "Was S_ACK_WAIT, signaling signalDone(SUCCESS)" ;
		m_msg->setMetaWasAcked(false) ; // our implementation
		signalDone(SUCCESS) ;
		break;

	case S_SFD:
		trace() << "Don't know what I'm doing here, releasing SpiResource and signaling SignalDone(ERETRY)" ;
		// We didn't receive an SFD interrupt within CC2420_ABORT_PERIOD
		// jiffies. Assume something is wrong.
		releaseSpiResource();
		signalDone(ERETRY) ;
		break;

	default:
		break;
	}
}

/***************** Functions ****************/
/**
 * Set up a message to be sent. First load it into the outbound tx buffer
 * on the chip, then attempt to send it.
 * @param *p_msg Pointer to the message that needs to be sent
 * @param cca TRUE if this transmit should use clear channel assessment
 */
error_t CC2420Mac::send( CC2420Packet* p_msg, bool cca ){
	trace()<<"send()" ;
	if (m_state == S_CANCEL) {
		trace () << "MAC state S_CANCEL, returning ECANCEL" ;
		return ECANCEL;
	}
	if ( m_state != S_STARTED ) {
		trace () << "MAC not statrted, returning FAIL" ;
		return FAIL;
	}

	m_state = S_LOAD;
	m_cca = cca;
	m_msg = p_msg;
	totalCcaChecks = 0;

	trace() << "MAC state changed to S_LOAD, trying to acquire Spi resource" ;
	if ( acquireSpiResource() == SUCCESS ) {
		loadTXFIFO();
	}
	return SUCCESS;
}

/**
 * Resend a packet that already exists in the outbound tx buffer on the
 * chip
 * @param cca TRUE if this transmit should use clear channel assessment
 */
error_t CC2420Mac::resend( bool cca ){
	trace()<<"resend()" ;
	if (m_state == S_CANCEL) {
		return ECANCEL;
	}

	if ( m_state != S_STARTED ) {
		return FAIL;
	}

	m_cca = cca;
	m_state = cca ? S_SAMPLE_CCA : S_BEGIN_TRANSMIT;
	totalCcaChecks = 0;

	if(m_cca) {
		signal_RadioBackoff_requestInitialBackoff() ;
		command_BackoffTimer_start(myInitialBackoff) ;

	} else if ( acquireSpiResource() == SUCCESS ) {
		attemptSend();
	}

	return SUCCESS;
}

/**
 * Attempt to send the packet we have loaded into the tx buffer on
 * the radio chip.  The STXONCCA will send the packet immediately if
 * the channel is clear.  If we're not concerned about whether or not
 * the channel is clear (i.e. m_cca == FALSE), then STXON will send the
 * packet without checking for a clear channel.
 *
 * If the packet didn't get sent, then congestion == TRUE.  In that case,
 * we reset the backoff timer and try again in a moment.
 *
 * If the packet got sent, we should expect an SFD interrupt to take
 * over, signifying the packet is getting sent.
 */
void CC2420Mac::attemptSend(){
	trace()<<"AttemptSend()";
	uint8_t status;
	bool congestion = true;

	if (m_state == S_CANCEL) {
		trace()<<"Was S_CANCEL, releasing SpiResource and changing state to S_STARTED" ;
		releaseSpiResource();
		m_state = S_STARTED;
		trace() <<"signaling sendDone(ECANCEL)" ;
		signal_Send_sendDone(ECANCEL) ;
		return;
	}

	/***************** FROM CC2420 DATASHEET **********************/
	//	  Implementing CSMA-CA may easiest be done by using the STXONCCA command strobe,
	//	  as described in the Radio control state machine section on page 42. Transmission
	//	  will then only start if the channel is clear. The TX_ACTIVE status bit (see Table 5)
	//	  may be used to detect the result of the CCA.

	status = m_cca ? radioModule->isChannelClear() : CLEAR ; // instead of clear, in the original code there is the strobe for packet sending. We send it just next...
	if(status == CLEAR){
		trace() << "Channel clear before send... sending" ;

		toRadioLayer(m_msg->dup());
		toRadioLayer(createRadioCommand(SET_STATE, TX));

		congestion = false ;
	}
	else trace()<<"Channel NOT clear: "<<(int)status;

	m_state = congestion ? S_SAMPLE_CCA : S_SFD;

	if ( congestion ) {
		trace()<<"Channel not clear before send, congestionBackoff" ;
		totalCcaChecks = 0;
		releaseSpiResource();
		congestionBackoff();
	} else {
		trace()<<"Backoff Timer set with abort period: "<<tos32KhzToSeconds(CC2420_ABORT_PERIOD);
		command_BackoffTimer_start(CC2420_ABORT_PERIOD) ;

		/*
		 * Emulates the SFD and EFD by starting SFD now and generating an EFD at time:
		 *
		 * (PhyPacketOverhead + packetLength ) * 8 / datarate
		 *
		 */
		setTimer(CAPTURE_SFD,0) ; // directly call SFD interrupt
		double txTime = ((double)((m_msg->getByteLength()+PhyFrameOverhead) * 8.0f)) / datarate;
		trace()<<"CAPTURE_SFD event scheduled now and EFD in: "<<txTime;
		setTimer(CAPTURE_EFD,txTime) ; // schedule the end of the packet transmission.
	}
}

void CC2420Mac::congestionBackoff(){
	trace()<<"congestionBackoff()" ;
	signal_RadioBackoff_requestCongestionBackoff() ;
	command_BackoffTimer_start(myCongestionBackoff) ;
}

error_t CC2420Mac::acquireSpiResource(){
	trace()<<"acquireSpiResource()" ;

	error_t error = command_SpiResource_immediateRequest(SPI_TX) ;
	if(error != SUCCESS){
		command_SpiResource_request(SPI_TX) ;
	}
	return error ;
}

error_t CC2420Mac::releaseSpiResource(){
	trace()<<"releaseSpiResource()" ;
	return command_SpiResource_release(SPI_TX) ;
}

void CC2420Mac::loadTXFIFO(){
	trace()<<"loadTXFIFO()" ;
//	1 - sets the output power based on header content - not implemented (but maybe in future it can be added)
//	2 - writes the packet content in the buffer - simulated through a timeout
	setTimer(TXFIFO_WRITE,txFifoWriteTimeout) ;
}

void CC2420Mac::signalDone(error_t err ){
	trace()<<"signalDone()" ;
	m_state = S_STARTED;
	abortSpiRelease = false;
	command_ChipSpiResource_attemptRelease() ;
	signal_Send_sendDone(err);
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////// Custom Functions /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


void CC2420Mac::command_BackoffTimer_start(uint32_t t){
	setTimer(BACKOFF,tos32KhzToSeconds(t)) ;
}

void CC2420Mac::command_BackoffTimer_stop(){
	cancelTimer(BACKOFF) ;
}

void CC2420Mac::signal_Send_sendDone(error_t err){
	trace()<<"signal_sendDone()" ;
	CC2420MacControlMessage *msg = new CC2420MacControlMessage("CC2420 Mac control message", MAC_CONTROL_MESSAGE);
	msg->setMcmType(EVENT) ;
	msg->setMcmInterface(SEND) ;
	msg->setMcmEvent(EVT_SENDDONE);
	msg->setError(err) ;
	msg->setMetaWasAcked(m_msg->getMetaWasAcked()) ;
	msg->setMetaDest(m_msg->getDest()) ;
	toNetworkLayer(msg) ;
	delete m_msg ;
	m_msg = NULL ;
}

/*
 * Creates a new Control packet with the return value of the Send.send command
 * and sends it to the Routing layer.
 */
void CC2420Mac::return_Send_send(error_t err){
	CC2420MacControlMessage* errMsg = new CC2420MacControlMessage("CC2420 Mac control message", MAC_CONTROL_MESSAGE) ;
	errMsg->setMcmType(COMMAND_RETURN) ;
	errMsg->setMcmInterface(SEND) ;
	errMsg->setMcmCommand(CMD_SEND);
	errMsg->setError(err) ;
	toNetworkLayer(errMsg) ;
}


/*
 * SpiResource implementation. -----------------------------------------------------------------------
 */
error_t CC2420Mac::command_SpiResource_request(spi_request_t sr){
	trace()<<"SpiResource - request: "<<(int) sr ;
	if(spiResource == SPI_NULL || spiResource == sr){
		spiResource = sr ;
		signal_SpiResource_granted(spiResource) ;
	}
	else{
		spiRequests.push(sr) ;
	}
	return SUCCESS ;
}

error_t CC2420Mac::command_SpiResource_immediateRequest(spi_request_t sr){
	trace()<<"SpiResource - immediateRequest: "<<(int) sr ;
	if(spiRequests.empty() && (spiResource == SPI_NULL || spiResource == sr)){
		spiResource = sr ;
		return SUCCESS ;
	}
	else return EBUSY ;
}

// If not the owner, return fail, otherwise attemptRelease() or signal next grant.
error_t CC2420Mac::command_SpiResource_release(spi_request_t sr){
	trace()<<"SpiResource - release: "<<(int)sr ;
	if(spiResource != sr){
		trace()<<"Failed!" ;
		return FAIL ;
	}

	if(spiRequests.empty()){
		attemptRelease() ;
	}
	else{
		trace()<<"Calling next..." ;
		spiResource = spiRequests.front() ;
		spiRequests.pop() ;
		signal_SpiResource_granted(spiResource) ;
	}
	return SUCCESS ;

}

error_t CC2420Mac::command_ChipSpiResource_attemptRelease(){
	return attemptRelease() ;
}

void CC2420Mac::command_ChipSpiResource_abortRelease(){
	spiRelease = false ;
}

error_t CC2420Mac::attemptRelease(){
	trace()<<"SpiResource - attempt Release..." ;
	if(spiRequests.size() > 0){
		trace()<<"Failed!" ;
		return FAIL;
	}

	spiRelease = true ;
	signal_ChipSpiResource_releasing() ;

	if(spiRelease) {
		spiResource = SPI_NULL ;
		trace()<<"Done!" ;
		return SUCCESS;
	}
	trace()<<"Busy!";
	return EBUSY;
}

void CC2420Mac::signal_ChipSpiResource_releasing(){
	event_ChipSpiResource_releasing() ;
}

void CC2420Mac::signal_SpiResource_granted(spi_request_t sr){
	trace()<<"SpiResource - Spi granted: "<<(int)sr ;
	if(sr == SPI_TX){
		setTimer(SPI_TX_GRANTED,0) ;
	}
	else if(sr == SPI_RX){
		setTimer(SPI_RX_GRANTED,0) ;
	}
	else opp_error("Unkown state!") ;
}
// ------------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
