#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatModule -- singleton module owning the combat system
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI .?AVC_CombatModule@combatmodule@wh@@   primary vtable 0x183E50DB8 (7 slots).
// ctor sub_180A4602C (0x113 bytes; alloc 0x48 == sizeof), created in the module bootstrap
// sub_180C9DA44 (framework registry slot [31] @+0xF8). Publishes the undisplaced instance to
// the global qword_185482210 (RVA 0x5482210 -> C_CombatModule::GetInstance); the dtor
// sub_182776D54 clears it back to 0.
//
// [FUNDAMENTAL vs KCD1] sizeof 0xC8 -> 0x48. C_BaseModule was flattened (KCD1 C_Graph/S_Graph/
// S_Job ancestry + secondary I_ModuleMessageListener are gone -- see C_BaseModule.h), and
// C_Signal shrank 0x30 -> 0x10. What "moved out" of the module body: KCD1 kept only the scene +
// one scalar after the signal; KCD2 now also owns three named subsystems (C_BattleManager,
// C_DeadBodyManager, and a per-frame updated object) that Init allocates.
//
// === Primary vtable (0x183E50DB8, 7 slots; wh:: -> not interfuscator-shuffled) ===
//   [0] sub_182777788  scalar deleting dtor (thunk -> sub_182776D54 + free 0x48)
//   [1] sub_181147458  OnModuleMessage (I_ModuleMessageListener; dispatches on *(msg+8) type)
//   [2] sub_180D243C0  Init   -- registers the scene's entity/side-effect sinks, then allocates
//                              m_pBattleManager (0x110), m_pDeadBodyManager (0x90) and m_pUpdated (0x128)
//   [3] sub_182778C10  Deinit -- releases the three Init subsystems (framework unload path)
//   [4] sub_1805331CC  Update -- ticks m_pBattleManager, m_pDeadBodyManager, m_pUpdated
//   [5] sub_181A72E50  GetModuleId  -- returns the constant 6
//   [6] sub_181A71B00  GetModuleName -- returns "CombatModule"
// (Slot 7 position holds the next class's COL, TD 0x4B12630 -- the vtable ends at [6];
//  C_BaseModule.h now declares the correct 7-slot base.)

namespace wh::combatmodule {

class I_CombatActor;
class C_CombatScene;      // owned; ctor sub_180A46140, alloc 0x78
class C_BattleManager;    // owned; ctor sub_180D24624, alloc 0x110
class C_DeadBodyManager;  // owned; ctor sub_180D24894, alloc 0x90

class C_CombatModule : public wh::framework::C_BaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatModule;
    // ---- virtual overrides (all 7 base slots; declared for documentation) ----
    ~C_CombatModule() override = default;                                       // [0] sub_182777788
    void OnModuleMessage(void* msg) override {}                                  // [1] sub_181147458
    bool Init(void* ctx) override { return true; }                               // [2] sub_180D243C0
    void Deinit() override {}                                                    // [3] sub_182778C10  (teardown; earlier "PostInit" reading was wrong)
    void Update() override {}                                                    // [4] sub_1805331CC
    int  GetModuleId() const override { return 6; }                              // [5] sub_181A72E50
    const char* GetModuleName() const override { return "CombatModule"; }        // [6] sub_181A71B00

    // ---- data members (base C_BaseModule ends at +0x10) ----
    // Fires when a combat actor is registered/changed. Args KCD1-correlated (m_connections
    // points at the shared empty sentinel unk_185666A40 while idle).
    wh::shared::C_Signal<I_CombatActor&>  m_onCombatActorChanged;   // +0x10  (0x10)
    C_CombatScene*                        m_pCombatScene;           // +0x20  owned (ctor)
    // Non-owning cached pointer to an external subsystem (dtor never releases it). OnModuleMessage
    // dereferences its +0x450/+0x458 fields; the soul-ability query sub_1809D43FC treats it as an
    // actor-like target. Set post-Init by an external system. [UNVERIFIED role]
    void*                                 m_pExternalSubsystem;     // +0x28  non-owning; actor/soul-like target (module+0x28 passed to soul-query sub_180D5CF54 at 0x1809D44FC), OnModuleMessage derefs +0x450/+0x458 [pointee class UNVERIFIED]
    C_BattleManager*                      m_pBattleManager;         // +0x30  owned (Init)
    C_DeadBodyManager*                    m_pDeadBodyManager;       // +0x38  owned (Init)
    // Per-frame updated combat object (0x128, ctor sub_180D23FE0). Non-polymorphic (six wh::shared::
    // C_Signal heads + a -100000 int64 countdown timer at +0x118); has no RTTI, so it is left
    // unnamed. Allocated by Init, ticked by Update (fires sub_1827CF338 on timer expiry), released by
    // the dtor (free 0x128). [name UNVERIFIED -- no RTTI]
    void*                                 m_pUpdatedObject;         // +0x40  owned (Init)

    [[nodiscard]] static C_CombatModule* GetInstance();            // C_CombatModule.cpp (qword_185482210)
};
static_assert(sizeof(C_CombatModule) == 0x48, "C_CombatModule must be 0x48");

}  // namespace wh::combatmodule
