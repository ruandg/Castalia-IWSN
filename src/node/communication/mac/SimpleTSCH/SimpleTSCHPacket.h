#ifndef _SimpleTSCHPACKET_
#define _SimpleTSCHPACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112


enum SimpleTSCHFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class SimpleTSCHPacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_tsch;
  private:
    void copy(const SimpleTSCHPacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const SimpleTSCHPacket&);

  public:
    SimpleTSCHPacket(const char *name=NULL, int kind=0);
    SimpleTSCHPacket(const SimpleTSCHPacket& other);
    virtual ~SimpleTSCHPacket();
    SimpleTSCHPacket& operator=(const SimpleTSCHPacket& other);
    virtual SimpleTSCHPacket *dup() const {return new SimpleTSCHPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
	virtual bool getAckReq() const;
	virtual void setAckReq(bool);
	int getKindTSCH();
	void setKindTSCH(int);
};

inline void doPacking(cCommBuffer *b, SimpleTSCHPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SimpleTSCHPacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _SimpleTSCHPACKET_

