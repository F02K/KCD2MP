#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "E_AlchemyMode.h"
#include "E_AlchemyPotBoilingState.h"
#include "E_AlchemyStationKind.h"
#include "E_AlchemyState.h"
#include "E_AlchemyVerb.h"

// -----------------------------------------------
// wh::playermodule::S_AlchemyBrewState -- the embedded brew-state machine at C_Alchemy+0x108
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x560 (spans C_Alchemy +0x108..+0x668).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the region has its own ctor sub_1816E9A68(base=C_Alchemy+0x108) and reset
// sub_1816E9C58, and the binary passes ITS BASE as the "container" argument everywhere
// (AddResource sub_1806C49B8(container, kind, res) writes bucket = container+0x368+0x18*kind,
// which lands exactly on m_buckets below), so it is modeled as one embedded struct.  Offsets in
// comments are ABSOLUTE C_Alchemy offsets (= relative + 0x108) to match the dossiers.
//
// Station kinds (verb code == bucket index; c_alchemy_layout.md §5.1):
//   2 mortar (mill writer sub_1806C4384) · 4 retort-distill INPUT (writer sub_182E2B148, then
//   moved 4->5 by sub_1806C4D5C) · 5 retort OUTPUT · 6 pot/base (pour sub_1806C29D8; boil accrual
//   sub_18073920C) · 7-9 herb slots (add sub_1808D42D8) · 10-12 special slots (add sub_182E19644).
// The ~16 embedded 0x10/0x18 delegate/change-set objects are modeled as opaque byte blocks
// (subset wired to named handlers in the C_Alchemy ctor tail; the rest untraced).

namespace wh::playermodule {

class C_AlchemyResource;

struct S_AlchemyBrewState {
    // ---- engine-function forwarders (src/playermodule/S_AlchemyBrewState.cpp) ----
    // Average accrued boil time over the bucket records whose item class matches (exact or the
    // one-hop registry alias), converted seconds -> sandglass turns (/ the HourglassTimeout RPG
    // param, 10 s).  THE evaluator the recipe-DSL boil builtins read (core of
    // Weak/StrongBoilingTime); accumulatorIndex 0 = m_weakBoilSeconds, 1 = m_strongBoilSeconds.
    static float GetAverageBoilTurns(const std::vector<C_AlchemyResource*>& records,
                                     const CryGUID& itemClass,
                                     uint8_t accumulatorIndex);              // 0x182E2C110

