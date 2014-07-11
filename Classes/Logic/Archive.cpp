/*
* File:   Archive.cpp
* Author: thunderliu
*
* Created on 2014年7月7日, 下午20:11
*/


#include "CommInc.h"
#include "Archive.h"


// CValue
CValue::CValue(int type)
: m_type(type)
{
}

CValue::~CValue()
{
}

void CValue::setType(int type)
{
    m_type = type;
}

int CValue::getType() const
{
    return m_type;
}

// CValueBase
CValueBase::CValueBase(int type, const string& data)
: CValue(type)
, m_baseData(data)
{
}

CValueBase::CValueBase(const string& s)
: CValue(kVtStr)
, m_baseData(s)
{
}

CValueBase::CValueBase(double f)
: CValue(kVtFLT)
{
    char buf[32];
    sprintf(buf, "%.14g", f);
    m_baseData = buf;
}

CValueBase::CValueBase(int n)
: CValue(kVtINT)
{
    char buf[32];
    sprintf(buf, "%d", n);
    m_baseData = buf;
}

CValueBase::CValueBase(bool b)
: CValue(kVtBOOL)
, m_baseData(string(1, b ? kVtbTrue : kVtbFalse))
{
}

void CValueBase::setValue(const string& data)
{
    m_baseData = data;
}

string& CValueBase::getValue()
{
    return m_baseData;
}

const string& CValueBase::getValue() const
{
    return m_baseData;
}

// CValueMap
CValueMap::CValueMap()
: CValue(kVtMAP)
{
}

CValueMap::~CValueMap()
{
    M_MAP_FOREACH(m_mapData)
    {
        CValue* el = M_MAP_EACH;
        M_MAP_NEXT;

        delete el;
    }
}

void CValueMap::setValue(const string& name, CValue* data)
{
    m_mapData[name] = data;
}

void CValueMap::setArrayElement(int index, CValue* data)
{
    char name[32];
    sprintf(name, "%d", index);
    data->setType(data->getType() | kVtfArrayElement);
    m_mapData[name] = data;
}

CValueMap::MAP_VALUE& CValueMap::getValue()
{
    return m_mapData;
}

const CValueMap::MAP_VALUE& CValueMap::getValue() const
{
    return m_mapData;
}

// CArchive
CArchive::CArchive()
: m_data(nullptr)
, m_fp(nullptr)
{
}

CArchive::~CArchive()
{
    if (m_data != nullptr)
    {
        delete m_data;
    }
}

bool CArchive::loadFromFile(const char* file)
{
    m_fp = fopen(file, "rb");
    if (m_fp == nullptr)
    {
        return false;
    }

    fseek(m_fp, 0, SEEK_END);
    auto size = (unsigned int)ftell(m_fp);
    fseek(m_fp, 0, SEEK_SET);

    if (m_data != nullptr)
    {
        delete m_data;
    }
    m_data = DCAST(readValue(kVtMAP, size), CValueMap*);

    fclose(m_fp);
    m_fp = nullptr;

    return true;
}

unsigned int CArchive::saveToFile(const char* file)
{
    m_fp = fopen(file, "wb+");

    auto size = writeValue(m_data);

    fclose(m_fp);
    m_fp = nullptr;

    return size;
}

void CArchive::setValue(const string& name, CValue* data)
{
    if (m_data == nullptr)
    {
        m_data = new CValueMap();
    }

    m_data->setValue(name, data);
}

CValue* CArchive::getValue(const string& name)
{
    if (m_data == nullptr)
    {
        return nullptr;
    }

    auto it = m_data->getValue().find(name);
    if (it == m_data->getValue().end())
    {
        return nullptr;
    }

    return it->second;
}

void CArchive::setValueMap(CValueMap* valueMap)
{
    if (m_data == valueMap)
    {
        return;
    }

    if (m_data != nullptr)
    {
        delete m_data;
    }

    m_data = valueMap;
}

CValueMap* CArchive::getValueMap()
{
    return m_data;
}

const CValueMap* CArchive::getValueMap() const
{
    return m_data;
}

void CArchive::readName(string& data)
{
    unsigned char len = 0;
    char buf[256];
    fread(&len, sizeof(len), 1, m_fp);
    fread(buf, sizeof(char), len, m_fp);
    data.assign(buf, len);
}

CValue* CArchive::readValue(int type, unsigned int size)
{
    switch (type & 0x0000FFFF)
    {
    case kVtStr:
    case kVtFLT:
    case kVtINT:
    case kVtUINT:
    case kVtBOOL:
        {
            
            char* buf = new char[size];
            fread(buf, sizeof(char), size, m_fp);
            CValueBase* ret = new CValueBase(type, string(buf, size));
            delete[] buf;
            return ret;
        }
        break;

    case kVtMAP:
        {
            CValueMap* ret = new CValueMap();
            while (size > 0)
            {
                string eln;
                readName(eln);

                int elt;
                fread(&elt, sizeof(elt), 1, m_fp);
                
                unsigned int elsz;
                fread(&elsz, sizeof(elsz), 1, m_fp);
                
                CValue* eld = readValue(elt, elsz);

                ret->setValue(eln, eld);

                size -= sizeof(char) + eln.size() + sizeof(elt) + sizeof(elsz) + elsz;
            }
            return ret;
        }
        break;
    }

    return nullptr;
}

void CArchive::writeName(const string& data)
{
    unsigned char len = (unsigned char)data.size();
    fwrite(&len, sizeof(len), 1, m_fp);
    fwrite(data.c_str(), sizeof(char), data.size(), m_fp);
}

unsigned int CArchive::writeValue(CValue* data)
{
    int type = data->getType();
    unsigned int size = 0;

    switch (type & 0x0000FFFF)
    {
    case kVtStr:
    case kVtFLT:
    case kVtINT:
    case kVtUINT:
    case kVtBOOL:
        {
            auto db = DCAST(data, CValueBase*);
            auto& buf = db->getValue();
            size = (unsigned int)buf.size();
            fwrite(buf.c_str(), sizeof(char), size, m_fp);
            return size;
        }
        break;

    case kVtMAP:
        {
            auto& dm = DCAST(data, CValueMap*)->getValue();
            M_MAP_FOREACH(dm)
            {
                auto& eln = M_MAP_IT->first;
                auto eld = M_MAP_EACH;
                M_MAP_NEXT;

                writeName(eln);

                int elt = eld->getType();
                fwrite(&elt, sizeof(elt), 1, m_fp);

                unsigned int elsz = 0;
                fwrite(&elsz, sizeof(elsz), 1, m_fp);
                elsz = writeValue(eld);
                
                fseek(m_fp, (int)(0 - elsz - sizeof(elsz)), SEEK_CUR);
                fwrite(&elsz, sizeof(elsz), 1, m_fp);
                fseek(m_fp, elsz, SEEK_CUR);

                size += sizeof(char) + eln.size() + sizeof(type) + sizeof(elsz) + elsz;
            }
            return size;
        }
        break;

    }

    return size;
}
