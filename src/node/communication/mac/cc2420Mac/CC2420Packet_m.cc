//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/mac/cc2420Mac/CC2420Packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CC2420Packet_m.h"

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
    cEnum *e = cEnum::find("CC2420Packet_type");
    if (!e) enums.getInstance()->add(e = new cEnum("CC2420Packet_type"));
);

Register_Class(CC2420Ieee802154Packet);

CC2420Ieee802154Packet::CC2420Ieee802154Packet(const char *name, int kind) : ::MacPacket(name,kind)
{
    this->fcf_var = 0;
    this->dsn_var = 0;
    this->fcs_var = 0;
    this->metaWasAcked_var = 0;
}

CC2420Ieee802154Packet::CC2420Ieee802154Packet(const CC2420Ieee802154Packet& other) : ::MacPacket(other)
{
    copy(other);
}

CC2420Ieee802154Packet::~CC2420Ieee802154Packet()
{
}

CC2420Ieee802154Packet& CC2420Ieee802154Packet::operator=(const CC2420Ieee802154Packet& other)
{
    if (this==&other) return *this;
    ::MacPacket::operator=(other);
    copy(other);
    return *this;
}

void CC2420Ieee802154Packet::copy(const CC2420Ieee802154Packet& other)
{
    this->fcf_var = other.fcf_var;
    this->dsn_var = other.dsn_var;
    this->fcs_var = other.fcs_var;
    this->metaWasAcked_var = other.metaWasAcked_var;
}

void CC2420Ieee802154Packet::parsimPack(cCommBuffer *b)
{
    ::MacPacket::parsimPack(b);
    doPacking(b,this->fcf_var);
    doPacking(b,this->dsn_var);
    doPacking(b,this->fcs_var);
    doPacking(b,this->metaWasAcked_var);
}

void CC2420Ieee802154Packet::parsimUnpack(cCommBuffer *b)
{
    ::MacPacket::parsimUnpack(b);
    doUnpacking(b,this->fcf_var);
    doUnpacking(b,this->dsn_var);
    doUnpacking(b,this->fcs_var);
    doUnpacking(b,this->metaWasAcked_var);
}

uint16_t CC2420Ieee802154Packet::getFcf() const
{
    return fcf_var;
}

void CC2420Ieee802154Packet::setFcf(uint16_t fcf)
{
    this->fcf_var = fcf;
}

uint8_t CC2420Ieee802154Packet::getDsn() const
{
    return dsn_var;
}

void CC2420Ieee802154Packet::setDsn(uint8_t dsn)
{
    this->dsn_var = dsn;
}

uint16_t CC2420Ieee802154Packet::getFcs() const
{
    return fcs_var;
}

void CC2420Ieee802154Packet::setFcs(uint16_t fcs)
{
    this->fcs_var = fcs;
}

bool CC2420Ieee802154Packet::getMetaWasAcked() const
{
    return metaWasAcked_var;
}

void CC2420Ieee802154Packet::setMetaWasAcked(bool metaWasAcked)
{
    this->metaWasAcked_var = metaWasAcked;
}

class CC2420Ieee802154PacketDescriptor : public cClassDescriptor
{
  public:
    CC2420Ieee802154PacketDescriptor();
    virtual ~CC2420Ieee802154PacketDescriptor();

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

Register_ClassDescriptor(CC2420Ieee802154PacketDescriptor);

CC2420Ieee802154PacketDescriptor::CC2420Ieee802154PacketDescriptor() : cClassDescriptor("CC2420Ieee802154Packet", "MacPacket")
{
}

CC2420Ieee802154PacketDescriptor::~CC2420Ieee802154PacketDescriptor()
{
}

bool CC2420Ieee802154PacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CC2420Ieee802154Packet *>(obj)!=NULL;
}

const char *CC2420Ieee802154PacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CC2420Ieee802154PacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int CC2420Ieee802154PacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *CC2420Ieee802154PacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "fcf",
        "dsn",
        "fcs",
        "metaWasAcked",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int CC2420Ieee802154PacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fcf")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dsn")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "fcs")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "metaWasAcked")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CC2420Ieee802154PacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "uint16_t",
        "uint8_t",
        "uint16_t",
        "bool",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *CC2420Ieee802154PacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CC2420Ieee802154PacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Ieee802154Packet *pp = (CC2420Ieee802154Packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CC2420Ieee802154PacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Ieee802154Packet *pp = (CC2420Ieee802154Packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getFcf());
        case 1: return ulong2string(pp->getDsn());
        case 2: return ulong2string(pp->getFcs());
        case 3: return bool2string(pp->getMetaWasAcked());
        default: return "";
    }
}

