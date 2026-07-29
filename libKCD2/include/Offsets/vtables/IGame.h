#pragma once
#include <cstdint>
#include "IGameFrameworkListener.h"   // Offsets::IGameFrameworkListener (moved to its own header,
                                      // slot order re-verified from the CCryAction broadcast sites)
#include "IInputEventListener.h"      // Offsets::IInputEventListener (extracted to its own header)

// -----------------------------------------------
// IGame + game-framework listener interfaces — KCD2 binary vtable order
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (KCD2), image base 0x180000000
// These are the five polymorphic base interfaces of wh::game::C_Game
// (see ../../game/C_Game.h). Each is a pure interface: one vptr, no data.
//
//   Base                       sub-vtable        this-disp   slots
//   IGame                      0x184095688       +0x00       41   (primary / RTTI COL group)
//   IGameFrameworkListener     0x184095638       +0x08        9
//   ILevelSystemListener       0x1840955C8       +0x10       10
//   IInputEventListener        0x1840955A0       +0x18        4
//   wh::framework::I_ModuleMessageListener  0x184095620  +0x20  2
//
// RTTI base array of .?AVC_Game@game@wh@@ (verified):
//   IGame, IGameFrameworkListener, ILevelSystemListener,
//   IInputEventListener, I_ModuleMessageListener@framework@wh
//
// Confidence: slot COUNTS and accessor/getter identities are VERIFIED from disasm.
// SDK-style method names on non-accessor slots are inferred from CryEngine's classic
// interface order + fingerprints and are marked "tentative"; unknown behaviour is _vfN.
// Stubs seen: ret0 = 0x1803B6E80, ret_null = 0x18066CD10, ret_true = 0x18041A6A0.

