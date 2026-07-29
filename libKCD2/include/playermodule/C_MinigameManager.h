#pragma once
#include <cstdint>
#include <cstddef>
#include <map>
#include "E_MinigameType.h"

// -----------------------------------------------
// wh::playermodule::C_MinigameManager -- the minigame-session + scriptbind owner
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB0.  [Class name SYNTHETIC -- the object is
// NON-POLYMORPHIC (no vtable, no RTTI); named for what it verifiably owns.]
// -----------------------------------------------
// Built by ctor sub_180EF7184 (alloc 0xB0) and stored at C_PlayerModule+0x18 by the module ctor
// sub_180BE9208.  Owns (a) the per-user minigame-session multimap and (b) the 10 minigame
// scriptbinds (each ctor writes its ??_7 vtable and registers its Lua table name -- all 10
// identities below are vtable-certified from the ctor at 0x180EF71E6..0x180EF7327).
//
// Session lifecycle: FindOrCreateSession = sub_182024240 (equal_range on m_sessions by userId via
// sub_1809E6980, match I_Minigame slot [0] GetMinigameType, skip finished via slot [7] unless
// includeFinished); absent+create -> typed factory sub_1809F0234 (gate: userId resolves via
// S_GameContext+0x180 IActorSystem::GetActor; registers the new session via sub_182025BA0 +
// sub_1803A524C).  Session node value sits at tree-node+0x28 (multimap<u32, ptr> pair layout).
//
// +0x00/+0x10 slot heads: sub_180EF7360 writes { &unk_1856681D0 (static sentinel, first qword
// -1), int16 -1, int16 -1 } [role UNRESOLVED]; +0xA8 callback head installed by sub_180515D90
// (functor from sub_180EF7610) [role UNRESOLVED].

namespace wh::playermodule {

class I_Minigame;
class C_ScriptBind_Alchemy;
class C_ScriptBind_Sharpening;
class C_ScriptBind_StoneThrowing;
class C_ScriptBind_Blacksmithing;
class C_ScriptBind_ForgeBuilder;
class C_ScriptBind_Dice;
class C_ScriptBind_HoleDigging;
class C_ScriptBind_ShootingContest;
class C_ScriptBindMinigame;
class C_FastTravelScriptBind;

class C_MinigameManager {
public:
    // 12-byte {static-sentinel ptr, idx, gen} handle head (init sub_180EF7360) [role UNRESOLVED]
    struct S_HandleSlot {
        void*   m_pSentinel;   // &unk_1856681D0
        int16_t m_idx;         // -1
        int16_t m_gen;         // -1
    };

    // Find the live session of `type` for a user; when absent and `create`, build it through the
    // typed factory (sub_1809F0234).  includeFinished=false skips sessions whose I_Minigame
    // slot [7] reports finished.  (src/playermodule/C_MinigameManager.cpp)
    I_Minigame* FindOrCreateSession(uint32_t userId, E_MinigameType::Type type,
                                    bool create, bool includeFinished);   // 0x182024240

    S_HandleSlot m_slot00;                             // +0x00  [role UNRESOLVED]
    S_HandleSlot m_slot10;                             // +0x10  [role UNRESOLVED]
    std::multimap<uint32_t, I_Minigame*> m_sessions;   // +0x20  userId -> live sessions (one per type)
    C_ScriptBind_Alchemy*         m_pScriptBindAlchemy;         // +0x30  Lua "Alchemy"   (ctor 0x180EF737C)
    C_ScriptBind_Sharpening*      m_pScriptBindSharpening;      // +0x38  Lua "Sharpening" (0x180EF7400)
    C_ScriptBind_StoneThrowing*   m_pScriptBindStoneThrowing;   // +0x40  Lua "StoneThrowing" (0x180EF789C)
    C_ScriptBind_Blacksmithing*   m_pScriptBindBlacksmithing;   // +0x48  Lua "Blacksmithing" (0x180EF77D0)
    C_ScriptBind_ForgeBuilder*    m_pScriptBindForgeBuilder;    // +0x50  Lua "ForgeBuilder" (0x182040F70)
    C_ScriptBind_Dice*            m_pScriptBindDice;            // +0x58  Lua "Dice"       (0x180EF7508)
    C_ScriptBind_HoleDigging*     m_pScriptBindHoleDigging;     // +0x60  Lua "HoleDigging" (0x180EF7738)
    C_ScriptBind_ShootingContest* m_pScriptBindShootingContest; // +0x68  Lua "ShootingContest" (0x180EF758C)
    C_ScriptBindMinigame*         m_pScriptBindMinigame;        // +0x70  Lua "Minigame"   (0x180EF7484)
    C_FastTravelScriptBind*       m_pFastTravelScriptBind;      // +0x78  Lua "FastTravel" (0x180EF76A0)
    uint64_t m_unk80;                                  // +0x80  ctor 0 [role UNRESOLVED]
    uint64_t m_unk88;                                  // +0x88  ctor 0 [role UNRESOLVED]
    uint64_t m_unk90;                                  // +0x90  ctor 0 [role UNRESOLVED]
    uint64_t m_unk98;                                  // +0x98  ctor 0 [role UNRESOLVED]
    uint64_t m_unkA0;                                  // +0xA0  ctor 0 [role UNRESOLVED]
    uint64_t m_callbackA8;                             // +0xA8  callback head (sub_180515D90) [role UNRESOLVED]
};
static_assert(sizeof(C_MinigameManager) == 0xB0, "C_MinigameManager must be 0xB0");
static_assert(offsetof(C_MinigameManager, m_sessions) == 0x20, "session multimap at 0x20");
static_assert(offsetof(C_MinigameManager, m_pScriptBindAlchemy) == 0x30, "Alchemy scriptbind at 0x30");
static_assert(sizeof(std::multimap<uint32_t, I_Minigame*>) == 0x10,
              "MSVC multimap must be {head, size} for the +0x20 layout");

}  // namespace wh::playermodule
