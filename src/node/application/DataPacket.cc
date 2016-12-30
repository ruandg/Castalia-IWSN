//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/application/DataPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "DataPacket.h"
#include "ApplicationPacket_m.h"

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


Register_Class(DataPacket);

DataPacket::DataPacket(const char *name, int kind) : ::cPacket(name,kind)
{
    this->applicationID_var = 0;
    this->sequenceNumber_var = 0;
    //this->data_packet = 0;
}

DataPacket::DataPacket(const DataPacket& other) : ::cPacket(other)
{
    copy(other);
}

DataPacket::~DataPacket()
{
}

DataPacket& DataPacket::operator=(const DataPacket& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataPacket::copy(const DataPacket& other)
{
    this->appNetInfoExchange_var = other.appNetInfoExchange_var;
    this->applicationID_var = other.applicationID_var;
    this->sequenceNumber_var = other.sequenceNumber_var;
    this->data_packet = other.data_packet;
}

void DataPacket::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->appNetInfoExchange_var);
    doPacking(b,this->applicationID_var);
    doPacking(b,this->sequenceNumber_var);
    doPacking(b,this->data_packet);
}

void DataPacket::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->appNetInfoExchange_var);
    doUnpacking(b,this->applicationID_var);
    doUnpacking(b,this->sequenceNumber_var);
    doUnpacking(b,this->data_packet);
}

AppNetInfoExchange_type& DataPacket::getAppNetInfoExchange()
{
    return appNetInfoExchange_var;
}

void DataPacket::setAppNetInfoExchange(const AppNetInfoExchange_type& appNetInfoExchange)
{
    this->appNetInfoExchange_var = appNetInfoExchange;
}

const char * DataPacket::getApplicationID() const
{
    return applicationID_var.c_str();
}

void DataPacket::setApplicationID(const char * applicationID)
{
    this->applicationID_var = applicationID;
}

unsigned int DataPacket::getSequenceNumber() const
{
    return sequenceNumber_var;
}

void DataPacket::setSequenceNumber(unsigned int sequenceNumber)
{
    this->sequenceNumber_var = sequenceNumber;
}


double* DataPacket::getDataPacket() const
{
    return data_packet;
}

void DataPacket::setDataPacket(double* data)
{
    this->data_packet = data;
}

class DataPacketDescriptor : public cClassDescriptor
{
  public:
    DataPacketDescriptor();
    virtual ~DataPacketDescriptor();

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

Register_ClassDescriptor(DataPacketDescriptor);

DataPacketDescriptor::DataPacketDescriptor() : cClassDescriptor("DataPacket", "cPacket")
{
}

DataPacketDescriptor::~DataPacketDescriptor()
{
}

bool DataPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<DataPacket *>(obj)!=NULL;
}

const char *DataPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int DataPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int DataPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *DataPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "appNetInfoExchange",
        "applicationID",
        "sequenceNumber",
        "data",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int DataPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "appNetInfoExchange")==0) return base+0;
    if (fieldName[0]=='a' && strcmp(fieldName, "applicationID")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *DataPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "AppNetInfoExchange_type",
        "string",
        "unsigned int",
        "double",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *DataPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int DataPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string DataPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getAppNetInfoExchange(); return out.str();}
        case 1: return oppstring2string(pp->getApplicationID());
        case 2: return ulong2string(pp->getSequenceNumber());
        case 3: return double2string(pp->getDataPacket()[0]); //TODO: criar adouble2string
        default: return "";
    }
}

bool DataPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
	double v;
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        case 1: pp->setApplicationID((value)); return true;
        case 2: pp->setSequenceNumber(string2ulong(value)); return true;
        case 3: v = string2double(value); pp->setDataPacket(&v); return true; //TODO: criar string2Doublep
        default: return false;
    }
}

const char *DataPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0: return opp_typename(typeid(AppNetInfoExchange_type));
        default: return NULL;
    };
}

void *DataPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    DataPacket *pp = (DataPacket *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getAppNetInfoExchange()); break;
        default: return NULL;
    }
}