    void*    m_herbSlot[3];          // +0x108..+0x118  herb-slot item values (verb 7/8/9; reset = qword_185332918 sentinel)
    CryGUID  m_herbSlotKey[3];       // +0x120..+0x150  16B slot-select keys (TakeIngredient factory memcmp) [role PARTIALLY VERIFIED]
    int32_t  m_kindEntityId[21];     // +0x150  per-kind spawned prop entity id (base+4*kind; filled by sub_180D5AE50)
    uint8_t  _padA4_1A4[0x14];       // +0x1A4
    void*    m_pPlayerCtx;           // +0x1B8  player context (-> +0x668 = alchemy-skill object; sub_182E8B9AC)
    float    m_errorProb1;           // +0x1C0  shared C_Minigame-family slot; alchemy zeroes it (reset) and has NO
    float    m_errorProb2;           // +0x1C4  reader -- the LCG mistake sim that consumes the dice twin is dice-only
    uint8_t  _pad1C8[0x30];          // +0x1C8
    uint64_t m_stateSlots[21];       // +0x1F8..+0x2A0  per-state 8B element array (array_ctor sub_1803D1670)
    E_AlchemyState::Type m_primaryState; // +0x2A0  Idle (21) = interactive idle (reader sub_1808D237C)
    E_AlchemyState::Type m_subState;     // +0x2A4  effective state while primary == Idle
    uint8_t  m_delegate2A8[0x10];    // +0x2A8  delegate slot (wired sub_1806C67B0)
    void*    m_specialSlot[3];       // +0x2B8..+0x2C8  special-ingredient slot values (verb 10/11/12)
    CryGUID  m_specialSlotKey[3];    // +0x2D0..+0x300  16B slot-select keys, mirror of m_herbSlotKey:
                                     //         change-detected write sub_182E18E9C(alc+264, slot, &guid)
                                     //         = alc+0x2D0+16*slot, from ApplyIngredient's special
                                     //         branch sub_182E145B4 (category 3)
    E_AlchemyMode::Type m_mode;      // +0x300  mode/phase byte (setter sub_1806C3094)
    uint8_t  _pad301[3];             // +0x301
    E_AlchemyVerb::Type m_lastVerb;  // +0x304  last performed verb (base-pour reads it to pick the liquid)
    uint8_t  m_flag308;              // +0x308  [role UNVERIFIED]
    uint8_t  _pad309[7];             // +0x309
    uint8_t  m_delegate310[0x10];    // +0x310  delegate (wired sub_180B66A60)
    float    m_fireIntensity;        // +0x320  cauldron heat X, integrated per frame by the pot ambient
                                     //         sub_180738CBC: pot on fire + non-empty -> ramps to the
                                     //         boil point 1.0 in WeakBoilTimeout*0.6 s and HOLDS there
                                     //         (weak boil, sustained); a bellows squeeze (sub_1806C5274,
                                     //         from the use handler 0x1806C4BA0) SETS it to
                                     //         StrongBoilFireIntensity 2.0 (no stacking), which drains
                                     //         back to 1.0 over StrongBoilTimeout*0.6 s = the strong-
                                     //         boil window per pull; off-fire/empty it decays (floor
                                     //         0.01).  Setter sub_1807391E0 (+ change observer below).
    uint32_t _pad324;                // +0x324
    uint8_t  m_fireObserver[0x18];   // +0x328  fire-intensity change observer (sub_18073C720)
    uint8_t  m_delegate340[0x18];    // +0x340
    uint8_t  m_herbChangeSet[0x10];  // +0x358  change-tracking set (sub_1806C3338)
    uint32_t m_herbChangeCounter;    // +0x368  bumped by the herb add sub_1808D42D8
    uint32_t _pad36C;                // +0x36C
    uint8_t  m_specialChangeSet[0x10]; // +0x370  change-tracking set (sub_1806C3338)
    uint32_t m_specialChangeCounter; // +0x380  bumped by the special add sub_182E19644
    uint32_t _pad384;                // +0x384
    uint8_t  m_delegate388[0x18];    // +0x388
    uint8_t  m_delegate3A0[0x18];    // +0x3A0
    uint8_t  m_delegate3B8[0x18];    // +0x3B8
    uint8_t  m_delegate3D0[0x18];    // +0x3D0
    uint8_t  m_delegate3E8[0x18];    // +0x3E8  wired sub_1809F16A8
    uint8_t  m_delegate400[0x10];    // +0x400  wired sub_182E18E58
    int32_t  m_dirty;                // +0x410  set to 3 while distilling (sub_182E1719C)
    uint32_t _pad414;                // +0x414
    uint8_t  m_hintDelegate[0x10];   // +0x418  wired sub_1809F1F2C (HUD hints / context toggles)
    bool     m_potOnFire;            // +0x428  kettle-halter position: 1 = pot over the fire.  TOGGLED
                                     //         0<->1 by the halter anim-event listener sub_1817B1CE8 on
                                     //         its "done" event; cleared by Reset (0x1806C50CA).  The
                                     //         boil gate sub_1807391C4 requires ==1 (&& !m_potMoving)
                                     //         for any heat-up/boil accrual.
    bool     m_potMoving;            // +0x429  1 while the halter swing montage runs ("moving" ->
                                     //         "done" anim events, same listener); accrual suspends
    uint8_t  _pad42A[6];             // +0x42A
    uint8_t  m_potMoveObserver[0x10];// +0x430  pot-move state observer (notified by sub_1817B1CE8)
    uint8_t  m_potBoilingState;      // +0x440  E_AlchemyPotBoilingState byte (written by sub_180738CBC)
    uint8_t  _pad441[7];             // +0x441
    uint8_t  m_boilObserver[0x10];   // +0x448  boiling-state change observer (sub_180393524)
    float    m_audioBoilIntensity;   // +0x458  CVar al_boiling_intensity feed
    uint32_t _pad45C;                // +0x45C
    uint8_t  m_delegate460[0x10];    // +0x460
    // +0x470..+0x668: the 21 per-kind resource buckets.  AddResource writes
    // bucket = containerBase + 0x368 + 0x18*kind == &m_buckets[kind].
    std::vector<C_AlchemyResource*> m_buckets[E_AlchemyStationKind::Count];  // +0x470
};
static_assert(sizeof(S_AlchemyBrewState) == 0x560, "S_AlchemyBrewState must span 0x108..0x668");
static_assert(offsetof(S_AlchemyBrewState, m_primaryState) == 0x2A0 - 0x108, "state at abs 0x2A0");
static_assert(offsetof(S_AlchemyBrewState, m_mode) == 0x300 - 0x108, "mode at abs 0x300");
static_assert(offsetof(S_AlchemyBrewState, m_fireIntensity) == 0x320 - 0x108, "fire at abs 0x320");
static_assert(offsetof(S_AlchemyBrewState, m_potOnFire) == 0x428 - 0x108, "pot position at abs 0x428");
static_assert(offsetof(S_AlchemyBrewState, m_potBoilingState) == 0x440 - 0x108, "boil state at abs 0x440");
static_assert(offsetof(S_AlchemyBrewState, m_buckets) == 0x470 - 0x108, "buckets at abs 0x470");

}  // namespace wh::playermodule
