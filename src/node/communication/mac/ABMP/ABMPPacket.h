#ifndef _ABMPPACKET_
#define _ABMPPACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112
#define QTDNODES 5


enum ABMPFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class ABMPPacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_dsme;
	int ack_bitmap;
	int *channel;
	int beacon_id;
	int blacklist;
	int first_ch;
	int b0_error;
	int change_bl;
	
  private:
    void copy(const ABMPPacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const ABMPPacket&);

  public:
    ABMPPacket(const char *name=NULL, int kind=0);
    ABMPPacket(const ABMPPacket& other);
    virtual ~ABMPPacket();
    ABMPPacket& operator=(const ABMPPacket& other);
    virtual ABMPPacket *dup() const {return new ABMPPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
	virtual bool getAckReq() const;
	virtual void setAckReq(bool);
	virtual int getAckBitmap() const;
	virtual void setAckBitmap(int);	
	
	virtual int* getChannel() const;
	virtual void setChannel(int*);
	
	virtual int getBeaconId() const;
	virtual void setBeaconId(int);
	virtual int getBlacklist() const;
	virtual void setBlacklist(int);
	virtual int getFirstch() const;
	virtual void setFirstch(int);
	virtual int getB0error() const;
	virtual void setB0error(int);
	virtual int getChangebl() const;
	virtual void setChangebl(int);

	int getKindDSME();
	void setKindDSME(int);
};

inline void doPacking(cCommBuffer *b, ABMPPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ABMPPacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _ABMPPACKET_

