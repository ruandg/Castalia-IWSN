#ifndef _DSMEACKPACKET_
#define _DSMEACKPACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112


enum DSMEACKFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class DSMEACKPacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_dsme;
	int ack_bitmap;
	int channel_bitmap;
  private:
    void copy(const DSMEACKPacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const DSMEACKPacket&);

  public:
    DSMEACKPacket(const char *name=NULL, int kind=0);
    DSMEACKPacket(const DSMEACKPacket& other);
    virtual ~DSMEACKPacket();
    DSMEACKPacket& operator=(const DSMEACKPacket& other);
    virtual DSMEACKPacket *dup() const {return new DSMEACKPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
	virtual bool getAckReq() const;
	virtual void setAckReq(bool);
	virtual int getAckBitmap() const;
	virtual void setAckBitmap(int);
	virtual int getChannelBitmap() const;
	virtual void setChannelBitmap(int);
	int getKindDSME();
	void setKindDSME(int);
};

inline void doPacking(cCommBuffer *b, DSMEACKPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, DSMEACKPacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _DSMEACKPACKET_

