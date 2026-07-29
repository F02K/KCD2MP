#pragma once
#include <cstdint>

// -----------------------------------------------
// SFlashVarValue — Scaleform GFx value variant
// -----------------------------------------------
// CryCommon/IFlashPlayer.h type, GLOBAL namespace (matches the binary; the
// IFlashPlayer replica's SetVariable/GetVariable/Invoke take it by const ref /
// as a 0x10-stride array). KCD2 1.5.6: converted to GFxValue by sub_1807F6490
// and back by sub_1807F659C — layout {Type@0, Data@8} and Type order verified
// unchanged vs KCD1 (eInt/eUInt/eFloat all -> GFx number 3, str->4, wstr->5).
//
// Lifted from the SDK struct verbatim: the body is pure POD + inline accessors
// with NO engine deps — it is only the SDK *header* that pulls Cry_Math.h
// (which must never be included here). A default ctor (eUndefined) is added for
// use as a GetVariable out-param; the SDK has none.

struct SFlashVarValue {
    union Data {
        bool           b;
        int            i;
        unsigned int   ui;
        double         d;
        float          f;
        const char*    pStr;
        const wchar_t* pWstr;
    };

    enum Type {
        eUndefined,
        eNull,
        eBool,
        eInt,
        eUInt,
        eDouble,
        eFloat,
        eConstStrPtr,
        eConstWstrPtr,
        eObject,        // receive only
    };

    SFlashVarValue()                  : type(eUndefined)    {}
    SFlashVarValue(bool val)          : type(eBool)         { data.b = val; }
    SFlashVarValue(int val)           : type(eInt)          { data.i = val; }
    SFlashVarValue(unsigned int val)  : type(eUInt)         { data.ui = val; }
    SFlashVarValue(double val)        : type(eDouble)       { data.d = val; }
    SFlashVarValue(float val)         : type(eFloat)        { data.f = val; }
    SFlashVarValue(const char* val)   : type(eConstStrPtr)  { data.pStr = val; }
    SFlashVarValue(const wchar_t* val): type(eConstWstrPtr) { data.pWstr = val; }
    static SFlashVarValue CreateUndefined() { return SFlashVarValue(eUndefined); }
    static SFlashVarValue CreateNull()      { return SFlashVarValue(eNull); }

    bool           GetBool()         const { return data.b; }
    int            GetInt()          const { return data.i; }
    unsigned int   GetUInt()         const { return data.ui; }
    double         GetDouble()       const { return data.d; }
    float          GetFloat()        const { return data.f; }
    const char*    GetConstStrPtr()  const { return data.pStr; }
    const wchar_t* GetConstWstrPtr() const { return data.pWstr; }

    Type GetType()     const { return type; }
    bool IsUndefined() const { return type == eUndefined; }
    bool IsNull()      const { return type == eNull; }
    bool IsBool()      const { return type == eBool; }
    bool IsInt()       const { return type == eInt; }
    bool IsUInt()      const { return type == eUInt; }
    bool IsDouble()    const { return type == eDouble; }
    bool IsFloat()     const { return type == eFloat; }
    bool IsString()    const { return type == eConstStrPtr; }
    bool IsWString()   const { return type == eConstWstrPtr; }

private:
    explicit SFlashVarValue(Type t) : type(t) {}
    Type type;
    Data data;
};
static_assert(sizeof(SFlashVarValue) == 0x10, "SFlashVarValue must be 0x10 (Invoke uses 0x10-stride)");