namespace Offsets {

// ---------------------------------------------------------------------------
// IGame — CryEngine game module interface. C_Game primary vtable (41 slots).
// ---------------------------------------------------------------------------
struct IGame {
    virtual void        _vf0(char flags) = 0;                 // [0]  0x1839D8468  scalar deleting dtor -> operator delete(this,0x198)
    virtual bool        Init(void* pSystem, void* pFramework) = 0; // [1]  0x1806034B4  IGame::Init (1787 bytes)
    virtual bool        _vf2() = 0;                           // [2]  ret 0        (CompleteInit? tentative)
    virtual void        Shutdown() = 0;                       // [3]  0x1839D8FC0  tentative
    virtual int         Update(bool haveFocus, unsigned updateFlags) = 0; // [4] 0x180BC50D8  increments frame counter dword_18549D308
    virtual void        _vf5() = 0;                           // [5]  0x1839D9798  (565 bytes)
    virtual void        _vf6() = 0;                           // [6]  0x180FC457C  sends C_ModuleMessage 0x0B
    virtual void        _vf7() = 0;                           // [7]  0x180FC45BC  sends C_ModuleMessage 0x0C
    virtual void        _vf8() = 0;                           // [8]  0x180532C54  (630 bytes)
    virtual void        _vf9() = 0;                           // [9]  0x1839D9244
    virtual void        _vf10() = 0;                          // [10] ret 0
    virtual void        _vf11(int value) = 0;                 // [11] 0x180B855F4  *(uint32*)(this+0x3C)=value; then external
    virtual const char* GetLongName() = 0;                    // [12] 0x180DA30E0  returns "Kingdom Come: Deliverance"
    virtual const char* GetName() = 0;                        // [13] 0x1839D91DC  returns "kcd2"
    virtual void        _vf14() = 0;                          // [14] 0x18173CB3C  (385 bytes)
    virtual void        _vf15() = 0;                          // [15] ret 0
    virtual void*       _vf16() = 0;                          // [16] 0x180645968  returns (gEnv/framework + 8)
    virtual void*       _vf17(void* p) = 0;                   // [17] 0x18046B830  returns arg (identity)
    virtual void*       _vf18() = 0;                          // [18] ret null
    virtual void*       _vf19() = 0;                          // [19] ret null
    virtual void        _vf20() = 0;                          // [20] ret 0
    virtual void        _vf21() = 0;                          // [21] ret 0
    virtual bool        _vf22() = 0;                          // [22] ret true
    virtual void        _vf23() = 0;                          // [23] ret 0
    virtual void        _vf24() = 0;                          // [24] ret 0
    virtual void        _vf25() = 0;                          // [25] ret 0
    virtual void*       _vf26() = 0;                          // [26] ret null
    virtual void        _vf27() = 0;                          // [27] 0x1839D8E28
    virtual void        _vf28() = 0;                          // [28] ret 0
    virtual void*       _vf29(void* out) = 0;                 // [29] 0x181AA57B0  zeroes out[0..0xC], returns out (GetMemoryStatistics-like)
    virtual void        _vf30() = 0;                          // [30] 0x18170FF94  (550 bytes)
    virtual void        _vf31() = 0;                          // [31] 0x1803DF0BC
    virtual void*       _vf32() = 0;                          // [32] 0x180834590  return *(void**)(this+0x140)   VERIFIED getter@0x140
    virtual void*       _vf33() = 0;                          // [33] ret null
    virtual void        _vf34() = 0;                          // [34] 0x1839D91E4  if(*(u32*)(this+0x30)==0) *(u32*)(this+0x30)=1
    virtual void*       _vf35() = 0;                          // [35] 0x181A77FA0  return *(void**)(this+0x178)   VERIFIED getter@0x178
    virtual void        _vf36(int value) = 0;                 // [36] 0x181AB4D00  *(uint32*)(this+0x190)=value    VERIFIED setter@0x190
    virtual void        _vf37() = 0;                          // [37] 0x180E55B0C  (232 bytes)
    virtual void        _vf38() = 0;                          // [38] 0x18052DD10  string scan
    virtual void*       _vf39() = 0;                          // [39] 0x1819DF510  return (this+0x160)  &vector    VERIFIED &member@0x160
    virtual void        _vf40(void* p) = 0;                   // [40] 0x180EC7810  (*(void**)(this+0x130))[0x70]=p
};

// (IGameFrameworkListener lives in IGameFrameworkListener.h; C_Game implements it at +0x08 --
//  its [0] is a thunk `sub rcx,8` into the C_Game scalar deleting dtor, [1] OnPostUpdate
//  0x180FC45FC sends C_ModuleMessage 0x24, [5] OnActionEvent 0x180668FF0.)

// ---------------------------------------------------------------------------
// ILevelSystemListener — level load/unload listener (10 slots; classic SDK is 7,
// KCD2 adds a virtual dtor at [0] + two extra slots). this-adjust: C_Game+0x10.
// ---------------------------------------------------------------------------
struct ILevelSystemListener {
    virtual void _vf0(char flags) = 0;                        // [0] thunk (sub rcx,0x10) -> C_Game scalar deleting dtor
    virtual void OnLevelNotFound(const char* levelName) = 0;  // [1] 0x1839D928C  level-state @+0x30 (5->6)   tentative
    virtual void OnLoadingStart(void* pLevelInfo) = 0;        // [2] 0x180DA815C  forwards manager @+0xA0      tentative
    virtual void OnLoadingLevelEntitiesStart(void* pLevelInfo) = 0; // [3] 0x1839D929C  sends C_ModuleMessage 0x12  tentative
    virtual void OnLoadingComplete(void* pLevel) = 0;         // [4] ret 0                                    tentative
    virtual void OnLoadingError(void* pLevelInfo, const char* error) = 0; // [5] 0x1819CAF64  sends C_ModuleMessage  tentative
    virtual void OnLoadingProgress(void* pLevelInfo, int progressAmount) = 0; // [6] 0x1839D928C            tentative
    virtual void OnUnloadComplete(void* pLevel) = 0;          // [7] ret 0                                    tentative
    virtual void _vf8() = 0;                                  // [8] 0x180FC4194
    virtual void _vf9() = 0;                                  // [9] 0x180FC4248
};

// ---------------------------------------------------------------------------
// wh::framework::I_ModuleMessageListener — Warhorse module message bus (2 slots).
// this-adjust: C_Game+0x20.
// ---------------------------------------------------------------------------
struct I_ModuleMessageListener {
    virtual void _vf0(char flags) = 0;                        // [0] thunk (sub rcx,0x20) -> C_Game scalar deleting dtor
    virtual void OnModuleMessage(void* message) = 0;          // [1] 0x18047A034  wh::framework::C_ModuleMessage&  tentative
};

}  // namespace Offsets
