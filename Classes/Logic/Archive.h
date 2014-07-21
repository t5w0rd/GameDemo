#ifndef __ARCHIVE_H__
#define	__ARCHIVE_H__


enum VALUE_TYPE
{
    kVtRaw,
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

    const string& toString() const;
    double toFloat() const;
    int toInteger() const;
    bool toBoolean() const;

    void setData(const string& data);
    string& getData();
    const string& getData() const;
    
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

    void setMapValue(const string& name, CValue* data);
    CValue* getMapValue(const string& name) const;

    void setArrayValue(int index, CValue* data);
    CValue* getArrayValue(int index) const;

    MAP_VALUE& getDataMap();
    const MAP_VALUE& getDataMap() const;

protected:
    MAP_VALUE m_mapData;
};

class CArchive
{
public:
    static bool loadValue(const char* file, CValue*& val);
    static bool saveValue(const char* file, CValue* val);

    static void luaPushValue(lua_State* L, CValue* val);
    static CValue* luaToValue(lua_State* L, int index);

protected:
    static unsigned int readName(FILE* fp, string& name);
    static unsigned int readValue(FILE* fp, CValue*& val);

    static unsigned int writeName(FILE* fp, const string& name);
    static unsigned int writeValue(FILE* fp, CValue* val);

};


// fast cast, set functions, unsafe
void vms_v(CValue* vm, const string& nm, CValue* v);
void vms_v(CValue* vm, int idx, CValue* v);

void vms_str(CValue* vm, const string& nm, const string& str);
void vms_str(CValue* vm, int idx, const string& str);

void vms_flt(CValue* vm, const string& nm, double flt);
void vms_flt(CValue* vm, int idx, double flt);

void vms_int(CValue* vm, const string& nm, int integer);
void vms_int(CValue* vm, int idx, int integer);

void vms_bool(CValue* vm, const string& nm, bool boolean);
void vms_bool(CValue* vm, int idx, bool boolean);


// fast cast, get functions, unsafe
CValue* vmg_v(CValue* vm, const string& nm);
CValue* vmg_v(CValue* vm, int idx);

const string& vmg_str(CValue* vm, const string& nm);
const string& vmg_str(CValue* vm, int idx);

double vmg_flt(CValue* vm, const string& nm);
double vmg_flt(CValue* vm, int idx);

int vmg_int(CValue* vm, const string& nm);
int vmg_int(CValue* vm, int idx);

bool vmg_bool(CValue* vm, const string& nm);
bool vmg_bool(CValue* vm, int idx);

CValueMap* vmg_vm(CValue* vm, const string& nm);
CValueMap* vmg_vm(CValue* vm, int idx);










//////////////////////////////// Inline ////////////////////////////////

inline void vms_v(CValue* vm, const string& nm, CValue* v)
{
    ((CValueMap*)vm)->setMapValue(nm, v);
}

inline void vms_v(CValue* vm, int idx, CValue* v)
{
    ((CValueMap*)vm)->setArrayValue(idx, v);
}

inline void vms_str(CValue* vm, const string& nm, const string& str)
{
    ((CValueMap*)vm)->setMapValue(nm, new CValueBase(str));
}

inline void vms_str(CValue* vm, int idx, const string& str)
{
    ((CValueMap*)vm)->setArrayValue(idx, new CValueBase(str));
}

inline void vms_flt(CValue* vm, const string& nm, double flt)
{
    ((CValueMap*)vm)->setMapValue(nm, new CValueBase(flt));
}

inline void vms_flt(CValue* vm, int idx, double flt)
{
    ((CValueMap*)vm)->setArrayValue(idx, new CValueBase(flt));
}

inline void vms_int(CValue* vm, const string& nm, int integer)
{
    ((CValueMap*)vm)->setMapValue(nm, new CValueBase(integer));
}

inline void vms_int(CValue* vm, int idx, int integer)
{
    ((CValueMap*)vm)->setArrayValue(idx, new CValueBase(integer));
}

inline void vms_bool(CValue* vm, const string& nm, bool boolean)
{
    ((CValueMap*)vm)->setMapValue(nm, new CValueBase(boolean));
}

inline void vms_bool(CValue* vm, int idx, bool boolean)
{
    ((CValueMap*)vm)->setArrayValue(idx, new CValueBase(boolean));
}

inline CValue* vmg_v(CValue* vm, const string& nm)
{
    return ((CValueMap*)vm)->getMapValue(nm);
}

inline CValue* vmg_v(CValue* vm, int idx)
{
    return ((CValueMap*)vm)->getArrayValue(idx);
}

inline const string& vmg_str(CValue* vm, const string& nm)
{
    return ((CValueBase*)((CValueMap*)vm)->getMapValue(nm))->toString();
}

inline const string& vmg_str(CValue* vm, int idx)
{
    return ((CValueBase*)((CValueMap*)vm)->getArrayValue(idx))->toString();
}

inline double vmg_flt(CValue* vm, const string& nm)
{
    return ((CValueBase*)((CValueMap*)vm)->getMapValue(nm))->toFloat();
}

inline double vmg_flt(CValue* vm, int idx)
{
    return ((CValueBase*)((CValueMap*)vm)->getArrayValue(idx))->toFloat();
}

inline int vmg_int(CValue* vm, const string& nm)
{
    return ((CValueBase*)((CValueMap*)vm)->getMapValue(nm))->toInteger();
}

inline int vmg_int(CValue* vm, int idx)
{
    return ((CValueBase*)((CValueMap*)vm)->getArrayValue(idx))->toInteger();
}

inline bool vmg_bool(CValue* vm, const string& nm)
{
    return ((CValueBase*)((CValueMap*)vm)->getMapValue(nm))->toBoolean();
}

inline bool vmg_bool(CValue* vm, int idx)
{
    return ((CValueBase*)((CValueMap*)vm)->getArrayValue(idx))->toBoolean();
}

inline CValueMap* vmg_vm(CValue* vm, const string& nm)
{
    return (CValueMap*)((CValueMap*)vm)->getMapValue(nm);
}

inline CValueMap* vmg_vm(CValue* vm, int idx)
{
    return (CValueMap*)((CValueMap*)vm)->getArrayValue(idx);
}


#endif
