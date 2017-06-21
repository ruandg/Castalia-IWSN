#ifndef _SimpleDSMEPACKET_
#define _SimpleDSMEPACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112


enum SimpleDSMEFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class SimpleDSMEPacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_dsme;
	int ack_bitmap;
	int channel_bitmap;
  private:
    void copy(const SimpleDSMEPacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const SimpleDSMEPacket&);

  public:
    SimpleDSMEPacket(const char *name=NULL, int kind=0);
    SimpleDSMEPacket(const SimpleDSMEPacket& other);
    virtual ~SimpleDSMEPacket();
    SimpleDSMEPacket& operator=(const SimpleDSMEPacket& other);
    virtual SimpleDSMEPacket *dup() const {return new SimpleDSMEPacket(*this);}
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

inline void doPacking(cCommBuffer *b, SimpleDSMEPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SimpleDSMEPacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _SimpleDSMEPACKET_