bool CC2420Ieee802154PacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Ieee802154Packet *pp = (CC2420Ieee802154Packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setFcf(string2ulong(value)); return true;
        case 1: pp->setDsn(string2ulong(value)); return true;
        case 2: pp->setFcs(string2ulong(value)); return true;
        case 3: pp->setMetaWasAcked(string2bool(value)); return true;
        default: return false;
    }
}

const char *CC2420Ieee802154PacketDescriptor::getFieldStructName(void *object, int field) const
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

void *CC2420Ieee802154PacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Ieee802154Packet *pp = (CC2420Ieee802154Packet *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CC2420Packet);

CC2420Packet::CC2420Packet(const char *name, int kind) : ::CC2420Ieee802154Packet(name,kind)
{
    this->destpan_var = 0;
    this->dest_var = 0;
    this->src_var = 0;
    this->type_var = 0;
}

CC2420Packet::CC2420Packet(const CC2420Packet& other) : ::CC2420Ieee802154Packet(other)
{
    copy(other);
}

CC2420Packet::~CC2420Packet()
{
}

CC2420Packet& CC2420Packet::operator=(const CC2420Packet& other)
{
    if (this==&other) return *this;
    ::CC2420Ieee802154Packet::operator=(other);
    copy(other);
    return *this;
}

void CC2420Packet::copy(const CC2420Packet& other)
{
    this->destpan_var = other.destpan_var;
    this->dest_var = other.dest_var;
    this->src_var = other.src_var;
    this->type_var = other.type_var;
}

void CC2420Packet::parsimPack(cCommBuffer *b)
{
    ::CC2420Ieee802154Packet::parsimPack(b);
    doPacking(b,this->destpan_var);
    doPacking(b,this->dest_var);
    doPacking(b,this->src_var);
    doPacking(b,this->type_var);
}

void CC2420Packet::parsimUnpack(cCommBuffer *b)
{
    ::CC2420Ieee802154Packet::parsimUnpack(b);
    doUnpacking(b,this->destpan_var);
    doUnpacking(b,this->dest_var);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->type_var);
}

uint16_t CC2420Packet::getDestpan() const
{
    return destpan_var;
}

void CC2420Packet::setDestpan(uint16_t destpan)
{
    this->destpan_var = destpan;
}

uint16_t CC2420Packet::getDest() const
{
    return dest_var;
}

void CC2420Packet::setDest(uint16_t dest)
{
    this->dest_var = dest;
}

uint16_t CC2420Packet::getSrc() const
{
    return src_var;
}

void CC2420Packet::setSrc(uint16_t src)
{
    this->src_var = src;
}

uint8_t CC2420Packet::getType() const
{
    return type_var;
}

void CC2420Packet::setType(uint8_t type)
{
    this->type_var = type;
}

class CC2420PacketDescriptor : public cClassDescriptor
{
  public:
    CC2420PacketDescriptor();
    virtual ~CC2420PacketDescriptor();

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

Register_ClassDescriptor(CC2420PacketDescriptor);

CC2420PacketDescriptor::CC2420PacketDescriptor() : cClassDescriptor("CC2420Packet", "CC2420Ieee802154Packet")
{
}

CC2420PacketDescriptor::~CC2420PacketDescriptor()
{
}

bool CC2420PacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CC2420Packet *>(obj)!=NULL;
}

const char *CC2420PacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CC2420PacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int CC2420PacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *CC2420PacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "destpan",
        "dest",
        "src",
        "type",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int CC2420PacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destpan")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dest")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "type")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CC2420PacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "uint16_t",
        "uint16_t",
        "uint16_t",
        "uint8_t",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *CC2420PacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CC2420PacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Packet *pp = (CC2420Packet *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CC2420PacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Packet *pp = (CC2420Packet *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getDestpan());
        case 1: return ulong2string(pp->getDest());
        case 2: return ulong2string(pp->getSrc());
        case 3: return ulong2string(pp->getType());
        default: return "";
    }
}

bool CC2420PacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Packet *pp = (CC2420Packet *)object; (void)pp;
    switch (field) {
        case 0: pp->setDestpan(string2ulong(value)); return true;
        case 1: pp->setDest(string2ulong(value)); return true;
        case 2: pp->setSrc(string2ulong(value)); return true;
        case 3: pp->setType(string2ulong(value)); return true;
        default: return false;
    }
}

const char *CC2420PacketDescriptor::getFieldStructName(void *object, int field) const
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

void *CC2420PacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420Packet *pp = (CC2420Packet *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CC2420AckPacket);

CC2420AckPacket::CC2420AckPacket(const char *name, int kind) : ::CC2420Ieee802154Packet(name,kind)
{
}

CC2420AckPacket::CC2420AckPacket(const CC2420AckPacket& other) : ::CC2420Ieee802154Packet(other)
{
    copy(other);
}

CC2420AckPacket::~CC2420AckPacket()
{
}

