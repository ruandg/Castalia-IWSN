#ifndef _SimpleTSCHTreePACKET_
#define _SimpleTSCHTreePACKET_

#include <omnetpp.h>

#include "MacPacket_m.h"

#define ACK_PACKET 222
#define DATA_PACKET 111
#define BEACON_PACKET 112


enum SimpleTSCHTreeFrameTypeDef {
    DATA_FRAME = 1,
    BEACON_FRAME = 2,
	ACK_FRAME = 3
};

class SimpleTSCHTreePacket : public ::MacPacket
{
  protected:
	bool ackReq;
	int kind_tsch;
  private:
    void copy(const SimpleTSCHTreePacket& other);
	
  protected:
	 // protected and unimplemented operator==(), to prevent accidental usage
  bool operator==(const SimpleTSCHTreePacket&);

  public:
    SimpleTSCHTreePacket(const char *name=NULL, int kind=0);
    SimpleTSCHTreePacket(const SimpleTSCHTreePacket& other);
    virtual ~SimpleTSCHTreePacket();
    SimpleTSCHTreePacket& operator=(const SimpleTSCHTreePacket& other);
    virtual SimpleTSCHTreePacket *dup() const {return new SimpleTSCHTreePacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
	virtual bool getAckReq() const;
	virtual void setAckReq(bool);
	int getKindTSCH();
	void setKindTSCH(int);
};

inline void doPacking(cCommBuffer *b, SimpleTSCHTreePacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, SimpleTSCHTreePacket& obj) {obj.parsimUnpack(b);}

#endif // ifndef _SimpleTSCHTreePACKET_

