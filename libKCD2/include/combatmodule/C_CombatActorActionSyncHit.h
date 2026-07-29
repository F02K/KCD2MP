#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionSyncHitParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionSyncHit -- synchronized (paired) hit-reaction action
// (KCD2 1.5.6, kd7u).  sizeof 0xC8.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionSyncHit@combatmodule@wh@@  (TD 0x184AFC7A0).
// primary vtable 0x183A993D8 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x60
// object ctor sub_180D49EBC   factory sub_180D4A144 (operator new 0xC8 -> proven sizeof)   base-chain ctor sub_180D4A0AC
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionSyncHitParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00; NO extra MI interface base). m_params (SyncHitParams, 0x10) at +0x50;
//   C_CombatActorObject at +0x60; leaf data begins at +0x80.
//
// m_hitHelper (+0x88): embedded-by-value C_CombatActionHelperHit (0x30; ctor sub_180D4A034); raw storage.
//
// [vs KCD1] 0x108 -> 0xC8. KCD2 embeds the hit helper inline; still keeps a separate sync-helper pointer.
//
// Own primary-vtable overrides (leaf 0x183A993D8 vs base 0x183A60988): GetActionTypeId[14]=0x181A7D870,
// combat slots [26..55], _vfOwn0[56]=0x18178995C, OnStart[57]=0x180D4B104, OnStop[58]=0x180D4B2C8,
// OnTick[60]=0x180D4CC9C, GetNameString[64]=0x18275860C. Vtable not re-enumerated here.

namespace wh::combatmodule {

class C_CombatActionHelperHit;    // embedded by value below (module-core wave)
class C_CombatActionEarlyExitHelper;   // owned early-exit/sync helper (alloc 0x18; ctor sub_180914FA4; RTTI-named)

class C_CombatActorActionSyncHit
    : public C_CombatActorAnimatedAction<S_CombatActorActionSyncHitParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionSyncHit;
    uint32_t                m_unknown_80;       // +0x80  (init 2)  uint32 state/query-id: OnTick 0x180D4CC9C caches HIDWORD of a subsystem vf[0x118] result, nonzero gates a predicate re-check then resets to 0
    uint32_t                m_flags_84;         // +0x84  (init 0x100)
    // +0x88  embedded wh::combatmodule::C_CombatActionHelperHit (by value, 0x30); raw storage.
    alignas(8) uint8_t      m_hitHelper[0x30];  // +0x88
    C_CombatActionEarlyExitHelper* m_pSyncHelper;    // +0xB8  (alloc 0x18)
    void*                   m_pSyncPartner;     // +0xC0  weak ptr to a paired sync object; OnStop 0x180D4B2C8 does sub_180C5385C(partner+0xA0) (releases a _smart_ptr back-ref, slot2) then nulls this. Pointee class unresolved
};
static_assert(sizeof(C_CombatActorActionSyncHit) == 0xC8);

}  // namespace wh::combatmodule