CC2420AckPacket& CC2420AckPacket::operator=(const CC2420AckPacket& other)
{
    if (this==&other) return *this;
    ::CC2420Ieee802154Packet::operator=(other);
    copy(other);
    return *this;
}

void CC2420AckPacket::copy(const CC2420AckPacket& other)
{
}

void CC2420AckPacket::parsimPack(cCommBuffer *b)
{
    ::CC2420Ieee802154Packet::parsimPack(b);
}

void CC2420AckPacket::parsimUnpack(cCommBuffer *b)
{
    ::CC2420Ieee802154Packet::parsimUnpack(b);
}

class CC2420AckPacketDescriptor : public cClassDescriptor
{
  public:
    CC2420AckPacketDescriptor();
    virtual ~CC2420AckPacketDescriptor();

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

Register_ClassDescriptor(CC2420AckPacketDescriptor);

CC2420AckPacketDescriptor::CC2420AckPacketDescriptor() : cClassDescriptor("CC2420AckPacket", "CC2420Ieee802154Packet")
{
}

CC2420AckPacketDescriptor::~CC2420AckPacketDescriptor()
{
}

bool CC2420AckPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CC2420AckPacket *>(obj)!=NULL;
}

const char *CC2420AckPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CC2420AckPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount(object) : 0;
}

unsigned int CC2420AckPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return 0;
}

const char *CC2420AckPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

int CC2420AckPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CC2420AckPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

const char *CC2420AckPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int CC2420AckPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CC2420AckPacket *pp = (CC2420AckPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CC2420AckPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420AckPacket *pp = (CC2420AckPacket *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CC2420AckPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CC2420AckPacket *pp = (CC2420AckPacket *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CC2420AckPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    return NULL;
}

void *CC2420AckPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420AckPacket *pp = (CC2420AckPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CC2420MacControlMessage);

CC2420MacControlMessage::CC2420MacControlMessage(const char *name, int kind) : ::MacControlMessage(name,kind)
{
    this->mcmType_var = 0;
    this->mcmInterface_var = 0;
    this->mcmEvent_var = 0;
    this->mcmCommand_var = 0;
    this->error_var = 0;
    this->useCca_var = 0;
    this->metaWasAcked_var = 0;
    this->metaDest_var = 0;
}

CC2420MacControlMessage::CC2420MacControlMessage(const CC2420MacControlMessage& other) : ::MacControlMessage(other)
{
    copy(other);
}

CC2420MacControlMessage::~CC2420MacControlMessage()
{
}

CC2420MacControlMessage& CC2420MacControlMessage::operator=(const CC2420MacControlMessage& other)
{
    if (this==&other) return *this;
    ::MacControlMessage::operator=(other);
    copy(other);
    return *this;
}

void CC2420MacControlMessage::copy(const CC2420MacControlMessage& other)
{
    this->mcmType_var = other.mcmType_var;
    this->mcmInterface_var = other.mcmInterface_var;
    this->mcmEvent_var = other.mcmEvent_var;
    this->mcmCommand_var = other.mcmCommand_var;
    this->error_var = other.error_var;
    this->useCca_var = other.useCca_var;
    this->metaWasAcked_var = other.metaWasAcked_var;
    this->metaDest_var = other.metaDest_var;
}

void CC2420MacControlMessage::parsimPack(cCommBuffer *b)
{
    ::MacControlMessage::parsimPack(b);
    doPacking(b,this->mcmType_var);
    doPacking(b,this->mcmInterface_var);
    doPacking(b,this->mcmEvent_var);
    doPacking(b,this->mcmCommand_var);
    doPacking(b,this->error_var);
    doPacking(b,this->useCca_var);
    doPacking(b,this->metaWasAcked_var);
    doPacking(b,this->metaDest_var);
}

void CC2420MacControlMessage::parsimUnpack(cCommBuffer *b)
{
    ::MacControlMessage::parsimUnpack(b);
    doUnpacking(b,this->mcmType_var);
    doUnpacking(b,this->mcmInterface_var);
    doUnpacking(b,this->mcmEvent_var);
    doUnpacking(b,this->mcmCommand_var);
    doUnpacking(b,this->error_var);
    doUnpacking(b,this->useCca_var);
    doUnpacking(b,this->metaWasAcked_var);
    doUnpacking(b,this->metaDest_var);
}

int CC2420MacControlMessage::getMcmType() const
{
    return mcmType_var;
}

void CC2420MacControlMessage::setMcmType(int mcmType)
{
    this->mcmType_var = mcmType;
}

int CC2420MacControlMessage::getMcmInterface() const
{
    return mcmInterface_var;
}

void CC2420MacControlMessage::setMcmInterface(int mcmInterface)
{
    this->mcmInterface_var = mcmInterface;
}

int CC2420MacControlMessage::getMcmEvent() const
{
    return mcmEvent_var;
}

void CC2420MacControlMessage::setMcmEvent(int mcmEvent)
{
    this->mcmEvent_var = mcmEvent;
}

int CC2420MacControlMessage::getMcmCommand() const
{
    return mcmCommand_var;
}

void CC2420MacControlMessage::setMcmCommand(int mcmCommand)
{
    this->mcmCommand_var = mcmCommand;
}

uint8_t CC2420MacControlMessage::getError() const
{
    return error_var;
}

void CC2420MacControlMessage::setError(uint8_t error)
{
    this->error_var = error;
}

bool CC2420MacControlMessage::getUseCca() const
{
    return useCca_var;
}

void CC2420MacControlMessage::setUseCca(bool useCca)
{
    this->useCca_var = useCca;
}

bool CC2420MacControlMessage::getMetaWasAcked() const
{
    return metaWasAcked_var;
}

void CC2420MacControlMessage::setMetaWasAcked(bool metaWasAcked)
{
    this->metaWasAcked_var = metaWasAcked;
}

uint16_t CC2420MacControlMessage::getMetaDest() const
{
    return metaDest_var;
}

void CC2420MacControlMessage::setMetaDest(uint16_t metaDest)
{
    this->metaDest_var = metaDest;
}

class CC2420MacControlMessageDescriptor : public cClassDescriptor
{
  public:
    CC2420MacControlMessageDescriptor();
    virtual ~CC2420MacControlMessageDescriptor();

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

Register_ClassDescriptor(CC2420MacControlMessageDescriptor);

CC2420MacControlMessageDescriptor::CC2420MacControlMessageDescriptor() : cClassDescriptor("CC2420MacControlMessage", "MacControlMessage")
{
}

CC2420MacControlMessageDescriptor::~CC2420MacControlMessageDescriptor()
{
}

bool CC2420MacControlMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CC2420MacControlMessage *>(obj)!=NULL;
}

const char *CC2420MacControlMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CC2420MacControlMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount(object) : 8;
}

unsigned int CC2420MacControlMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *CC2420MacControlMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "mcmType",
        "mcmInterface",
        "mcmEvent",
        "mcmCommand",
        "error",
        "useCca",
        "metaWasAcked",
        "metaDest",
    };
    return (field>=0 && field<8) ? fieldNames[field] : NULL;
}

