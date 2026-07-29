#pragma once
#include "C_Cause.h"
#include "S_BowDrawBeginData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BowDrawBeginCause -- "BowDrawBegin" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD4E3C tracked-alloc 24).  RTTI TD rva 0x4D5EF80,
// vtable rva 0x3F276F0 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial), [1] OVERRIDDEN ->
// 0x1823CBF70 -> sub_1809155C8 (496-byte process singleton; role [U]), [2] Accept
// 0x182C8A400 -> visitor+728, [3] GetTypeName 0x182C8A82C -> "BowDrawBegin" (verified).
// Event-factory shape.

namespace wh::rpgmodule {

class C_BowDrawBeginCause : public C_Cause<S_BowDrawBeginData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BowDrawBeginCause;
};
static_assert(sizeof(C_BowDrawBeginCause) == 0x18, "C_BowDrawBeginCause must be 0x18 (alloc 24 at sub_182CD4E3C)");

}  // namespace wh::rpgmodule
