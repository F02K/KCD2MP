#pragma once
#include <cstdint>
#include "../Offsets/vtables/IScriptTable.h"

// -----------------------------------------------
// CScriptTable - CryScriptSystem concrete Lua-table wrapper  (KCD2)
// -----------------------------------------------
// RTTI:   .?AVCScriptTable@@     vtable 0x183a4a700
// Target: WHGame.dll 1.5.6 (base 0x180000000)
//
// A CScriptTable is a tiny 0x10 handle: a vtable ptr, a refcount, and a Lua
// registry reference (m_nRef) that names the underlying Lua table. All state
// lives in the shared Lua VM owned by the CScriptSystem singleton
// (CScriptSystem* @ 0x18549CF98). GetScriptSystem() returns that static.
//
// sizeof recovered from the allocator, NOT operator delete: the pool allocation
// in the create path (sub_1807180B0) calls ScriptSystem::pool_alloc(16); Release/
// dtor free the same block through the pooled deallocator (vfunc +0x1F8), which
// carries no size constant. => sizeof == 0x10.  VERIFIED.
//
// m_nRefCount (+0x08): AddRef `inc [this+8]`, Release `dec [this+8]` (frees at <=0).  VERIFIED
// m_nRef      (+0x0C): Lua registry ref; sentinel -1 == released (Release stores -1
//                      and errors on double-release: "Attempt to Release already
//                      released script table").                                       VERIFIED

class CScriptTable : public Offsets::IScriptTable   // +0x00  vtable ptr
{
public:
    int  m_nRefCount;   // +0x08  reference count (AddRef/Release)
    int  m_nRef;        // +0x0C  Lua registry index (luaL_ref); -1 => released

    // static CScriptSystem* m_pSS;  // shared VM owner; global @0x18549CF98 (not per-instance)
};
static_assert(sizeof(CScriptTable) == 0x10);
