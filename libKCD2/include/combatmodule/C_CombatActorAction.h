#pragma once
#include <cstdint>
#include "../framework/C_ParamAction.h"
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorAction<TPrivate, TBase> -- combat wrapper over the framework
// action stack (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// First of the three intermediate templates that replaced KCD1's C_CombatActorActionT:
//   leaf -> C_CombatActorAnimatedAction<TParams,N,TPrivate> -> C_CombatActorParamAction<TParams,TPrivate>
//        -> C_CombatActorAction<TPrivate, TBase>  (TBase = framework::C_ParamAction<TParams, C_Action<TPrivate>>)
// Proven from the RTTI BaseClassArray of every action leaf (e.g. C_CombatActorActionAttack COL
// 0x184160D80: numContainedBases shows C_CombatActorAction<> contains the C_ParamAction subtree PLUS
// C_CombatActorObject => it introduces the C_CombatActorObject MI base) and from the base-chain ctors
// (e.g. Attack sub_18091581C: C_CombatActorObject ctor sub_180914798(this+0x68), then word +0x78 = 0,
// dword +0x7C = 0; Guard base ctor sub_180916DD8 writes the same pair at +0x68/+0x6C for 8-byte params).
//
// Layout: TBase (0x50 + sizeof(TParams)), then C_CombatActorObject (0x10), then the two fields below.
// sizeof == sizeof(TBase) + 0x18.
//
// The combat wrapper layer (this class and/or C_CombatActorParamAction -- split UNVERIFIED, the two
// slots are adjacent either way) appends TWO primary vtable slots after C_Action's 65:
//   [65] flag getter -- returns the low byte of m_actorActionFlags (e.g. Guard 0x181A7D910
//        `return *(u8*)(this+0x68)`; Attack variant 0x181A7D900 reads +0x78)
//   [66] tagged-name reflection -- fills {int tag=1, CryStringT} out-param from a per-leaf static
//        string (e.g. Guard 0x182758188, Attack 0x182757FE8); every leaf overrides it.
// Slot counts proven on kd7u: C_Action<Private> = 65, plain-ParamAction leaf (Guard) = 67,
// AnimatedAction leaves = 68.

namespace wh::combatmodule {

template<typename TPrivate, typename TBase>
class C_CombatActorAction : public TBase, public C_CombatActorObject {
public:
    // ---- combat-wrapper virtuals appended after C_Action's [0..64] ----
    virtual uint8_t GetActorActionFlag() const = 0;      // [65] returns (uint8)m_actorActionFlags
    virtual void*   GetTaggedDebugName(void* out) = 0;   // [66] out = {int 1, string}; per-leaf static name

    // ---- data (offsets relative: after the C_CombatActorObject base) ----
    uint16_t m_actorActionFlags;   // +0x00 rel  (ctor 0; read by [65]; semantics UNVERIFIED)
    uint16_t _padRel02;            // +0x02 rel
    uint32_t m_actorActionField;   // +0x04 rel  (ctor 0; semantics UNVERIFIED)
};

}  // namespace wh::combatmodule
