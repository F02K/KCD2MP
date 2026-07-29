#pragma once
#include "C_Cause.h"
#include "S_AlchemyStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AlchemyStartedCause -- "AlchemyStarted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_1808989FC tracked-alloc 16).  RTTI TD rva 0x4D5CEF8,
// vtable rva 0x3A5A580 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x1819B04CC -> visitor+400, [3] GetTypeName 0x1819B04E0 -> "AlchemyStarted"
// (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_AlchemyStartedCause : public C_Cause<S_AlchemyStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyStartedCause;
};
static_assert(sizeof(C_AlchemyStartedCause) == 0x10, "C_AlchemyStartedCause must be 0x10 (alloc 16 at sub_1808989FC)");

}  // namespace wh::rpgmodule
