#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <new>
#include "CryEngine/CryCommon/Cry_Math.h"        // Vec3
#include "CryEngine/CryCommon/CryString.h"       // CryStringT<char>/<wchar_t> (8-byte handle)
#include "CryEngine/CryCommon/CryArray.h"        // DynArray / WHDynStorage (game NArray block format)
#include "CryEngine/CryCommon/SFlashVarValue.h"  // SFlashVarValue (16B; SUIArguments +0x18 element)

// ---------------------------------------------------------------------------
// FlashUI value types -- KCD2 WHGame.dll 1.5.6 (kd7u).  Native, binary-layout-matched.
// ---------------------------------------------------------------------------
// The argument/result types IUIElement::CallFunction [slot 69] / the FlashUI event system
// marshal between C++ and ActionScript.  Declared in the GLOBAL namespace (matching CryEngine's
// SUITypes and KCD1's replica): the Offsets vtable interfaces forward-declare a GLOBAL
// `SUIArguments` and reference it unqualified, so `el->CallFunction(name, args, &res, tmpl)`
// binds with no cast.  (The vendored SDK CryEngine/CryCommon/IFlashUI.h -- which also declares a
// global SUIArguments -- is never compiled; every TU uses Offsets/vtables/IFlashUI.h instead, so
// there is no redefinition clash.)
//
// Layouts VERIFIED against kd7u (ctor sub_1807F7170, dtor sub_1807F71C8, element copy
// sub_1803C2170 stride 0x20, CallFunction[68] result write sub_181561BF8, read helper
// sub_180B5BBB8) and adversarially re-checked.  DELTAS vs the KCD1 SUITypes.h replica:
//   1. TUIData is INVERTED -- KCD2 stores { value@+0x00 (16B); type tag@+0x10 }, whereas
//      KCD1 stored { type@+0x00; value@+0x08 }.  Both sizeof 0x18.
//   2. SUIArguments +0x18 is a live DynArray<SFlashVarValue> (lazy flash-var cache), NOT
//      KCD1's unused void*.  Still 8 bytes, so sizeof stays 0x30.
// Everything else (SUIData 0x20, the enum, the delimiter/dirty fields, the DynArray
// discipline: count @storage-4 / capacity @storage-8 / shared empty sentinel) matches KCD1.

// EntityId: identical (unsigned int) to the CryEngine typedef -> a repeated typedef of the
// same underlying type is legal even if another header already declares it.
typedef uint32_t EntityId;

// Type tags = index into the variant's type list.  eUIDT_Any (-1) = untyped.  Verified by the
// read switch in sub_180B5BBB8 and the write dispatch in sub_181561BF8 (kd7u).
enum EUIDataTypes {
    eUIDT_Any      = -1,
    eUIDT_Int      =  0,
    eUIDT_Float    =  1,
    eUIDT_EntityId =  2,
    eUIDT_Vec3     =  3,
    eUIDT_String   =  4,
    eUIDT_WString  =  5,
    eUIDT_Bool     =  6,
};

// ===========================================================================
// TUIData -- tagged union (0x18).  KCD2 order: value FIRST, type tag at +0x10 (byte-accessed
// everywhere; declared int32 for the padded slot, only the low byte is used).  Strings are a
// refcounted CryStringT (8-byte handle) placed at the front of the value buffer.
// ===========================================================================
struct TUIData
{
    union {                          // +0x00  (16 bytes)
        int32_t   m_int;
        float     m_float;
        EntityId  m_entityId;
        Vec3      m_vec3;
        uint8_t   m_bool;
        char      m_buffer[16];      // CryStringT<char>/<wchar_t> handle lives here
        void*     _align8;           // 8-byte align device (not a real field): String case stores an 8B CryStringT<char> handle at +0x00 (sub_1804FD208, refcnt at ptr-12); Vec3 case does a QWORD store there
    };
    // +0x10  CryVariant index: a SINGLE BYTE (-1 = valueless); the engine NEVER
    // initializes +0x11..+0x17. Reading this as int32 poisons the tag with heap
    // garbage (probe 2026-07-12: float tag read as 0x3F800001 -> AsString ''/
    // AsFloat 0 -> the whole phantom "EI transport kills args" saga). Writers in
    // the binary: builders 0x1803C2034/0x1803C2158 et al. store the byte only;
    // copy loop 0x1810C2AD8 presets byte -1 then dispatches on (tag+1).
    int8_t  m_type;
    uint8_t _pad11[7];               // +0x11  uninitialized in engine-built args

