#include <iostream>
#include <sstream>
#include "SimpleTSCHPacket.h"

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
    cEnum *e = cEnum::find("SimpleTSCHFrameTypeDef");
    if (!e) enums.getInstance()->add(e = new cEnum("SimpleTSCHFrameTypeDef"));
    e->insert(DATA_FRAME, "DATA_FRAME");
    e->insert(BEACON_FRAME, "BEACON_FRAME");
    e->insert(ACK_FRAME, "ACK_FRAME");
	
);

Register_Class(SimpleTSCHPacket);

SimpleTSCHPacket::SimpleTSCHPacket(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->ackReq = 0;
}

SimpleTSCHPacket::SimpleTSCHPacket(const SimpleTSCHPacket& other) : ::MacPacket(other)
{
    copy(other);
}

SimpleTSCHPacket::~SimpleTSCHPacket()
{
}

SimpleTSCHPacket& SimpleTSCHPacket::operator=(const SimpleTSCHPacket& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void SimpleTSCHPacket::copy(const SimpleTSCHPacket& other)
{
    this->ackReq = other.ackReq;
	this->kind_tsch = other.kind_tsch;
}

void SimpleTSCHPacket::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->ackReq);
}

void SimpleTSCHPacket::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->ackReq);
}

bool SimpleTSCHPacket::getAckReq() const
{
    return ackReq;
}

void SimpleTSCHPacket::setAckReq(bool ackReq)
{
    this->ackReq = ackReq;
}

void SimpleTSCHPacket::setKindTSCH(int kind_tsch) {
	this->kind_tsch = kind_tsch;
}

int SimpleTSCHPacket::getKindTSCH() {
	return this->kind_tsch;
}

class SimpleTSCHPacketDescriptor : public cClassDescriptor
{
  public:
    SimpleTSCHPacketDescriptor();
    virtual ~SimpleTSCHPacketDescriptor();

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

Register_ClassDescriptor(SimpleTSCHPacketDescriptor);

SimpleTSCHPacketDescriptor::SimpleTSCHPacketDescriptor() : cClassDescriptor("SimpleTSCHPacket", "MacPacket")
{
}

SimpleTSCHPacketDescriptor::~SimpleTSCHPacketDescriptor()
{
}

bool SimpleTSCHPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SimpleTSCHPacket *>(obj)!=NULL;
}

const char *SimpleTSCHPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SimpleTSCHPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SimpleTSCHPacketDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *SimpleTSCHPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "ackReq","kind_tsch"
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SimpleTSCHPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && (strcmp(fieldName, "ackReq")==0 || strcmp(fieldName, "kind_tsch")==0)) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SimpleTSCHPacketDescriptor::getFieldTypeString(void *object, int field) const
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

const char *SimpleTSCHPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "SimpleTSCHFrameTypeDef";
            return NULL;
        default: return NULL;
    }
}

int SimpleTSCHPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SimpleTSCHPacket *pp = (SimpleTSCHPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SimpleTSCHPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleTSCHPacket *pp = (SimpleTSCHPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAckReq());
        case 1: return long2string(pp->getKindTSCH());
        default: return "";
    }
}

bool SimpleTSCHPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SimpleTSCHPacket *pp = (SimpleTSCHPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setAckReq(string2long(value)); return true;
		case 1: pp->setKindTSCH(string2long(value)); return true;
        default: return false;
    }
}

const char *SimpleTSCHPacketDescriptor::getFieldStructName(void *object, int field) const
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

void *SimpleTSCHPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SimpleTSCHPacket *pp = (SimpleTSCHPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


