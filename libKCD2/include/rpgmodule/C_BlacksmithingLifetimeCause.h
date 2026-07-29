#pragma once
#include "C_Cause.h"
#include "S_BlacksmithingLifetimeData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BlacksmithingLifetimeCause -- "BlacksmithingLifetime" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_180B682AC tracked-alloc 24).  RTTI TD rva 0x4D5D0D0,
// vtable rva 0x3A7BAE8 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial), [1] nullsub,
// [2] Accept 0x180C3EC88 -> visitor+96, [3] GetTypeName 0x1818482CC ->
// "BlacksmithingLifetime" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_BlacksmithingLifetimeCause : public C_Cause<S_BlacksmithingLifetimeData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlacksmithingLifetimeCause;
};
static_assert(sizeof(C_BlacksmithingLifetimeCause) == 0x18,
              "C_BlacksmithingLifetimeCause must be 0x18 (alloc 24 at sub_180B682AC)");

}  // namespace wh::rpgmodule
