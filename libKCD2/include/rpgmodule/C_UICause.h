#pragma once
#include "C_Cause.h"
#include "S_UIData.h"

// -----------------------------------------------
// wh::rpgmodule::C_UICause -- "UI" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_1811A0BD0 tracked-alloc 16).  RTTI TD rva 0x4D5C968,
// vtable rva 0x3B270C8 (4 slots): [0] dtor rva 0x19BC354 (own body -- releases string),
// [1] nullsub, [2] Accept rva 0x1789254 -> visitor+280, [3] GetName rva 0x1789788 -> "UI"
// (verified; string in byte_183C125A0).  Factory caller sub_1811A0A64.

namespace wh::rpgmodule {

class C_UICause : public C_Cause<S_UIData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICause;
};
static_assert(sizeof(C_UICause) == 0x10, "C_UICause must be 0x10 (alloc 16 at sub_1811A0BD0)");

}  // namespace wh::rpgmodule