    TUIData()                             : m_type(eUIDT_Int)     { m_int = 0; }
    TUIData(int v)                        : m_type(eUIDT_Int)     { m_int = v; }
    TUIData(float v)                      : m_type(eUIDT_Float)   { m_float = v; }
    TUIData(bool v)                       : m_type(eUIDT_Bool)    { m_bool = v ? 1 : 0; }
    TUIData(const Vec3& v)                : m_type(eUIDT_Vec3)    { m_vec3 = v; }
    TUIData(const char* s)                : m_type(eUIDT_String)  { new (m_buffer) CryStringT<char>(s); }
    TUIData(const CryStringT<char>& s)    : m_type(eUIDT_String)  { new (m_buffer) CryStringT<char>(s); }
    TUIData(const CryStringT<wchar_t>& s) : m_type(eUIDT_WString) { new (m_buffer) CryStringT<wchar_t>(s); }

    TUIData(const TUIData& o) : m_type(o.m_type)
    {
        if (o.m_type == eUIDT_String)       new (m_buffer) CryStringT<char>(o.str());
        else if (o.m_type == eUIDT_WString) new (m_buffer) CryStringT<wchar_t>(o.wstr());
        else                                memcpy(m_buffer, o.m_buffer, sizeof(m_buffer));
    }
    TUIData& operator=(const TUIData& o)
    {
        if (this != &o) { this->~TUIData(); new (this) TUIData(o); }
        return *this;
    }
    ~TUIData()
    {
        if (m_type == eUIDT_String)       str().~CryStringT();
        else if (m_type == eUIDT_WString) wstr().~CryStringT();
    }

    EUIDataTypes GetType() const { return (EUIDataTypes)m_type; }

    CryStringT<char>&          str()        { return *reinterpret_cast<CryStringT<char>*>(m_buffer); }
    const CryStringT<char>&    str()  const { return *reinterpret_cast<const CryStringT<char>*>(m_buffer); }
    CryStringT<wchar_t>&       wstr()       { return *reinterpret_cast<CryStringT<wchar_t>*>(m_buffer); }
    const CryStringT<wchar_t>& wstr() const { return *reinterpret_cast<const CryStringT<wchar_t>*>(m_buffer); }

