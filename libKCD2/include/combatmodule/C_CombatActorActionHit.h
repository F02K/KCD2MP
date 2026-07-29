#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionHitParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionHit -- hit-reaction action (KCD2 1.5.6, kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionHit@combatmodule@wh@@  (TD 0x184AFBFD0).
// primary vtable 0x183A989B0 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x68
// object ctor sub_180D49D94   factory sub_180D49C28 (operator new 0xD8 -> proven sizeof)   base-chain ctor sub_180D49CF4
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionHitParams, 2, I_CombatActorActionPrivate>
//   (primary @+0x00; NO extra MI interface base). N=2 (two animation scopes) -- the ONLY leaf with N!=1;
//   the extra scope makes the AnimatedAction base 0x90 (8 bytes larger than the N=1 equivalent), so leaf
//   data begins at +0x90. m_params (HitParams, 0x18) at +0x50; C_CombatActorObject at +0x68.
//
// m_hitHelper (+0x90): an embedded-by-value C_CombatActionHelperHit (0x30; ctor sub_180D4A034, vtable
// 0x183A99D48) that carries its OWN best-score sentinel (-100000 at helper+0x20). The leaf ALSO holds a
// second score sentinel at +0xD0. Represented as raw storage (that concrete helper is a later wave).
//
// [vs KCD1] 0x118 -> 0xD8. KCD2 embeds the hit helper inline (KCD1 had 2 trailing dwords).
//
// Own primary-vtable overrides (leaf 0x183A989B0 vs base 0x183A60988): GetActionTypeId[14]=0x181A724F0,
// combat slots [26..55], OnStart[57]=0x18155EF10, OnStop[58]=0x180E7B06C, OnTick[60]=0x180D4BAD0,
// GetNameString[64]=0x18275860C. Vtable not re-enumerated here.

namespace wh::combatmodule {

class C_CombatActionHelperHit;   // embedded by value below (module-core wave)

class C_CombatActorActionHit
    : public C_CombatActorAnimatedAction<S_CombatActorActionHitParams, 2, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionHit;
    // +0x90  embedded wh::combatmodule::C_CombatActionHelperHit (by value, 0x30); raw storage.
    alignas(8) uint8_t      m_hitHelper[0x30];  // +0x90
    uint32_t                m_deferPendingC0;   // +0xC0  OnStart sub_18155EF10 @0x18155F204 sets 1 (a deferred hit is queued); OnTick sub_180D4BAD0 @0x180D4BB0A clears to 0 once the queued object's virtual[+0xE0] confirms consumption
    uint32_t                _padC4;             // +0xC4
    bool                    m_flagC8;           // +0xC8  OnStart sub_18155EF10 @0x18155F1DF = sub_1809D1614(actor+0x504 subsystem) query bool
    uint8_t                 _padC9;             // +0xC9
    uint8_t                 _padCA[6];          // +0xCA
    int64_t                 m_scoreSentinel;    // +0xD0  (init -100000; best/min-score sentinel)
};
static_assert(sizeof(C_CombatActorActionHit) == 0xD8);

}  // namespace wh::combatmodule
