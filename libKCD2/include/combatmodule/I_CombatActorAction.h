#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/smartptr.h"
#include "../framework/I_Action.h"

// -----------------------------------------------
// wh::combatmodule::I_CombatActorAction -- combat action interface (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AVI_CombatActorAction@combatmodule@wh@@  (TD 0x184AFE248; abstract -- no standalone vtable).
// Primary-chain layer: extends framework::I_Action with the combat-specific virtuals, then is itself
// extended by I_CombatActorActionPrivate, C_Action<...> and the C_ParamAction/leaf classes.
//
// The combat interface contributes primary vtable slots [26..55] (30 methods). Verified against
// C_Action<I_CombatActorActionPrivate> (0x183A60988): almost all are _purecall in the abstract base
// (leaf-implemented); a handful carry base defaults. Names are NOT recoverable from the abstract
// base alone (the impls live in the concrete attack/guard/block/... leaves, a later wave), so they
// are declared as _vfN with their base-default behavior noted. Handler addresses are the
// C_Action<...Private> slots.
//
// [vs KCD1] The KCD1 reference put the PUBLIC action queries (IsStarted/GetPriority/...) here; those
// are actually framework::I_Action slots [3..25] (see I_Action.h). This layer holds the strictly
// combat-specific virtuals.
//
// The exact boundary between I_CombatActorAction and I_CombatActorActionPrivate is NOT statically
// recoverable (both are abstract). Following the C_CombatActor.h precedent, all 30 combat slots are
// declared here and I_CombatActorActionPrivate adds none; where the split truly falls does not
// affect object layout (objects store only vtable pointers).

namespace wh::combatmodule {

class I_CombatActorAction : public wh::framework::I_Action {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorAction;
    // ---- combat action interface (primary vtable slots [26..55]) ----
    virtual int32_t _vf26() const = 0;   // [26] 0x181A72600  returns -1 (base default)
    virtual void    _vf27() = 0;         // [27] _purecall
    virtual void    _vf28() = 0;         // [28] _purecall
    virtual void    _vf29() = 0;         // [29] _purecall
    virtual void    _vf30() = 0;         // [30] _purecall
    virtual void    _vf31() = 0;         // [31] _purecall
    virtual void    _vf32() = 0;         // [32] _purecall
    virtual void    _vf33() = 0;         // [33] _purecall
    virtual void    _vf34() = 0;         // [34] _purecall
    virtual void    _vf35() = 0;         // [35] _purecall
    virtual void    _vf36() = 0;         // [36] _purecall
    virtual void    _vf37() = 0;         // [37] _purecall
    virtual bool    _vf38() const = 0;   // [38] 0x18041A6A0  returns true (base default)
    virtual int32_t _vf39() const = 0;   // [39] 0x181A72600  returns -1 (base default)
    virtual int32_t _vf40() const = 0;   // [40] 0x181A72600  returns -1 (base default)
    virtual void    _vf41() = 0;         // [41] _purecall
    virtual void    _vf42() = 0;         // [42] _purecall
    virtual void    _vf43() = 0;         // [43] 0x1803B6E80  nullsub (base default)
    virtual void    _vf44() = 0;         // [44] _purecall
    virtual void    _vf45() = 0;         // [45] _purecall
    virtual void    _vf46() = 0;         // [46] _purecall
    virtual void    _vf47() = 0;         // [47] _purecall
    virtual void    _vf48() = 0;         // [48] _purecall
    virtual void    _vf49() = 0;         // [49] _purecall
    virtual void    _vf50() = 0;         // [50] _purecall
    virtual void    _vf51() = 0;         // [51] _purecall
    virtual void    _vf52() = 0;         // [52] _purecall
    virtual void    _vf53() = 0;         // [53] _purecall
    virtual void    _vf54() = 0;         // [54] _purecall
    virtual void    _vf55() = 0;         // [55] _purecall
    // No new data members (inherits I_Action's vtable ptr + m_nRefCounter).
};
static_assert(sizeof(I_CombatActorAction) == 0x10, "I_CombatActorAction = I_Action (vtable + refcount)");

typedef _smart_ptr<I_CombatActorAction> I_CombatActorActionPtr;

}  // namespace wh::combatmodule
