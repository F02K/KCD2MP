#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "../CryEngine/CryCommon/CryString.h"
#include "../Offsets/vtables/IActionListener.h"
#include "../framework/C_Signal.h"
#include "../framework/WUID.h"
#include "C_Minigame.h"
#include "C_AlchemyActionDirector.h"
#include "E_AlchemyState.h"
#include "E_AlchemyVerb.h"
#include "S_AlchemyBrewState.h"

namespace wh::entitymodule { class C_Actor; class C_Item; }

// -----------------------------------------------
// wh::playermodule::C_Alchemy -- the live brewing-minigame session (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x6D8.
// -----------------------------------------------
// RTTI .?AVC_Alchemy@playermodule@wh@@ (TD 0x184DD5670).  MI (??_R2 walk):
//   C_Minigame { I_Minigame @+0x00 (vtable 0x183F60B08 chain), I_MinigameCamera @+0x08
//   (0x183F60AD0) }, Offsets::IActionListener @+0x10 (0x183F60AB0, action-map input sink).
// ctor sub_1816E9570; allocated (0x6D8) by the typed session factory sub_1809F0234 for
// E_MinigameType::Alchemy (3); one per user, owned by the player action subsystem:
//   S_GameContext (*0x18549D388, Instance sub_1809155C8) +0x128 -> C_PlayerModule +0x18 -> owner
//   +0x20 hash(userId) -> session list.  FindOrCreateAction = sub_182024240(owner, userId, 3,
//   create, 0).  Start = sub_180897E1C(this, tableEntityId); End = I_Minigame slot [5]
//   (sub_1809F1A8C).
//
// There is NO single C_Alchemy::Update -- the session is event/input-driven: per frame
// sub_180737920 drains m_pendingVerb -> PerformVerb, then ticks the embedded director
// (sub_180737A7C); brew-state progression happens in the verb actions' Mannequin anim callbacks
// and the persistent C_PotEffectAction (fire/boil).  The earlier "Update = 0x182E907D0" claim was
// the DICE minigame (KOSTKY) -- see c_alchemy_layout.md §0.
//
// Autobrew driver surface: write m_pendingVerb (or call PerformVerb) while m_brewState.m_mode==2;
// wait for director slot 1 to clear between verbs; read the live C_AlchemyResource records in
// m_brewState.m_buckets to time boils (skill window = GetBrewTolerance()).
//
// INPUT BINDINGS (alchemy_action_bindings.md): the ctor registers the 6 "alchemy"-context input
// delegates ONCE into I_ActionSets (*(C_PlayerModule+0x60), slot [9]); End (I_Minigame [5],
// 0x1809F1A8C) unregisters them and pops the "Alchemy" control-reason token.  Context switching
// (Start/book) only toggles IActionMapManager::EnableActionMap -- registration is NEVER per-frame.

