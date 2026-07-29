#pragma once
#include <cstdint>
#include "../../crysystem/ScriptAnyValue.h"   // ScriptAnyValue / ScriptVarType (KCD2 layouts)

// -----------------------------------------------
// IScriptTable - KCD2 binary vtable order  (CryScriptSystem CScriptTable)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (base 0x180000000).
// RTTI:   .?AVCScriptTable@@     primary vtable 0x183a4a700 (25 slots)
// The interface is interfuscator-shuffled; slot identities below were recovered by
// decompiling each thunk on kd7u (not ported blindly from KCD1). 20/25 slots are
// behaviourally confirmed; the 5 marked "tentative" still need a consumer before trust.
//
// Every method drives the shared Lua stack held by the CScriptSystem singleton
// (global CScriptSystem* @ 0x18549CF98; per-table Lua registry ref = CScriptTable::m_nRef).

struct IScriptTableDumpSink;
struct SUserFunctionDesc;    // C-function registration descriptor (crysystem/SUserFunctionDesc.h)

namespace Offsets {

struct IScriptSystem;        // Offsets/vtables/IScriptSystem.h

struct IScriptTable {
    // ---- lifetime / identity ----
    virtual void _vf0() = 0;                                                 // [0]  0x00  scalar-deleting dtor; restores vtbl, frees 0x10 via ScriptSystem pool
    virtual IScriptSystem* GetScriptSystem() const = 0;                      // [1]  0x08  returns global CScriptSystem (static m_pSS @0x18549CF98)   VERIFIED
    virtual int  GetLuaRegisterRef() const = 0;                             // [2]  0x10  return *(int*)(this+0x0C) = m_nRef                        tentative
    virtual void AddRef() = 0;                                               // [3]  0x18  inc [this+0x08] (m_nRefCount)                             VERIFIED
    virtual void Release() = 0;                                              // [4]  0x20  dec [this+0x08]; at <=0 luaL_unref + pool-free             VERIFIED
    virtual void Delegate(IScriptTable* pMetatable) = 0;                     // [5]  0x28  sets __index metatable to pMetatable                       VERIFIED

    // ---- value get/set ----
    virtual void* GetUserDataValue() = 0;                                    // [6]  0x30  pushes self, reads lua userdata                           tentative
    virtual void SetValueAny(const char* sKey, const ScriptAnyValue& any,
                             bool bChain = false) = 0;                       // [7]  0x38  set field; special-cases Vec3 (type 9)                     VERIFIED
    virtual bool GetValueAny(const char* sKey, ScriptAnyValue& any,
                             bool bChain = false) = 0;                       // [8]  0x40  get field -> any; false if missing                        VERIFIED
    virtual bool _vf9() = 0;                                                 // [9]  0x48  pushes self, returns true (stub-like)                     tentative
    virtual int  _vf10() = 0;                                                // [10] 0x50  pops if top-of-stack lua type == table(5)                 tentative
    virtual ScriptVarType GetValueType(const char* sKey) = 0;               // [11] 0x58  ScriptVarType of field (lua->svt map)                      VERIFIED
    virtual ScriptVarType GetAtType(int nIdx) = 0;                          // [12] 0x60  ScriptVarType of array element                             VERIFIED
    virtual void SetAtAny(int nIndex, const ScriptAnyValue& any) = 0;        // [13] 0x68  set array element                                          VERIFIED
    virtual bool GetAtAny(int nIndex, ScriptAnyValue& any) = 0;             // [14] 0x70  get array element -> any                                    VERIFIED

    // ---- iteration ----
    struct Iterator;                                                         // fwd (defined below)
    virtual Iterator BeginIteration(bool resolvePrototypeTableAsWell = false) = 0; // [15] 0x78  seed iterator (sret)                          VERIFIED
    virtual bool MoveNext(Iterator& iter) = 0;                               // [16] 0x80  advance; false at end                                     VERIFIED
    virtual void EndIteration(const Iterator& iter) = 0;                     // [17] 0x88  pop iterator stack frame                                  VERIFIED

    // ---- bulk / misc ----
    virtual void Clear() = 0;                                                // [18] 0x90  remove all entries                                        VERIFIED
    virtual int  Count() = 0;                                                // [19] 0x98  lua rawlen                                                VERIFIED
    virtual bool Clone(IScriptTable* pSrcTable, bool bDeepCopy = false,
                       bool bCopyByReference = false) = 0;                   // [20] 0xA0  copy src into this (3 copy modes)                         VERIFIED
    virtual void Dump(IScriptTableDumpSink* p) = 0;                          // [21] 0xA8  enumerate keys -> sink->OnElementFound                     VERIFIED
    virtual bool AddFunction(const SUserFunctionDesc& fd) = 0;               // [22] 0xB0  register C fn; builds "%s.%s(%s)" debug name            VERIFIED
    virtual void SetMetatable(IScriptTable* pMetatable) = 0;                 // [23] 0xB8  lua_setmetatable (Delegate helper)                       tentative
    virtual bool _vf24(const char* sKey) = 0;                               // [24] 0xC0  GetValueAny(key) then releases func/handle refs           tentative

    // Iterator: tentative layout (~0x50) reconstructed from BeginIteration/MoveNext disasm.
    struct Iterator {
        const char* sKey;               // +0x00  string key (lua_tolstring); null when integer-keyed
        int      nKey;                  // +0x08  integer key (-1 => string-key iteration)
        ScriptAnyValue value;           // +0x10  current value
        ScriptAnyValue key;             // +0x28  current key
        uint8_t  bResolvePrototype;     // +0x40  resolvePrototypeTableAsWell
        uint8_t  _pad41[3];             // +0x41
        int      nStackMarker;          // +0x44  pushed-stack-entry count
        int      nCursor;               // +0x48  internal cursor
        uint8_t  _pad4C[4];             // +0x4C
    };
    static_assert(sizeof(Iterator) == 0x50, "iterator frame size (BeginIteration/MoveNext)");
};

}  // namespace Offsets
