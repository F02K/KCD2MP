#pragma once
#include <cstdint>
#include "REL/ID.h"

// -----------------------------------------------
// SSystemGlobalEnvironment (gEnv) -- KCD2 binary layout
// -----------------------------------------------
// Address: WHGame+0x492D7F8 (.data, IDA labels as gEnv)
// Non-polymorphic global; every +0x08 slot from +0x00 to +0x1B8 named.
// VERIFIED slots recovered from direct-absolute field reads + the vtable call each reader makes.
// Offsets that differ from KCD1: pPhysicalWorld +0x28->+0x38, pLog +0xE0->+0xE8,
// pFlashUI +0x130->+0x140, pHardwareMouse +0x110->+0x120.
//
// gEnv base note: CSystem keeps its gEnv pointer at CSystem+0x20, pointing at
// 0x18492D800 (== this struct base 0x18492D7F8 + 8). So a writer that stores to
// "gEnv+0xE0" in code coordinates lands at header offset +0xE8 (both = 0x18492D8E8).
// The +0x08 skew was cross-checked on three known slots: code+0xA8 -> pConsole
// (header +0xB0, 0x18492D8A8), code+0xE0 -> pLog (header +0xE8, 0x18492D8E8),
// code+0x118 -> pHardwareMouse (header +0x120, 0x18492D918).

namespace Offsets {
    struct IScriptSystem; struct IPhysicalWorld; struct IInput; struct ITimer;
    struct IGame; struct IEntitySystem; struct IConsole; struct ISystem;
    struct ILog; struct IRenderer; struct IFlashUI; struct I3DEngine;
    struct IHardwareMouse; struct ICharacterManager; struct ICryPak;
}

