#pragma once
#include <cstdint>
#include "../Offsets/vtables/IScriptTable.h"
#include "../Offsets/vtables/IScriptSystem.h"

// -----------------------------------------------
// CScriptableBase -- CryEngine ScriptBind base (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVCScriptableBase@@ -- the single-inheritance base of EVERY wh ScriptBind (RTTI CHD of all
// derived binds = [self, CScriptableBase], mdisp 0). Shared Init sub_18144CE0C(this, pSS, ctx, flag);
// SetGlobalName sub_180B85140 (strncpy cap 64 into m_sGlobalName, then SetGlobalValue vf+0xF8).
//
// Base vtable is ~5 slots: [0] per-class deleting dtor, [1] Init sub_18144CE0C, [2] base dtor
// sub_1819D5648, [3] nullsub_1, [4] base method. Derived Lua-handler virtuals begin at slot 5
// (vtable +0x28) for the entity binds that register pointer-to-virtual-member handlers.
//
// Registration idiom (all derived binds): descriptor {name, argSig, &m_sGlobalName, invoker
// trampoline, delegate{this, handler}, 0x10} -> m_pMethodsTable->vf+0xB0 (RegisterFunction).
// The per-signature invoker trampolines are generic dispatchers; the REAL handler is the delegate
// target (documented per-bind in the derived headers).
//
// Init detail (verified): +0x50 stores Init's a2 -- in the bind ctors a2 is the result of
// pSS->vf[+0x278](pSS) (script-system accessor), and +0x48 = a2->CreateTable() (vf byte 0x68).
// m_sGlobalName inline char[64] (strncpy cap 64 + single-byte clear for empty; exact
// CryFixedStringT flavor vs raw buffer UNVERIFIED -- raw buffer fits all observed accesses).

class CScriptableBase {
public:
    virtual ~CScriptableBase();                  // [0] per-class scalar-deleting dtor

    char                    m_sGlobalName[64];   // +0x08  global-table name; EMPTY for per-entity binds
    Offsets::IScriptTable*  m_pMethodsTable;     // +0x48  CreateTable() result; RegisterFunction target (vf+0xB0)
    Offsets::IScriptSystem* m_pSS;               // +0x50  script system (Init a2 = pSS->vf[+0x278] result)
    int32_t                 m_bInstanceMethods;  // +0x58  1 = per-entity/instance methods table, 0 = global bind
    uint32_t                _pad5C;              // +0x5C
};
static_assert(sizeof(CScriptableBase) == 0x60, "CScriptableBase must be 0x60");
static_assert(offsetof(CScriptableBase, m_pMethodsTable) == 0x48, "methods table at 0x48");
