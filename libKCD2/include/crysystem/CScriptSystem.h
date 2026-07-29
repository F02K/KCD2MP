#pragma once
#include <cstdint>
#include <cstddef>
#include "ScriptAnyValue.h"               // ScriptAnyValue / ScriptVarType / ScriptAnyType (KCD2 layouts)
#include "../Offsets/vtables/IScriptSystem.h"
#include "../Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener (secondary base)

// -----------------------------------------------
// CScriptSystem - CryScriptSystem concrete Lua VM owner  (KCD2)
// -----------------------------------------------
// RTTI:   .?AVCScriptSystem@@      primary vtable 0x183b8c610 (69 slots)
// Target: WHGame.dll 1.5.6 (base 0x180000000)
// Constructor:        0x18144913c   |  Complete dtor: 0x1839ae5c8
// Scalar deleting dtor (vtable[0]): 0x1839ae87c -> operator delete(this, 17368)
//
// Multiple inheritance (from ctor/dtor vtable stores):
//   CScriptSystem : IScriptSystem (primary, +0x00), ISystemEventListener (+0x08)
//
// sizeof recovered from the scalar-deleting destructor:
//   sub_1839AE87C: sub_1839AE5C8(this); if(flag&1) sub_181AB5160(this, 17368);
//   => operator delete(this, 0x43D8).  VERIFIED.
//
// The bulk of the object (0x4300 of 0x43D8) is an inline array of 32 call/parse
// frames of 0x218 bytes each, built by the ctor loop
//   sub_18050B734(this+0xC0, /*stride*/536, /*count*/32, sub_181845230)
// and torn down by sub_180697F80(this+0xC0, 536, 32, ...).  VERIFIED extent.
//
// Global singleton pointer set by ctor: CScriptSystem* @ 0x18549CFB0.
// Shared lua_State* also cached at global 0x18549CFA0.

typedef struct lua_State lua_State;
struct ISystem;
class CScriptTimerMgr;   // owned timer manager (+0xB0, 0x40 bytes)
class CLUADbg;           // lazy Lua debugger (+0x43D0, 0x390 bytes)

// ScriptAnyValue (0x18-byte KCD2 layout) lives in ScriptAnyValue.h.

// -----------------------------------------------
// CScriptSystem
// -----------------------------------------------
class CScriptSystem
    : public Offsets::IScriptSystem          // +0x00  primary vtable (IScriptSystem)
    , public Offsets::ISystemEventListener   // +0x08  secondary base (dtor removes this as a system-event listener)
{
public:
    // ---- Lua VM ----
    lua_State*   m_pLuaState;                    // +0x10  Lua state; every script op reads this+0x10  VERIFIED
    ICVar*       m_pLuaDebuggerCVar;            // +0x18  Init 0x181449439 a1[3]=RegisterInt("lua_debugger",0,2,...)
    ICVar*       m_pCodeCoverageCVar;           // +0x20  Init 0x18144943d a1[4]=RegisterInt("lua_CodeCoverage",0,2,...)

    int          m_nParamCount;                  // +0x28  active-call param counter (BeginCall sets 0/-1, PushFuncParam++)  VERIFIED
    int          _pad2C;                         // +0x2C
    void*        _field30;                       // +0x30  (ctor 0)
    void*        _field38;                       // +0x38  (ctor 0)

    std::vector<void*> m_memBlocks;              // +0x40  {first,last,end}; dtor sub_1803F70D8 frees each elem-12 via pool free (sub_1804FD898), then buffer
    void*        _field58;                       // +0x58  (ctor 0; no writer found)
    std::set<uint64_t> m_loadedScripts;          // +0x60  MSVC _Tree {_Myhead@+0x60,_Mysize@+0x68}; ctor sub_1803F7FAC allocs RB sentinel; dtor _Tree-erase + delete node(0x28)

    uint8_t      m_scriptBindings[0x28];         // +0x70  embedded CScriptBindings (vtable@+0x70; dtor sub_1839AF040/sub_1839AE520)  tentative
    ISystem*     m_pSystem;                       // +0x98  Init 0x181449215 a1[19]=pSystem arg; dtor GetISystemEventDispatcher()->RemoveListener(this)

    float        m_fGCFreq;                      // +0xA0  GC frequency, seconds (ctor 10.0f; SetGCFrequency stores here)  VERIFIED
    uint8_t      _padA4[4];                      // +0xA4
    int          m_nLastGCCount;                 // +0xA8  Init 0x18144934a & Update 0x180a26c93 store GetCGCount() (vtbl[36]) as dword
    uint8_t      _padAC[4];                       // +0xAC
    CScriptTimerMgr* m_pTimerMgr;                // +0xB0  Init a1[22]=new CScriptTimerMgr(this) (sub_1814494CC, 0x40 bytes); dtor deletes via vtbl[0]
    int          m_nCallFrameDepth;              // +0xB8  active call-frame count for the +0xC0 frame pool; EnableDebugger 0x180a26f48 stores 0
    uint8_t      _padBC[4];                       // +0xBC

    // ---- inline call/parse frame pool: 32 x 0x218 = 0x4300 ----
    uint8_t      m_callFrames[0x4300];           // +0xC0   ctor sub_18050B734(+0xC0, 536, 32, ...)      VERIFIED extent

    void*        m_pAllocator;                   // +0x43C0 handle into GLOBAL script small-block pool: ctor a1[2168]=sub_1804FD80C()(&dword_18547D030)+12; dtor sub_1804FD898(x-12)
    void*        _field43C8;                      // +0x43C8 (unset in ctor)
    CLUADbg*     m_pLuaDebugger;                  // +0x43D0 Lua debugger (0x390 bytes) created lazily by EnableDebugger; also cached in global 0x1856652B0
};
static_assert(sizeof(CScriptSystem) == 0x43D8, "CScriptSystem must be 0x43D8 (operator delete size)");