struct SSystemGlobalEnvironment {
    void*                       pDialogSystem;          // +0x00
    void*                       p3DEngine_DEAD;         // +0x08  guarded dead slot (as in KCD1)
    Offsets::I3DEngine*         p3DEngine;              // +0x10  VERIFIED live I3DEngine* (CScriptBind_System +
                                                        //        e_TimeOfDay handler dispatch through it, 833 refs;
                                                        //        earlier "pNetwork" label was wrong)
    void*                       pLobby;                 // +0x18
    void*                       _unk20;                 // +0x20
    void*                       _unk28;                 // +0x28
    Offsets::IScriptSystem*     pScriptSystem;          // +0x30  VERIFIED: GetGlobalValue/ExecuteBuffer
    Offsets::IPhysicalWorld*    pPhysicalWorld;         // +0x38  VERIFIED: RayWorldIntersection (was +0x28)
    void*                       _unk40;                 // +0x40
    Offsets::IInput*            pInput;                 // +0x48  VERIFIED: AddEventListener("C_Keybinds")
    void*                       _unk50;                 // +0x50
    Offsets::ICryPak*           pCryPak;                // +0x58  VERIFIED: IActorSystem::Scan 0x180E46CE8 runs the FindFirst/FindNext wildcard loop (vtable +0x1F8/+0x200) on qword_18492D850, matching CCryPak vtable 0x183a97328 slots [63]/[64]; single most-referenced gEnv slot (681 reads); KCD1 pCryPak also +0x58.
    void*                       _unk60;                 // +0x60
    void*                       _unk68;                 // +0x68
    void*                       _unk70;                 // +0x70
    void*                       _unk78;                 // +0x78
    void*                       _unk80;                 // +0x80
    Offsets::ITimer*            pTimer;                 // +0x88  VERIFIED: GetCurrTime
    void*                       _unk90;                 // +0x90
    Offsets::IGame*             pGame;                  // +0x98  VERIFIED: GetIGameFramework chain
    void*                       _unkA0;                 // +0xA0
    Offsets::IEntitySystem*     pEntitySystem;          // +0xA8  VERIFIED: GetEntity(entityId)
    Offsets::IConsole*          pConsole;               // +0xB0  VERIFIED: GetCVar/RegisterVariable
    void*                       _unkB8;                 // +0xB8
    void*                       _unkC0;                 // +0xC0
    Offsets::ISystem*           pSystem;                // +0xC8  VERIFIED: == CSystem self
    void*                       _unkD0;                 // +0xD0
    Offsets::ICharacterManager* pCharacterManager;      // +0xD8  ICharacterManager* set by the CryAnimation engine module (sub_18099B504 @0x18099B672 stores its manager to gEnv+0xD8, also mirrored to g_pCharacterManager qword_185189BE0; module string "CEngineModule_CryAnimation")
    void*                       _unkE0;                 // +0xE0  NOT pCharacterManager (that is +0xD8, proven via CryAnimation module). Inferred pAISystem: reader sub_1803CF0D0 @0x1803CF0F3 calls vtable[0x1D0] (slot 58, a very large iface); KCD1 pAISystem +0xD8 -> +0xE0 with the +8 shift; 281 reads. UNVERIFIED interface name.
    Offsets::ILog*              pLog;                   // +0xE8  VERIFIED: Log/LogError (was +0xE0)
    void*                       _unkF0;                 // +0xF0
    void*                       _unkF8;                 // +0xF8
    void*                       _unk100;                // +0x100
    void*                       _unk108;                // +0x108
    Offsets::IRenderer*         pRenderer;              // +0x110  VERIFIED: renderer vtable calls
    void*                       _unk118;                // +0x118
    Offsets::IHardwareMouse*    pHardwareMouse;         // +0x120  VERIFIED: writer sub_18079FFD4 @ 0x1807A110F stores (CHardwareMouse*)+8 (the IHardwareMouse subobject) into gEnv->pHardwareMouse; abs 0x18492D918. Reader: CHardwareMouse dtor sub_1824226F0 unregisters from gEnv->pInput. (KCD1 was +0x110; that slot is now pRenderer in KCD2.)
    void*                       _unk128;                // +0x128
    void*                       _unk130;                // +0x130
    void*                       _unk138;                // +0x138
    Offsets::IFlashUI*          pFlashUI;               // +0x140  VERIFIED: abs 0x18492D938 = the CFlashUI self-singleton (qword_18492D938) read by the whole CFlashUI method family via its own vtable (e.g. GetUIElement(int) +0xA8); CFlashUI::Shutdown (vtable [10] sub_1835C0FB0 @ 0x1835C1265) zeroes it. GetUIElement("hud"). (was +0x130)
    void*                       _unk148;                // +0x148
    void*                       _unk150;                // +0x150
    void*                       _unk158;                // +0x158
    void*                       _unk160;                // +0x160
    void*                       _unk168;                // +0x168
    void*                       _unk170;                // +0x170
    void*                       pAudioSystem;           // +0x178  audio system singleton = CB_FmodStudioWrapper*; its ctor sub_180E3C9E4 (sets *this = CB_FmodStudioWrapper::vftable) stores `this` here at 0x180E3D06C
    void*                       _unk180;                // +0x180
    void*                       _unk188;                // +0x188
    void*                       _unk190;                // +0x190
    void*                       _unk198;                // +0x198
    void*                       _unk1A0;                // +0x1A0
    void*                       _unk1A8;                // +0x1A8
    uint32_t                    mMainThreadId;          // +0x1B0  GetCurrentThreadId() cached at init (sub_18079FFD4: 0x1807A002D call GetCurrentThreadId -> 0x1807A0038 mov cs:dword_18492D9A8, eax); 4-byte scalar, NOT a pointer
    uint8_t                     _pad1B4[4];             // +0x1B4
    void*                       _unk1B8;                // +0x1B8

    static SSystemGlobalEnvironment* GetInstance()
    {
        return reinterpret_cast<SSystemGlobalEnvironment*>(REL::ID(1300).address());
    }
};
static_assert(sizeof(SSystemGlobalEnvironment) == 0x1C0);