    // --- numeric read with coercion (matches sub_180B5BBB8's per-type conversion) ---
    float AsFloat() const
    {
        switch (m_type) {
            case eUIDT_Int:      return (float)m_int;
            case eUIDT_Float:    return m_float;
            case eUIDT_EntityId: return (float)m_entityId;
            case eUIDT_Bool:     return m_bool ? 1.f : 0.f;
            case eUIDT_Vec3:     return m_vec3.x;
            case eUIDT_String:   return (float)atof(str().c_str());
            case eUIDT_WString:  return (float)wcstod(wstr().c_str(), nullptr);   // binary case 5 in sub_180B5BBB8
            default:             return 0.f;
        }
    }
    int      AsInt()      const { return (m_type == eUIDT_Int)      ? m_int      : (int)AsFloat(); }
    EntityId AsEntityId() const { return (m_type == eUIDT_EntityId) ? m_entityId : (EntityId)AsFloat(); }
    bool     AsBool()     const { return (m_type == eUIDT_Bool)     ? (m_bool != 0) : (AsFloat() != 0.f); }
    Vec3     AsVec3()     const { if (m_type == eUIDT_Vec3) return m_vec3; float f = AsFloat(); return Vec3{ f, f, f }; }
    CryStringT<char> AsString() const
    {
        if (m_type == eUIDT_String) return str();
        if (m_type == eUIDT_WString) {
            // GFx hands flash strings over WIDE, so event args commonly arrive as
            // eUIDT_WString; narrow as UTF-8 (engine equivalent: SUIConversion<wstring,string>).
            const CryStringT<wchar_t>& w = wstr();
            CryStringT<char> out;
            for (size_t i = 0; i < w.length(); ++i) {
                unsigned int c = (unsigned int)(uint16_t)w[i];
                if (c >= 0xD800 && c <= 0xDBFF && i + 1 < w.length()) {   // surrogate pair
                    unsigned int lo = (unsigned int)(uint16_t)w[i + 1];
                    if (lo >= 0xDC00 && lo <= 0xDFFF) { c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00); ++i; }
                }
                if (c < 0x80) {
                    out.push_back((char)c);
                } else if (c < 0x800) {
                    out.push_back((char)(0xC0 | (c >> 6)));
                    out.push_back((char)(0x80 | (c & 0x3F)));
                } else if (c < 0x10000) {
                    out.push_back((char)(0xE0 | (c >> 12)));
                    out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
                    out.push_back((char)(0x80 | (c & 0x3F)));
                } else {
                    out.push_back((char)(0xF0 | (c >> 18)));
                    out.push_back((char)(0x80 | ((c >> 12) & 0x3F)));
                    out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
                    out.push_back((char)(0x80 | (c & 0x3F)));
                }
            }
            return out;
        }
        char tmp[64];
        switch (m_type) {
            case eUIDT_Int:      snprintf(tmp, sizeof(tmp), "%d", m_int); break;
            case eUIDT_Float:    snprintf(tmp, sizeof(tmp), "%f", m_float); break;
            case eUIDT_EntityId: snprintf(tmp, sizeof(tmp), "%u", m_entityId); break;
            case eUIDT_Bool:     snprintf(tmp, sizeof(tmp), "%d", m_bool); break;
            case eUIDT_Vec3:     snprintf(tmp, sizeof(tmp), "%f,%f,%f", m_vec3.x, m_vec3.y, m_vec3.z); break;
            default:             tmp[0] = 0; break;
        }
        return CryStringT<char>(tmp);
    }

    bool GetValueWithConversion(int& v)      const { v = AsInt(); return true; }
    bool GetValueWithConversion(float& v)    const { v = AsFloat(); return true; }
    bool GetValueWithConversion(bool& v)     const { v = AsBool(); return true; }
    bool GetValueWithConversion(EntityId& v) const { v = AsEntityId(); return true; }
    bool GetValueWithConversion(Vec3& v)     const { v = AsVec3(); return true; }
    bool GetValueWithConversion(CryStringT<char>& v)    const { v = AsString(); return true; }
    bool GetValueWithConversion(CryStringT<wchar_t>& v) const { if (m_type == eUIDT_WString) { v = wstr(); return true; } return false; }
};
static_assert(sizeof(TUIData) == 0x18, "TUIData 0x18 (kd7u)");
static_assert(offsetof(TUIData, m_type) == 0x10, "TUIData type tag @+0x10 (KCD2: inverted vs KCD1)");

// SUIData -- one {type, value} element of SUIArguments::m_ArgList (stride 0x20; element copy
// sub_1803C2170 indexes at storage + 0x20*i).  Field order matches KCD1; only the embedded
// TUIData's internals moved.
struct SUIData
{
    EUIDataTypes m_eType;   // +0x00  (int32; dword-copied by sub_1803C2170)
    int32_t      _pad04;
    TUIData      m_value;   // +0x08

    SUIData() : m_eType(eUIDT_Int), _pad04(0) {}
    SUIData(EUIDataTypes t, const TUIData& v) : m_eType(t), _pad04(0), m_value(v) {}
};
static_assert(sizeof(SUIData) == 0x20, "SUIData 0x20 (stride proven by sub_1803C2170)");
static_assert(offsetof(SUIData, m_value) == 0x08, "SUIData TUIData @+0x08");