int CC2420MacControlMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "mcmType")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "mcmInterface")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "mcmEvent")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "mcmCommand")==0) return base+3;
    if (fieldName[0]=='e' && strcmp(fieldName, "error")==0) return base+4;
    if (fieldName[0]=='u' && strcmp(fieldName, "useCca")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "metaWasAcked")==0) return base+6;
    if (fieldName[0]=='m' && strcmp(fieldName, "metaDest")==0) return base+7;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CC2420MacControlMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "uint8_t",
        "bool",
        "bool",
        "uint16_t",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : NULL;
}

const char *CC2420MacControlMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "Notification_type";
            return NULL;
        case 1:
            if (!strcmp(propertyname,"enum")) return "Notification_interface";
            return NULL;
        case 2:
            if (!strcmp(propertyname,"enum")) return "Notification_event";
            return NULL;
        case 3:
            if (!strcmp(propertyname,"enum")) return "Notification_command";
            return NULL;
        case 4:
            if (!strcmp(propertyname,"enum")) return "tos_err_types";
            return NULL;
        default: return NULL;
    }
}

int CC2420MacControlMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CC2420MacControlMessage *pp = (CC2420MacControlMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CC2420MacControlMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420MacControlMessage *pp = (CC2420MacControlMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMcmType());
        case 1: return long2string(pp->getMcmInterface());
        case 2: return long2string(pp->getMcmEvent());
        case 3: return long2string(pp->getMcmCommand());
        case 4: return ulong2string(pp->getError());
        case 5: return bool2string(pp->getUseCca());
        case 6: return bool2string(pp->getMetaWasAcked());
        case 7: return ulong2string(pp->getMetaDest());
        default: return "";
    }
}

bool CC2420MacControlMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CC2420MacControlMessage *pp = (CC2420MacControlMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setMcmType(string2long(value)); return true;
        case 1: pp->setMcmInterface(string2long(value)); return true;
        case 2: pp->setMcmEvent(string2long(value)); return true;
        case 3: pp->setMcmCommand(string2long(value)); return true;
        case 4: pp->setError(string2ulong(value)); return true;
        case 5: pp->setUseCca(string2bool(value)); return true;
        case 6: pp->setMetaWasAcked(string2bool(value)); return true;
        case 7: pp->setMetaDest(string2ulong(value)); return true;
        default: return false;
    }
}

const char *CC2420MacControlMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *CC2420MacControlMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CC2420MacControlMessage *pp = (CC2420MacControlMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


