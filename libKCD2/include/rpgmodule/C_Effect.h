#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "../framework/WUID.h"

// ===========================================================================
// wh::rpgmodule::C_Effect -- effect-system root (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVC_Effect@rpgmodule@wh@@ (TD 0x184CE4D60)  vtable 0x183A68BB0, 9 slots (KCD1: 8).
// Branches: C_SoulEffect / C_ItemEffect / C_FactionEffect (each 9-slot override sets).
// Visitor pattern: Accept dispatches into a per-effect-type slot of C_EffectVisitor /
// C_ConstEffectVisitor (e.g. C_StatXPEffect -> visitor vtbl slot [31]/[32]); the visitor
// interfaces are only forward-declared here (S_VisitorSpecHelper templates in RTTI).
//
// [FUNDAMENTAL vs KCD1] KCD1's C_Effect carried {+0x08 unk, +0x10 m_entityId} (0x18). KCD2 splits:
// C_Effect is {vfptr, +0x08 owner} (0x10) and the ctor SELF-REGISTERS the new effect into the
// owner's pending vector @owner+0x10 (grow sub_18040F4E4 / push sub_18240E2D0) -- the target id
// moved into the branch classes (C_SoulEffect +0x10 WUID, C_FactionEffect +0x10 cause string).

namespace wh::rpgmodule {

class C_EffectVisitor;        // TD 0x184CE4C20 -- abstract, one Visit slot per effect type
class C_ConstEffectVisitor;   // TD 0x184CE8DC0
class C_Event;                 // owner type, see C_Event.h

class C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_rpgmodule_C_Effect;
    virtual ~C_Effect();                                    // [0] 0x00  scalar-deleting (0x1819CA520)
    virtual void Accept(C_EffectVisitor& v) = 0;            // [1] 0x08  v.vtbl[type slot](v, this)
    virtual void AcceptConst(C_ConstEffectVisitor& v) = 0;  // [2] 0x10
    virtual bool Apply() = 0;                               // [3] 0x18  main processing
    virtual wh::framework::WUID* GetTargetId(wh::framework::WUID& out) = 0;  // [4] 0x20  branch classes return +0x10
    // [5] 0x28  base impl 0x1809DB8BC: C_SoulList::LookupByWUID(GetTargetId()) and test
    //     C_Soul+0xCF0 flag 0x10 (bit semantics UNVERIFIED -- skip/suppress gate candidate)
    virtual bool IsTargetSoulFlagged();
    virtual void _vf6() = 0;                                // [6] 0x30  pure in base; ret-stub in concretes
    virtual void _vf7() {}                                  // [7] 0x38  shared ret stub everywhere
    // [8] 0x40  returns a TLS-init static CryStringT ("StatXP", ...) per concrete type
    virtual const CryStringT<char>& GetName() = 0;

    C_Event* m_pOwner;   // +0x08  non-owning; ctor pushes `this` into owner->m_effects (+0x10)
};
static_assert(sizeof(C_Effect) == 0x10, "C_Effect must be 0x10 (branch ctors write +0x10 themselves)");

}  // namespace wh::rpgmodule
