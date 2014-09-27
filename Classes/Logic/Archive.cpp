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

const string& CValueBase::toString() const
{
    return m_baseData;
}

double CValueBase::toFloat() const
{
    double v = 0.0;
    sscanf(m_baseData.c_str(), "%lf", &v);
    return v;
}

int CValueBase::toInteger() const
{
    return atoi(m_baseData.c_str());
}

bool CValueBase::toBoolean() const
{
    return m_baseData.at(0) == kVtbTrue;
}

void CValueBase::setData(const string& data)
{
    m_baseData = data;
}

string& CValueBase::getData()
{
    return m_baseData;
}

const string& CValueBase::getData() const
{
    return m_baseData;
}

// CValueMap
CValueMap::CValueMap(int type /*= kVtMAP*/)
: CValue(type)
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

void CValueMap::setMapValue(const string& name, CValue* data)
{
    m_mapData[name] = data;
}

CValue* CValueMap::getMapValue(const string& name) const
{
    auto it = m_mapData.find(name);
    return it == m_mapData.end() ? nullptr : it->second;
}

void CValueMap::setArrayValue(int index, CValue* data)
{
    char name[32];
    sprintf(name, "%d", index);
    data->setType(data->getType() | kVtfArrayElement);
    m_mapData[name] = data;
}

CValue* CValueMap::getArrayValue(int index) const
{
    char name[32];
    sprintf(name, "%d", index);
    auto it = m_mapData.find(name);
    return it == m_mapData.end() ? nullptr : it->second;
}

CValueMap::MAP_VALUE& CValueMap::getDataMap()
{
    return m_mapData;
}

const CValueMap::MAP_VALUE& CValueMap::getDataMap() const
{
    return m_mapData;
}

// CArchive
bool CArchive::loadValue(const char* file, CValue*& val)
{
    auto fp = fopen(file, "rb");
    if (fp == nullptr)
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    auto fsz = (unsigned int)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    auto size = readValue(fp, val);

    fclose(fp);

    return true;
}

bool CArchive::saveValue(const char* file, CValue* val)
{
    auto fp = fopen(file, "wb+");
    if (fp == nullptr)
    {
        return false;
    }

    auto size = writeValue(fp, val);

    auto fsz = ftell(fp);

    fclose(fp);

    return true;
}

unsigned int CArchive::readName(FILE* fp, string& name)
{
    unsigned char len = 0;
    char buf[256];
    fread(&len, sizeof(len), 1, fp);
    fread(buf, sizeof(char), len, fp);
    name.assign(buf, len);
    return sizeof(len) + len;
}

// 该函数返回读取的总大小
unsigned int CArchive::readValue(FILE* fp, CValue*& val)
{
    // 4Byte 的 ValueType
    int type = 0;
    fread(&type, sizeof(type), 1, fp);
    
    // 4Byte 的 ValueSize
    unsigned int size = 0;
    fread(&size, sizeof(size), 1, fp);

    switch (type & 0x0000FFFF)
    {
    default:
        {
            
            char* buf = new char[size];
            fread(buf, sizeof(char), size, fp);
            CValueBase* v = new CValueBase(type, string(buf, size));
            delete[] buf;
            val = v;
        }
        break;

    case kVtMAP:
        {
            CValueMap* v = new CValueMap(type);

            unsigned left = size;
            while (left > 0)
            {
                string eln;
                left -= readName(fp, eln);

                CValue* elv = nullptr;
                left -= readValue(fp, elv);

                v->setMapValue(eln, elv);
            }

            val = v;
        }
        break;
    }

    return sizeof(type) + sizeof(size) + size;
}

unsigned int CArchive::writeName(FILE* fp, const string& data)
{
    unsigned char len = (unsigned char)data.size();
    fwrite(&len, sizeof(len), 1, fp);
    fwrite(data.c_str(), sizeof(char), data.size(), fp);
    return sizeof(len) + len;
}

