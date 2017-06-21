#include <iostream>
#include <sstream>
#include "DSMEACKPacket.h"

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
    cEnum *e = cEnum::find("DSMEACKFrameTypeDef");
    if (!e) enums.getInstance()->add(e = new cEnum("DSMEACKFrameTypeDef"));
    e->insert(DATA_FRAME, "DATA_FRAME");
    e->insert(BEACON_FRAME, "BEACON_FRAME");
    e->insert(ACK_FRAME, "ACK_FRAME");
	
);

Register_Class(DSMEACKPacket);

DSMEACKPacket::DSMEACKPacket(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->ackReq = 0;
	this->ack_bitmap = 0;
	this->channel_bitmap = 0;
}

DSMEACKPacket::DSMEACKPacket(const DSMEACKPacket& other) : ::MacPacket(other)
{
    copy(other);
}

DSMEACKPacket::~DSMEACKPacket()
{
}

DSMEACKPacket& DSMEACKPacket::operator=(const DSMEACKPacket& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void DSMEACKPacket::copy(const DSMEACKPacket& other)
{
    this->ackReq = other.ackReq;
	this->ack_bitmap = other.ack_bitmap;
	this->kind_dsme = other.kind_dsme;
	this->channel_bitmap = other.channel_bitmap;
}

void DSMEACKPacket::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->ackReq);
}

void DSMEACKPacket::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->ackReq);
}

bool DSMEACKPacket::getAckReq() const
{
    return ackReq;
}

void DSMEACKPacket::setAckReq(bool ackReq)
{
    this->ackReq = ackReq;
}

int DSMEACKPacket::getAckBitmap() const
{
    return ack_bitmap;
}

void DSMEACKPacket::setAckBitmap(int ack_bitmap)
{
    this->ack_bitmap = ack_bitmap;
}

int DSMEACKPacket::getChannelBitmap() const
{
    return channel_bitmap;
}

void DSMEACKPacket::setChannelBitmap(int channel_bitmap)
{
    this->channel_bitmap = channel_bitmap;
}

void DSMEACKPacket::setKindDSME(int kind_dsme) {
	this->kind_dsme = kind_dsme;
}

int DSMEACKPacket::getKindDSME() {
	return this->kind_dsme;
}

class DSMEACKPacketDescriptor : public cClassDescriptor
{
  public:
    DSMEACKPacketDescriptor();
    virtual ~DSMEACKPacketDescriptor();

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

Register_ClassDescriptor(DSMEACKPacketDescriptor);

DSMEACKPacketDescriptor::DSMEACKPacketDescriptor() : cClassDescriptor("DSMEACKPacket", "MacPacket")
{
}

DSMEACKPacketDescriptor::~DSMEACKPacketDescriptor()
{
}

bool DSMEACKPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DSMEACKPacket *>(obj)!=NULL;
}

const char *DSMEACKPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DSMEACKPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int DSMEACKPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *DSMEACKPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ackReq","kind_dsme","ack_bitmap", "channel_bitmap"
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int DSMEACKPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && (strcmp(fieldName, "ackReq")==0 || strcmp(fieldName, "ack_bitmap")==0 || strcmp(fieldName, "channel_bitmap")==0)) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DSMEACKPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *DSMEACKPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "DSMEACKFrameTypeDef";
            return NULL;
        default: return NULL;
    }
}

int DSMEACKPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DSMEACKPacket *pp = (DSMEACKPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DSMEACKPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DSMEACKPacket *pp = (DSMEACKPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAckReq());
        case 1: return long2string(pp->getKindDSME());
        case 2: return long2string(pp->getAckBitmap());
		case 3: return long2string(pp->getChannelBitmap());
        default: return "";
    }
}

bool DSMEACKPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DSMEACKPacket *pp = (DSMEACKPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setAckReq(string2long(value)); return true;
		case 1: pp->setKindDSME(string2long(value)); return true;
		case 2: pp->setAckBitmap(string2long(value)); return true;
		case 3: pp->setChannelBitmap(string2long(value)); return true;
        default: return false;
    }
}

const char *DSMEACKPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *DSMEACKPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DSMEACKPacket *pp = (DSMEACKPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


