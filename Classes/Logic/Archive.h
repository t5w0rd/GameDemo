#ifndef __ARCHIVE_H__
#define	__ARCHIVE_H__


enum VALUE_TYPE
{
    kVtStr,
    kVtFLT,
    kVtINT,
    kVtUINT,
    kVtBOOL,
    kVtMAP
};

enum VALUE_TYPE_FLAG
{
    kVtfArrayElement = 1 << 16
};

enum VALUE_TYPE_BOOL
{
    kVtbFalse = 0x00,  // or 'f'
    kVtbTrue = 0x01  // or 't'
};


class CValue
{
public:
    CValue(int type);
    virtual ~CValue();

    void setType(int type);
    int getType() const;

protected:
    int m_type;
};

class CValueBase : public CValue
{
public:
    CValueBase(int type, const string& data);
    CValueBase(const string& s);
    CValueBase(double f);
    CValueBase(int n);
    CValueBase(bool b);

    void setValue(const string& data);
    string& getValue();
    const string& getValue() const;
    
protected:
    string m_baseData;
};

class CValueMap : public CValue
{
public:
    typedef unordered_map<string, CValue*> MAP_VALUE;

public:
    CValueMap(int type = kVtMAP);
    virtual ~CValueMap();

    void setValue(const string& name, CValue* data);
    void setArrayElement(int index, CValue* data);
    MAP_VALUE& getValue();
    const MAP_VALUE& getValue() const;

protected:
    MAP_VALUE m_mapData;
};

class CArchive
{
public:
    CArchive();
    ~CArchive();

    bool loadFromFile(const char* file);
    unsigned int saveToFile(const char* file);

    void setValue(const string& name, CValue* data);
    CValue* getValue(const string& name);

    void setValueMap(CValueMap* valueMap);
    CValueMap* getValueMap();
    const CValueMap* getValueMap() const;

protected:
    void readName(string& data);
    CValue* readValue(int type, unsigned int size);

    void writeName(const string& data);
    unsigned int writeValue(CValue* data);

protected:
    CValueMap* m_data;
    FILE* m_fp;
};


#endif
