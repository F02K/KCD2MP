#pragma once
#include <cstdint>

// -----------------------------------------------
// ScriptVarType / ScriptAnyType / ScriptAnyValue - KCD2 binary layout  (WHGame.dll 1.5.6, kd7u)
// -----------------------------------------------
// Mirrors CryCommon/IScriptSystem.h; layout and tag values verified:
//  - PushAny 0x180717B60 / ToAny 0x18071698C: type dword @+0x00, payload union
//    @+0x08 (numbers are 4-byte FLOATS - the VM is float-Lua), vec3.z @+0x10;
//  - EndCallAnyN strides results by 0x18 => sizeof 0x18;
//  - typed ToAny requests: preset .type = required ScriptAnyType; mismatch
//    fails (GetParamAny additionally raises a script error). type ANY_ANY
//    accepts anything and returns the actual type;
//  - function refs: ToAny(ANY_TFUNCTION) does lua_pushvalue+lua_ref - the ref
//    in .function is OWNED by the receiver (IScriptSystem::ReleaseFunc frees);
//  - .str out of ToAny points into the Lua TString - copy before the value
//    can be collected.

typedef void* HSCRIPTFUNCTION;   // Lua registry ref (int) widened to pointer; same typedef as Offsets/vtables/IScriptSystem.h

namespace Offsets { struct IScriptTable; }

// GetParamType/GetValueType result. Full lua->svt map of GetParamType
// 0x18082B688: nil->svtNull, bool->svtBool, lightuserdata->svtPointer,
// number->svtNumber, string->svtString, table->svtObject, function->
// svtFunction; full userdata falls through to svtNull in this build.
enum ScriptVarType
{
    svtNull = 0,
    svtString,
    svtNumber,
    svtBool,
    svtFunction,
    svtObject,
    svtPointer,
    svtUserData,
};

enum ScriptAnyType
{
    ANY_ANY = 0,
    ANY_TNIL,
    ANY_TBOOLEAN,
    ANY_THANDLE,
    ANY_TNUMBER,
    ANY_TSTRING,
    ANY_TTABLE,
    ANY_TFUNCTION,
    ANY_TUSERDATA,
    ANY_TVECTOR,
    ANY_COUNT,
};

struct ScriptAnyValue
{
    ScriptAnyType type;                      // +0x00 (dword; 4 bytes tail padding)
    union {                                  // +0x08 .. +0x17
        bool            b;                   // ANY_TBOOLEAN
        int64_t         nHandle;             // ANY_THANDLE (ScriptHandle / EntityId / WUID)
        float           number;              // ANY_TNUMBER - float-Lua VM
        const char*     str;                 // ANY_TSTRING
        Offsets::IScriptTable* table;        // ANY_TTABLE
        HSCRIPTFUNCTION function;            // ANY_TFUNCTION
        struct { float x, y, z; } vec3;      // ANY_TVECTOR (z @+0x10)
        struct { void* ptr; int nRef; } ud;  // ANY_TUSERDATA (nRef @+0x10)
    };

    ScriptAnyValue() : type(ANY_ANY) { table = nullptr; }
    ScriptAnyValue(bool value) : type(ANY_TBOOLEAN) { b = value; }
    ScriptAnyValue(int value) : type(ANY_TNUMBER) { number = (float)value; }
    ScriptAnyValue(unsigned int value) : type(ANY_TNUMBER) { number = (float)value; }
    ScriptAnyValue(float value) : type(ANY_TNUMBER) { number = value; }
    ScriptAnyValue(double value) : type(ANY_TNUMBER) { number = (float)value; }
    ScriptAnyValue(const char* value) : type(ANY_TSTRING) { str = value; }
    ScriptAnyValue(Offsets::IScriptTable* value) : type(ANY_TTABLE) { table = value; }  // non-owning; ref management stays with the caller
    ScriptAnyValue(HSCRIPTFUNCTION value) : type(ANY_TFUNCTION) { function = value; }

    // "Type hint" constructors (SDK idiom): tag the REQUIRED type for a typed
    // ToAny (GetParam/GetValue) without copying a value in.
    ScriptAnyValue(bool, int) : type(ANY_TBOOLEAN) { b = false; }
    ScriptAnyValue(int, int) : type(ANY_TNUMBER) { number = 0; }
    ScriptAnyValue(unsigned int, int) : type(ANY_TNUMBER) { number = 0; }
    ScriptAnyValue(float&, int) : type(ANY_TNUMBER) { number = 0; }
    ScriptAnyValue(double&, int) : type(ANY_TNUMBER) { number = 0; }
    ScriptAnyValue(const char*, int) : type(ANY_TSTRING) { str = nullptr; }
    ScriptAnyValue(Offsets::IScriptTable*, int) : type(ANY_TTABLE) { table = nullptr; }
    ScriptAnyValue(HSCRIPTFUNCTION, int) : type(ANY_TFUNCTION) { function = nullptr; }

    bool CopyTo(bool& value) const           { if (type == ANY_TBOOLEAN)  { value = b; return true; } return false; }
    bool CopyTo(int& value) const            { if (type == ANY_TNUMBER)   { value = (int)number; return true; } return false; }
    bool CopyTo(unsigned int& value) const   { if (type == ANY_TNUMBER)   { value = (unsigned int)number; return true; } return false; }
    bool CopyTo(float& value) const          { if (type == ANY_TNUMBER)   { value = number; return true; } return false; }
    bool CopyTo(double& value) const         { if (type == ANY_TNUMBER)   { value = number; return true; } return false; }
    bool CopyTo(const char*& value) const    { if (type == ANY_TSTRING)   { value = str; return true; } return false; }
    bool CopyTo(Offsets::IScriptTable*& value) const { if (type == ANY_TTABLE) { value = table; return true; } return false; }
    bool CopyTo(HSCRIPTFUNCTION& value) const { if (type == ANY_TFUNCTION) { value = function; return true; } return false; }
};
static_assert(sizeof(ScriptAnyValue) == 0x18, "ScriptAnyValue must be 0x18 in KCD2");
