#pragma once
#include <cstdint>
#include <vector>
#include "I_CombatScene.h"
#include "../animationmodule/I_SceneListener.h"
#include "../game/I_GameSideEffectCallback.h"
#include "../crysystem/IEntitySystemSink.h"   // replica of the stock interface (stock header needs boost)

// -----------------------------------------------
// wh::combatmodule::C_CombatScene -- owns the live combat actors and drives scene-wide combat
// relations (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_CombatScene@combatmodule@wh@@   ctor sub_180A46140 (alloc 0x78, from the module ctor).
// Four vtables at +0x00/+0x08/+0x10/+0x18. RTTI base array (5 entries -> the 4 subobjects):
//   I_CombatScene            @+0x00  (vtable 0x183A6C7D0)
//   I_SceneListener          @+0x08  (vtable 0x183A6C7A0, animationmodule)
//   IEntitySystemSink        @+0x10  (vtable 0x183A6C7F0, CryEngine; interfuscator-shuffled)
//   I_GameSideEffectCallback @+0x18  (vtable 0x183A6C7B8, game)
//
// [FUNDAMENTAL vs KCD1] sizeof 0x40 -> 0x78. KCD1 had 3 bases; KCD2 adds a fourth,
// wh::game::I_GameSideEffectCallback. The +0x30 helper (KCD1 S_CombatSceneSubObject, a plain 0x10
// struct) is now C_CombatLodManager (a polymorphic 0x18 object) and two extra std::vectors appear.
// The ctor reserves m_actors to capacity 100.

namespace wh::combatmodule {

class C_CombatActor;
class C_CombatLodManager;   // 0x18, polymorphic; built inline by the scene ctor, registered with the anim system

class C_CombatScene : public I_CombatScene,
                      public wh::animationmodule::I_SceneListener,
                      public IEntitySystemSink,
                      public wh::game::I_GameSideEffectCallback
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatScene;
    // ---- I_CombatScene ----
    ~C_CombatScene() override = default;                                         // [0] sub_1827C60D4
    void GetActorsByEntityId(uint32_t, std::vector<C_CombatActor*>&) override {} // [1] sub_180C5962C
    void RefreshActorRelations() override {}                                     // [2] sub_1809D45A4
    // ---- I_SceneListener ----
    void OnCollision(const void*, const void*, const EventPhysCollision&) override {} // nop (sub_1803B6E80)
    // ---- I_GameSideEffectCallback (2 callback slots, no dtor -- see the header) ----
    void OnSideEffectDeactivated(char sideEffectId) override;                    // [0] sub_1827C7D74 (re-caches sub_1809D385C() into m_field40)
    void OnSideEffectActivated(char sideEffectId) override {}                    // [1] nop (sub_1803B6E80)
    // (IEntitySystemSink overrides -- OnEntityEvent etc. -- provided at +0x10; not enumerated here
    //  because the CryEngine interface is interfuscator-shuffled.)

    // ---- data members (four vtable pointers occupy +0x00..+0x1F) ----
    std::vector<C_CombatActor*>  m_actors;            // +0x20  {begin,end,cap}; reserved to 100
    C_CombatLodManager*          m_pLodManager;       // +0x38  owned sub-object (0x18)
    uint64_t                     m_field40;           // +0x40  zero-init; OnSideEffectDeactivated (store 0x1827C7D8A) caches the first qword of the object returned by qword_18492D880 vfunc[5] (sub_1809D385C) [value role UNVERIFIED]
    // Scratch container rebuilt by RefreshActorRelations: per-actor relation entries (8-byte
    // smart-ptr-like elements built via sub_18192DB38 / assigned via sub_18136657C). [element type UNVERIFIED]
    std::vector<void*>           m_relationEntries;   // +0x48  {begin,end,cap}
    // Scratch entity-id list appended during RefreshActorRelations (cleared each pass). [element type UNVERIFIED]
    std::vector<uint32_t>        m_scratchEntityIds;  // +0x60  {begin,end,cap}

    // Convenience accessor: C_CombatModule::GetInstance()->m_pCombatScene (module +0x20, ctor-owned).
    [[nodiscard]] static C_CombatScene* GetInstance();   // C_CombatScene.cpp
};
static_assert(sizeof(C_CombatScene) == 0x78, "C_CombatScene must be 0x78");

}  // namespace wh::combatmodule