namespace wh::playermodule {

class I_AlchemyAction;

class C_Alchemy
    : public C_Minigame                 // +0x00 I_Minigame / +0x08 I_MinigameCamera
    , public Offsets::IActionListener { // +0x10 action-map input listener
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Alchemy;

    // ---- engine-function forwarders (src/playermodule/C_Alchemy.cpp) ----
    // Get (create=false) or find-or-create (create=true) the alchemy session for a user: typed
    // walk S_GameContext -> C_PlayerModule -> C_MinigameManager::FindOrCreateSession(Alchemy).
    static C_Alchemy* GetSession(uint32_t userId, bool create);
    // The skill-scaled boil/heat tolerance band (lerp AlchemyToleranceBase..MaxLevel by the
    // player's alchemy level) -- the game's ONE per-operation accuracy input for alchemy.
    static float GetBrewTolerance();                                            // 0x182E2C594
    // Configure this session for a table entity + push the "alchemy" action-map context.
    bool Start(uint32_t tableEntityId);                                         // 0x180897E1C
    // "verb" = this project's term for the alchemy interaction code (E_AlchemyVerb, 0..21).  It
    // is NOT a CryEngine input action and NOT an I_AlchemyAction object id: it selects which
    // action the (state, verb) factory builds, and doubles as the brew-state bucket/station index.
    // Create+validate+enqueue the verb's action (gate: m_brewState.m_mode == 2 and the per-verb
    // permission byte).  The action plays its real Mannequin animation and self-completes.
    void PerformVerb(E_AlchemyVerb::Type verb);                                 // 0x1815FC494
    // Permission probe: verb allowed for the current state (settings byte + create+validate dry-run).
    bool CanPerformVerb(E_AlchemyVerb::Type verb);                              // 0x1808D1F90
    // Effective brew state: m_primaryState, or m_subState while primary == Idle.
    E_AlchemyState::Type GetEffectiveState() const;                             // 0x1808D237C
    // UI name of the action the verb would create (empty if none).
    void GetVerbActionName(CryStringT<char>& out, E_AlchemyVerb::Type verb);    // 0x1808D1ECC
    // In-session cleanup = the inherited Reset() virtual (I_Minigame [21], impl 0x1806C4F1C):
    // despawns content visuals, clears buckets, hides herb/special props, teleports props home,
    // mode = Brewing, fire idle.  The raw brew-state POD re-init (sub_1816E9C58, ctor-only) is
    // deliberately NOT exposed: it zeroes m_kindEntityId and the slot-2 ambient
    // C_PotEffectAction then derefs GetEntity(0) unguarded -> crash (0x180738DC9).

    // Stock one inventory item (by its C_Item::m_wuid) onto a free herb/special station: routes
    // herb vs special internally, un-hides the station prop, stores the wuid + class-guid slot
    // key.  The autocook executor calls this per gathered ingredient.
    void ApplyIngredient(wh::framework::WUID itemWuid);                         // 0x180A955DC
    // Un-stock all herb+special stations: hide the props, sentinel the slot values, null the
    // slot keys.  (Slot-level only -- buckets/pot content are Reset()'s job.)
    void ClearIngredientSlots();                                                // 0x180A95760

    // ---- data ----
    entitymodule::C_Actor* m_pPlayerActor; // +0x18  the brewing player's actor: the autocook gate
                                         //        resolves the SAME pointer via IActorSystem::
                                         //        GetActor(session vf[22]) and both it and the
                                         //        in-session reset call sub_1808D285C on it
                                         //        (skill obj at its +0x668; the grader's
                                         //        alias of the +0x1B8 cache -- finish_grading_site.md §7)
    uint8_t  _pad20[0x10];               // +0x20
    CryStringT<char> m_currentContext;   // +0x30  current action-map context ("alchemy"/"alchemy_reading");
                                         //        swapped by SetActionMapContext (vtbl slot [34], 0x18085A3EC)
    uint8_t  _pad38[0x30];               // +0x38
    // +0x68  the brew-verdict signal, emitted at grade time (GradeBrew 0x182E16680) iff
    //        m_presentResult: (code, potion, count) with code 1 = no recipe matched (null item),
    //        0/4 = grant miss (null item), 3 = brewed a known recipe, 5 = first-ever brew of it.
    //        Sole vanilla subscriber: the GUI session router 0x1809EFFFC (registered on session
    //        creation, switch on minigame type, alchemy = cb 0x182B8E3D8 / REL::ID 357984) --
    //        which pauses the game with the reward modal ONLY for codes 3/5 with an item and
    //        shows a preset center InfoText for everything else.
    shared::C_Signal<char, entitymodule::C_Item*, uint32_t> m_onBrewResult;
    C_AlchemyActionDirector m_director;  // +0x78..+0x108  slot-indexed action driver (mode 3)
    S_AlchemyBrewState      m_brewState; // +0x108..+0x668 the whole brewing state machine
    int32_t  m_compatMask;               // +0x668  ANDed with action GetCompatFlags on dispatch
    uint32_t _pad66C;                    // +0x66C
    uint8_t  _pad670[8];                 // +0x670
    int32_t  _unk678;                    // +0x678  ctor 0 [role UNVERIFIED]
    uint32_t _pad67C;                    // +0x67C
    void*    m_pSubsystemMgr;            // +0x680  GameContext[224]+0xC8 (copied into each action +0x60)
    E_AlchemyVerb::Type m_pendingVerb;   // +0x688  None (21) = empty; drained by sub_180737920 each frame
    uint32_t _pad68C;                    // +0x68C
    uint64_t _unk690;                    // +0x690  [role UNVERIFIED]
    uint64_t _unk698;                    // +0x698  [role UNVERIFIED]
    int16_t  _unk6A0;                    // +0x6A0  ctor 0
    bool     m_presentResult;            // +0x6A2  ctor 1 (SOLE writer, 0x1816E9645) -- gates
                                         //         ONLY the m_onBrewResult emit (both raise
                                         //         sites); verdict, rewards, XP and barks run
                                         //         regardless (the 2026-07-18 "unknown potions"
                                         //         episode was the Arming reset race, not this)
    uint8_t  _pad6A3[5];                 // +0x6A3
    std::vector<E_AlchemyVerb::Type> m_performedVerbs; // +0x6A8  verb history
    C_Alchemy* m_pSelf;                  // +0x6C0  self back-pointer (ctor); also the base the bark
                                         //         emitter is invoked on (sub_182E2C61C(alc+0x6C0, ...))
    uint64_t _unk6C8;                    // +0x6C8  [role UNVERIFIED]
    float    m_qualityCoefBase;          // +0x6D0  ctor 0.0f; the accumulated brew-quality coefficient:
                                         //         X = clamp01(perk74Float + this) banded vs
                                         //         AlchemyResultCoefToFirst/SecondQuality [writer UNVERIFIED]
    uint32_t _pad6D4;                    // +0x6D4
};
static_assert(sizeof(C_Alchemy) == 0x6D8, "C_Alchemy must be 0x6D8");
static_assert(offsetof(C_Alchemy, m_onBrewResult) == 0x68, "brew-result signal at 0x68");
static_assert(offsetof(C_Alchemy, m_director) == 0x78, "director at 0x78");
static_assert(offsetof(C_Alchemy, m_brewState) == 0x108, "brew state at 0x108");
static_assert(offsetof(C_Alchemy, m_compatMask) == 0x668, "compat mask at 0x668");
static_assert(offsetof(C_Alchemy, m_pendingVerb) == 0x688, "pending verb at 0x688");
static_assert(offsetof(C_Alchemy, m_performedVerbs) == 0x6A8, "verb history at 0x6A8");

}  // namespace wh::playermodule