// ===========================================================================
// SUIArguments -- typed argument list for FlashUI calls/events (0x30).  For a no-arg call
// (e.g. reading a cursor getter) a default-constructed instance is a valid empty arg pack:
// CallFunction reads m_ArgList's count (0) and the flash-var cache stays empty (dirty bit
// eBDF_FlashVar clear).
// ===========================================================================
struct SUIArguments
{
    enum EBufferDirtyFlag {
        eBDF_None      = 0x00,
        eBDF_String    = 0x01,
        eBDF_WString   = 0x02,
        eBDF_FlashVar  = 0x04,
        eBDF_Delimiter = 0x08,
        eBDF_ALL       = 0xFF,
    };

    SUIArguments() : m_cDelimiter("|"), m_Dirty(eBDF_Delimiter) {}

    // --- build ---
    void AddArgument(int v)                        { push(eUIDT_Int,     TUIData(v)); }
    void AddArgument(float v)                      { push(eUIDT_Float,   TUIData(v)); }
    void AddArgument(bool v)                        { push(eUIDT_Bool,    TUIData(v)); }
    void AddArgument(const Vec3& v)                { push(eUIDT_Vec3,    TUIData(v)); }
    void AddArgument(const char* v)                { push(eUIDT_String,  TUIData(v)); }
    void AddArgument(const CryStringT<char>& v)    { push(eUIDT_String,  TUIData(v)); }
    void AddArgument(const CryStringT<wchar_t>& v) { push(eUIDT_WString, TUIData(v)); }
    void AddArgument(const TUIData& d)             { push(d.GetType(),   d); }
    void AddArgumentEntityId(EntityId v)           { TUIData d; d.m_type = eUIDT_EntityId; d.m_entityId = v; push(eUIDT_EntityId, d); }

    template <class T>
    static SUIArguments Create(const T& arg) { SUIArguments a; a.AddArgument(arg); return a; }

    void Clear() { m_ArgList.clear(); m_Dirty = eBDF_ALL; }

    // --- read ---
    int            GetArgCount() const     { return (int)m_ArgList.size(); }
    EUIDataTypes   GetArgType(int i) const { return m_ArgList[i].m_eType; }
    const TUIData& GetArg(int i) const     { return m_ArgList[i].m_value; }

    template <class T>
    bool GetArg(int index, T& val) const
    {
        if (index < 0 || index >= (int)m_ArgList.size()) return false;
        return m_ArgList[index].m_value.GetValueWithConversion(val);
    }

    // --- layout (public so the offsetof guards below can see it; matches kd7u) ---
    DynArray<SUIData>            m_ArgList;        // +0x00  arg list (empty = shared sentinel)
    CryStringT<char>            m_sBuiltString;   // +0x08  lazy GetAsString() cache
    CryStringT<wchar_t>         m_sBuiltWString;  // +0x10  lazy GetAsStringW() cache
    DynArray< ::SFlashVarValue> m_FlashVars;      // +0x18  lazy SFlashVarValue cache (KCD2: was void* in KCD1)
    CryStringT<char>            m_cDelimiter;     // +0x20  default "|"
    uint32_t                    m_Dirty;          // +0x28  EBufferDirtyFlag mask (ctor = eBDF_Delimiter)

private:
    void push(EUIDataTypes t, const TUIData& d) { m_ArgList.push_back(SUIData(t, d)); m_Dirty = eBDF_ALL; }
};
static_assert(sizeof(SUIArguments)  == 0x30, "SUIArguments 0x30 (kd7u ctor sub_1807F7170)");
static_assert(offsetof(SUIArguments, m_ArgList)    == 0x00, "m_ArgList @+0x00");
static_assert(offsetof(SUIArguments, m_FlashVars)  == 0x18, "m_FlashVars @+0x18 (KCD2 change vs KCD1 void*)");
static_assert(offsetof(SUIArguments, m_cDelimiter) == 0x20, "delimiter @+0x20");
static_assert(offsetof(SUIArguments, m_Dirty)      == 0x28, "dirty @+0x28");
