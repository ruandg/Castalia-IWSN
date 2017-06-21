#include <iostream>
#include <sstream>
#include "ABMPTreePacket.h"

USING_NAMESPACE

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}


// Template rule for outputting std::vector<T> types
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("ABMPTreeFrameTypeDef");
    if (!e) enums.getInstance()->add(e = new cEnum("ABMPTreeFrameTypeDef"));
    e->insert(DATA_FRAME, "DATA_FRAME");
    e->insert(BEACON_FRAME, "BEACON_FRAME");
    e->insert(ACK_FRAME, "ACK_FRAME");
	
);

Register_Class(ABMPTreePacket);

ABMPTreePacket::ABMPTreePacket(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->ackReq = 0;
	//this->ack_bitmap = 0;
	//TODO: alocacao dinamica
	this->ack_bitmap = new int[100];
 	for(int i = 0; i < 100; i++) {
		this->ack_bitmap[i] = 0;
	}
	this->channel = new int[MAXNODES];
 	for(int i = 0; i < MAXNODES; i++) {
		this->channel[i] = 0;
	}
 	this->beacon_id = 0;
	this->blacklist = 0x0F;
	this->first_ch = 0;
	this->b0_error = 0;
	this->change_bl = 0;
	this->beacon_dup = 0;
}

ABMPTreePacket::ABMPTreePacket(const ABMPTreePacket& other) : ::MacPacket(other)
{
	this->channel = new int[MAXNODES];
	this->ack_bitmap = new int[100];
    copy(other);
}

ABMPTreePacket::~ABMPTreePacket()
{
	delete ack_bitmap;
	delete channel;
}

ABMPTreePacket& ABMPTreePacket::operator=(const ABMPTreePacket& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void ABMPTreePacket::copy(const ABMPTreePacket& other)
{
    this->ackReq = other.ackReq;
	//this->ack_bitmap = other.ack_bitmap;
	//TODO: alocacao dinamica
	for(int i = 0; i < 100; i++)
		this->ack_bitmap[i] = other.ack_bitmap[i];
	this->kind_dsme = other.kind_dsme;
	for(int i = 0; i < MAXNODES; i++)
		this->channel[i] = other.channel[i];
	this->beacon_id = other.beacon_id;
	this->blacklist = other.blacklist;
	this->first_ch = other.first_ch;
	this->b0_error = other.b0_error;
	this->change_bl = other.change_bl;
	this->beacon_dup = other.beacon_dup;
}

void ABMPTreePacket::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->ackReq);
}

void ABMPTreePacket::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->ackReq);
}

bool ABMPTreePacket::getAckReq() const
{
    return ackReq;
}

void ABMPTreePacket::setAckReq(bool ackReq)
{
    this->ackReq = ackReq;
}

int* ABMPTreePacket::getAckBitmap() const
{
    return ack_bitmap;
}

void ABMPTreePacket::setAckBitmap(int *ack_bitmap)
{
   // this->ack_bitmap = ack_bitmap;
	//TODO: alocacao dinamica
	for(int i = 0; i < 100; i++)
    	this->ack_bitmap[i] = ack_bitmap[i];
}

int *ABMPTreePacket::getChannel() const
{
    return channel;
}

void ABMPTreePacket::setChannel(int *channel)
{
	for(int i = 0; i < MAXNODES; i++)
    	this->channel[i] = channel[i];
}

void ABMPTreePacket::setBeaconId(int beacon_id) {
	this->beacon_id = beacon_id;
}

int ABMPTreePacket::getBeaconId() const{
	return beacon_id;
}

void ABMPTreePacket::setBlacklist(int blacklist) {
	this->blacklist = blacklist;
}

int ABMPTreePacket::getBlacklist() const{
	return blacklist;
}

void ABMPTreePacket::setFirstch(int first_ch) {
	this->first_ch = first_ch;
}

int ABMPTreePacket::getFirstch() const{
	return first_ch;
}

void ABMPTreePacket::setB0error(int b0_error) {
	this->b0_error = b0_error;
}

int ABMPTreePacket::getB0error() const{
	return b0_error;
}

void ABMPTreePacket::setChangebl(int change_bl) {
	this->change_bl = change_bl;
}

int ABMPTreePacket::getChangebl() const{
	return change_bl;
}

void ABMPTreePacket::setBeaconDup(int beacon_dup) {
	this->beacon_dup = beacon_dup;
}

int ABMPTreePacket::getBeaconDup() const{
	return beacon_dup;
}

void ABMPTreePacket::setKindDSME(int kind_dsme) {
	this->kind_dsme = kind_dsme;
}

int ABMPTreePacket::getKindDSME() {
	return this->kind_dsme;
}

class ABMPTreePacketDescriptor : public cClassDescriptor
{
  public:
    ABMPTreePacketDescriptor();
    virtual ~ABMPTreePacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ABMPTreePacketDescriptor);

ABMPTreePacketDescriptor::ABMPTreePacketDescriptor() : cClassDescriptor("ABMPTreePacket", "MacPacket")
{
}

ABMPTreePacketDescriptor::~ABMPTreePacketDescriptor()
{
}

bool ABMPTreePacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ABMPTreePacket *>(obj)!=NULL;
}

const char *ABMPTreePacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ABMPTreePacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int ABMPTreePacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *ABMPTreePacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ackReq","kind_dsme,","ack_bitmap", "channel", "beacon_id", "blacklist", "first_ch", "b0_error", "change_bl", "beacon_dup"
    };

    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int ABMPTreePacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && (strcmp(fieldName, "ackReq")==0 || strcmp(fieldName, "ack_bitmap")==0 || strcmp(fieldName, "channel")==0 || strcmp(fieldName, "beacon_id")==0 || strcmp(fieldName, "blacklist")==0 || strcmp(fieldName, "first_ch")==0 || strcmp(fieldName,"b0_error")==0 || strcmp(fieldName,"change_bl") || strcmp(fieldName,"beacon_dup") )) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ABMPTreePacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *ABMPTreePacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "ABMPTreeFrameTypeDef";
            return NULL;
        default: return NULL;
    }
}

int ABMPTreePacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ABMPTreePacket *pp = (ABMPTreePacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ABMPTreePacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ABMPTreePacket *pp = (ABMPTreePacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAckReq());
        case 1: return long2string(pp->getKindDSME());
       // case 2: return long2string(pp->getAckBitmap());
        default: return "";
    }
}

bool ABMPTreePacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ABMPTreePacket *pp = (ABMPTreePacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setAckReq(string2long(value)); return true;
		case 1: pp->setKindDSME(string2long(value)); return true;
		//case 2: pp->setAckBitmap(string2long(value)); return true;
        default: return false;
    }
}

const char *ABMPTreePacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *ABMPTreePacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ABMPTreePacket *pp = (ABMPTreePacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