unsigned int CArchive::writeValue(FILE* fp, CValue* data)
{
    int type = data->getType();
    fwrite(&type, sizeof(type), 1, fp);

    unsigned int size = 0;
    fwrite(&size, sizeof(size), 1, fp);

    switch (type & 0x0000FFFF)
    {
    default:
        {
            auto db = DCAST(data, CValueBase*);
            auto& buf = db->getData();
            size = (unsigned int)buf.size();
            fwrite(buf.c_str(), sizeof(char), size, fp);
        }
        break;

    case kVtMAP:
        {
            auto& dm = DCAST(data, CValueMap*)->getDataMap();
            M_MAP_FOREACH(dm)
            {
                auto& eln = M_MAP_EACH_KEY;
                auto elv = M_MAP_EACH;
                M_MAP_NEXT;

                size += writeName(fp, eln);

                size += writeValue(fp, elv);
            }
        }
        break;

    }

    fseek(fp, (int)(0 - size - sizeof(size)), SEEK_CUR);
    fwrite(&size, sizeof(size), 1, fp);
    fseek(fp, size, SEEK_CUR);

    return sizeof(type) + sizeof(size) + size;
}

void CArchive::luaPushValue(lua_State* L, CValue* val)
{
    int type = val->getType();

    switch (type & 0x0000FFFF)
    {
    case kVtStr:
        {
            auto& d = DCAST(val, CValueBase*)->getData();
            lua_pushlstring(L, d.c_str(), d.size());
        }
        break;

    case kVtFLT:
    case kVtINT:
    case kVtUINT:
        {
            auto& d = DCAST(val, CValueBase*)->getData();
            lua_pushnumber(L, lua_str2number(d.c_str(), nullptr));
        }
        break;

    case kVtBOOL:
        {
            auto& d = DCAST(val, CValueBase*)->getData();
            lua_pushboolean(L, d.at(0) == kVtbFalse ? false : true);
        }
        break;

    case kVtMAP:
        {
            lua_newtable(L);

            auto& vm = DCAST(val, CValueMap*)->getDataMap();
            M_MAP_FOREACH(vm)
            {
                auto& eln = M_MAP_EACH_KEY;
                auto eld = M_MAP_EACH;
                M_MAP_NEXT;

                if (eld->getType() & kVtfArrayElement)
                {
                    lua_pushnumber(L, atof(eln.c_str()));
                }
                else
                {
                    lua_pushlstring(L, eln.c_str(), eln.size());
                }
                
                luaPushValue(L, eld);
                lua_settable(L, -3);
            }
        }
        break;

    default:
        assert(false);
    }
}

CValue* CArchive::luaToValue(lua_State* L, int index)
{
    auto type = lua_type(L, index);
    switch (type)
    {
    case LUA_TSTRING:
        {
            size_t len = 0;
            auto buf = lua_tolstring(L, index, &len);
            return new CValueBase(string(buf, len));
        }
        break;

    case LUA_TNUMBER:
        {
            auto num = lua_tonumber(L, index);
            char buf[32];
            lua_number2str(buf, num);
            return new CValueBase((lua_Number)(lua_Integer)num == num ? kVtINT : kVtFLT, buf);
        }
        break;

    case LUA_TBOOLEAN:
        {
            bool b = lua_toboolean(L, index) != 0;
            return new CValueBase(b);
        }
        break;

    case LUA_TTABLE:
        {
            auto val = new CValueMap();

            lua_pushnil(L);  /* first key */
            while (lua_next(L, index) != 0)
            {
                /* uses 'key' (at index -2) and 'value' (at index -1) */
                if (lua_isnumber(L, -2))
                {
                    int k = (int)lua_tonumber(L, -2);
                    auto v = luaToValue(L, lua_gettop(L));
                    if (v != nullptr)
                    {
                        v->setType(v->getType() | kVtfArrayElement);
                        val->setArrayValue(k, v);
                    }
                }
                else if (lua_isstring(L, -2))
                {
                    size_t len = 0;
                    auto k = lua_tolstring(L, -2, &len);
                    auto v = luaToValue(L, lua_gettop(L));
                    if (v != nullptr)
                    {
                        val->setMapValue(string(k, len), v);
                    }
                }
                else
                {
                    assert(false);
                }

                /* removes 'value'; keeps 'key' for next iteration */
                lua_pop(L, 1);
            }

            return val;
        }
        break;
    }

    return nullptr;
}
