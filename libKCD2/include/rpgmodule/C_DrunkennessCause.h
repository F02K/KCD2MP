#pragma once
#include "C_Cause.h"
#include "S_DrunkennessData.h"

// -----------------------------------------------
// wh::rpgmodule::C_DrunkennessCause -- "Drunkenness" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182BC3A58 tracked-alloc 16, += 0x10 @0x182BC3A98).
// RTTI TD rva 0x4CE4130, vtable rva 0x3EFAB58 (4 slots): [0] dtor rva 0x9DC2C0 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x2C8A438 -> visitor+0x2A0, [3] GetTypeName rva
// 0x2C8AA70 -> "Drunkenness" (verified).  Pattern-B ctor; 4 create-sites
// (sub_182BC85B8 / sub_182BCBA24 / sub_182BCBC4C / sub_182BDB7B0).

namespace wh::rpgmodule {

class C_DrunkennessCause : public C_Cause<S_DrunkennessData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DrunkennessCause;
};
static_assert(sizeof(C_DrunkennessCause) == 0x10, "C_DrunkennessCause must be 0x10 (alloc 16 at sub_182BC3A58)");

}  // namespace wh::rpgmodule
