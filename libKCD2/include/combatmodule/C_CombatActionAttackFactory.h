#pragma once
#include <cstdint>
#include "C_CombatActionFactoryBase.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActionAttackFactory : C_CombatActionFactoryBase -- KCD2 1.5.6 (kd7u).
// sizeof 0x10 (no members beyond the C_CombatActorObject base; m_pOwner @+0x08).
// -----------------------------------------------
// The per-actor attack-action factory. RTTI .?AVC_CombatActionAttackFactory@combatmodule@wh@@
// (TD 0x4AFA6E0). vtable 0x183BB7220 (6 slots, all inherited from C_CombatActorObject -- see
// C_CombatActionFactoryBase.h). Single-inheritance spine (all RTTI mdisp 0):
//     C_CombatActionAttackFactory : C_CombatActionFactoryBase : C_CombatActorObject
//
// ctor sub_181564AE0(out): allocates 0x10, runs C_CombatActorObject ctor sub_180914798(this, actor)
// (stores the owning C_CombatActor at +0x08 and registers the subsystem on the actor), then writes
// vtable 0x183BB7220. Created during C_CombatActorActionManager construction and stored at
// C_CombatActorActionManager+0x50 (m_pAttackFactory) -- see C_CombatActorActionManager.h.
//
// ROLE (KCD2 dispatch model). The attack-candidate pipeline reads the GLOBAL attack database
// singleton (C_CombatActionAttackDatabase @ VA 0x18531EE80) -- NOT a per-factory table. The factory
// is the per-actor CONTEXT handed to the enumerator: it supplies the attacker's live combat state
// while the enumerator walks the global moveset. Proven in the build/enumerate/create free function
// sub_18164ED68:
//     S_CombatActionAttackQueryData q; q.ctor (sub_1806881C8)
//     sub_180687BBC(attackDb, -1, &q)                 // seed query defaults from the DB
//     S_AttackCandidateResultHolder holder;
//     sub_180687F3C(context, &holder, &q)             // == S_AttackCandidateResultHolder::Enumerate
//     if (holder non-empty) { action = sub_18164F0E4(actor); action->m_candidate=..; action->AddRef; }
// where `context` resolves through the actor's action manager to this attack factory
// (*(actor+0x3A8 /*action mgr*/ +0x50 /*attack factory*/)). Sibling build/enumerate/create sites
// (all read the same global DB + a query built from actor state): sub_180939310, sub_18170D350
// (automation-attack path), sub_180C60F94, sub_180936D2C, sub_182756018.
//
// [KCD1 -> KCD2] KCD1's factory exposed non-virtual CreateAndDispatch (sub_180460934) and
// BuildAttackQuery (sub_18045FF90) as members. KCD2 keeps the 0x10 per-actor factory object but the
// build/enumerate/create logic now lives in the free dispatch functions above (factory passed as
// context); there is no factory-owned virtual or a cleanly factory-`this` public method to bind, so
// none is declared here (would be fabricated). See the dossier for the dispatch trace.

namespace wh::combatmodule {

class C_CombatActionAttackFactory : public C_CombatActionFactoryBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActionAttackFactory;
    ~C_CombatActionAttackFactory() override = default;               // [0] sub_181645C08
    // GetSubsystemId() [2] sub_181A7D5B0 / GetName() [3] sub_181A7D440 override C_CombatActorObject.
    // No additional virtuals (vtable 0x183BB7220 ends after the 6 base slots).
};
static_assert(sizeof(C_CombatActionAttackFactory) == 0x10, "C_CombatActionAttackFactory must be 0x10");

}  // namespace wh::combatmodule
