#pragma once
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::I_Cause -- RPG event-cause interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CE4C58  vtable 0x183A68C00 (4 slots; [1]-[3] purecall in the interface vtable).
// Causes are one-shot event payloads dispatched into a handler via double-dispatch, mirroring
// the C_Effect visitor pattern.  Concrete causes are C_Cause<TData> instantiations (~100 TDs,
// e.g. S_CombatHitData, S_FallDamageData) plus named subclasses (C_FallDamageCause,
// C_GenericReputationChangeCause).

namespace wh::rpgmodule {

class I_Cause {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Cause;
    virtual ~I_Cause() = default;             // [0] 0x00
    virtual void _vf1() = 0;                  // [1] 0x08  nullsub in C_GenericReputationChangeCause [role UNVERIFIED]
    // [2] 0x10  double-dispatch: 0x180C63948 -> handler vtbl[37](handler, this); the slot
    //     index is per cause type (visitor pattern).
    virtual void Accept(void* handler) = 0;
    // [3] 0x18  TLS-once static per type ("GenericReputationChange", ...); matches the Cause
    //     strings of the reputation_change table.
    virtual const CryStringT<char>& GetName() = 0;
};
static_assert(sizeof(I_Cause) == 0x08, "I_Cause is a vfptr-only interface");

}  // namespace wh::rpgmodule
