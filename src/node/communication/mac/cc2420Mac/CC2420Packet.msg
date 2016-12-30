// /*
//  * @author Ugo Colesanti
//  * @author Silvia Santini
//  * @version 1.0 (January 27, 2011)
//  *
//  * Acknowledgment: This code is based upon the implementation of CC2420Transmit TinyOS component
//  * written by Jonathan Hui, David Moss, and Jung Il Choi.
//  */

// /*
//  * Copyright (c) 2011 Sapienza University of Rome.
//  * All rights reserved.
//  *
//  * Redistribution and use in source and binary forms, with or without
//  * modification, are permitted provided that the following conditions
//  * are met:
//  * - Redistributions of source code must retain the above copyright
//  *   notice, this list of conditions and the following disclaimer.
//  * - Redistributions in binary form must reproduce the above copyright
//  *   notice, this list of conditions and the following disclaimer in the
//  *   documentation and/or other materials provided with the
//  *   distribution.
//  * - Neither the name of the Sapienza University of Rome nor the names of
//  *   its contributors may be used to endorse or promote products derived
//  *   from this software without specific prior written permission.
//  *
//  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL SAPIENZA 
//  * UNIVERSITY OF ROME OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//  * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//  * OF THE POSSIBILITY OF SUCH DAMAGE.
//  */
 
// /*
//  * Copyright (c) 2011 ETH Zurich.
//  * All rights reserved.
//  *
//  * Redistribution and use in source and binary forms, with or without
//  * modification, are permitted provided that the following conditions
//  * are met:
//  * - Redistributions of source code must retain the above copyright
//  *   notice, this list of conditions and the following disclaimer.
//  * - Redistributions in binary form must reproduce the above copyright
//  *   notice, this list of conditions and the following disclaimer in the
//  *   documentation and/or other materials provided with the
//  *   distribution.
//  * - Neither the name of ETH Zurich nor the names of
//  *   its contributors may be used to endorse or promote products derived
//  *   from this software without specific prior written permission.
//  *
//  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ETH 
//  * ZURICH OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
//  * OF THE POSSIBILITY OF SUCH DAMAGE.
//  */

cplusplus {{
#include "MacPacket_m.h"
}};

enum Notification_type ;
enum Notification_interface ;
enum Notification_command ;
enum Notification_event ;
enum tos_err_types ;

class MacPacket;
class MacControlMessage ;

//////////////////// PACKETS 

enum CC2420Packet_type {
}

packet CC2420Ieee802154Packet extends MacPacket{
	uint16_t fcf ;
	uint8_t dsn ;
	uint16_t fcs ;
	bool metaWasAcked ;
}
packet CC2420Packet extends CC2420Ieee802154Packet {
	uint16_t destpan ;
	uint16_t dest ;
	uint16_t src ;
	uint8_t type ;
}

packet CC2420AckPacket extends CC2420Ieee802154Packet{
	
}

/////////////////// MESSAGES

//enum CC2420MacControlMessage_type{
//	EVENT = 1 ;
//	COMMAND = 2 ;
//}

//enum CC2420MacControlMessage_interface{
//	SEND = 1 ;
//}

//enum CC2420MacControlMessage_event{
//	EVT_SENDDONE = 1 ;
//}

//enum CC2420MacControlMessage_command{
//	CMD_SEND = 1 ;
//	CMD_RESEND = 2 ;
//	CMD_CANCEL = 3 ;
//}

//enum CC2420MacControlMessage_error{
//	SUCCESS = 1 ;
//	FAIL = 2 ;
//	EBUSY = 3 ;
//	ERETRY = 4 ;
//	ECANCEL = 5 ;
//	EOFF = 6 ;
//	ESIZE = 7 ;
//}

message CC2420MacControlMessage extends MacControlMessage{
	int mcmType enum (Notification_type) ;
	int mcmInterface enum (Notification_interface) ;
	int mcmEvent enum (Notification_event) ;
	int mcmCommand enum (Notification_command) ;
	uint8_t error enum (tos_err_types) ;
	bool useCca ;
	bool metaWasAcked ;
	uint16_t metaDest ;
}