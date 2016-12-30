/*
 * @author Ugo Colesanti
 * @author Silvia Santini
 * @version 1.01 (January 3, 2012)
 */

#ifndef _TOS_ENVIRONMENT_H_
#define _TOS_ENVIRONMENT_H_
#include "CastaliaModule.h"

typedef uint8_t error_t;
typedef uint16_t am_addr_t ;

enum{
	AM_BROADCAST_ADDR = (am_addr_t) 0xffff,
	TOS_BCAST_ADDR = (am_addr_t) 0xffff,
	TOSH_DATA_LENGTH = 28,
};

enum tos_err_types{
    SUCCESS = 1,
    FAIL = 2,
    EBUSY = 3,
    ERETRY = 4,
    ECANCEL = 5,
    EOFF = 6,
    ESIZE = 7
};

// notifications
enum Notification_type{
	EVENT = 1 ,
	COMMAND = 2 ,
	COMMAND_RETURN = 3,
};

enum Notification_interface{
	SEND = 1 ,
};

enum Notification_event{
	EVT_SENDDONE = 1 ,
};

enum Notification_command{
	CMD_SEND = 1 ,
	CMD_RESEND = 2 ,
	CMD_CANCEL = 3 ,
};

/*
 * Converts milliseconds in TinyOs into seconds in Castalia
 * Note that in TinyOs a second is represented by 1024 milliseconds
 */
inline double tosMillisToSeconds(uint32_t millis){
	long castalia_millis = (millis*1000) / 1024 ; // we want to truncate the value but prevent overflow -> long instead of int.
	return ((double) castalia_millis) / 1000.0 ; // converted in Castalia's seconds.
}

/*
 * Converts milliseconds in TinyOs into seconds in Castalia
 * Note that in TinyOs a second is represented by 1024 milliseconds
 */
inline double tos32KhzToSeconds(uint32_t t_32){
	return ((double) t_32 / 32768.0);
}

inline uint8_t command_Random_rand8(int k){
	int rvalue =  genk_intrand(k,0x100) ; // random number in range [0,2^8)
	return rvalue & 0xff ;
}

inline uint16_t command_Random_rand16(int k){
	return (uint16_t)genk_intrand(k,0x10000) ; // random number in range [0,2^16)
}

inline uint32_t command_Random_rand32(int k){
	return (uint32_t) genk_intrand(k,0) ; // random number in range [0,2^32)
}
#endif
