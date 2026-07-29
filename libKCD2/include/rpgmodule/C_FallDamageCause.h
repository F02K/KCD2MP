#pragma once
#include "C_Cause.h"
#include "S_FallDamageData.h"

// -----------------------------------------------
// wh::rpgmodule::C_FallDamageCause -- fall-damage event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_1807252E8 operator new(32); writes the
// C_Cause<S_FallDamageData> vftable, copies the 24-byte payload to +0x08, then the own
// vftable).  Exemplar of the named-cause pattern: ~100 further C_Cause<S_*Data>
// instantiations exist (see analysis/effects_cluster/residue/cause_family.md) -- their
// payload structs are recoverable with this same factory-copy technique.
// The factory dispatches the fresh cause via sub_1809DB5A4 and deletes it if still owned.

namespace wh::rpgmodule {

class C_FallDamageCause : public C_Cause<S_FallDamageData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FallDamageCause;
};
static_assert(sizeof(C_FallDamageCause) == 0x20, "C_FallDamageCause must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
