#ifndef _ABMPTreePACKET_
#define _ABMPTreePACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112
#define MAXNODES 100


enum ABMPTreeFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class ABMPTreePacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_dsme;
	//int ack_bitmap;
	int *ack_bitmap;
	int *channel;
	int beacon_id;
	int blacklist;
	int first_ch;
	int b0_error;
	int change_bl;
	int beacon_dup;
	
  private:
    void copy(const ABMPTreePacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const ABMPTreePacket&);

  public:
    ABMPTreePacket(const char *name=NULL, int kind=0);
    ABMPTreePacket(const ABMPTreePacket& other);
    virtual ~ABMPTreePacket();
    ABMPTreePacket& operator=(const ABMPTreePacket& other);
    virtual ABMPTreePacket *dup() const {return new ABMPTreePacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
	virtual bool getAckReq() const;
	virtual void setAckReq(bool);
	virtual int* getAckBitmap() const;
	virtual void setAckBitmap(int*);	
	
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
	virtual int getBeaconDup() const;
	virtual void setBeaconDup(int);

	int getKindDSME();
	void setKindDSME(int);
};

inline void doPacking(cCommBuffer *b, ABMPTreePacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, ABMPTreePacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _ABMPTreePACKET_

