#pragma once
#include "C_Quest.h"

// -----------------------------------------------
// wh::questmodule::C_ActivityUnit -- activity quest node (KCD2 1.5.6, kd7u).
// sizeof 0x150 (alloc 336).
// -----------------------------------------------
// Own vtable 0x183ED47E0; ctors sub_181F33370 / sub_181F33500 (both: alloc 336, memset,
// C_Quest ctor sub_180DC4B80, own vftable -- NO new members, virtual overrides only; the
// two wrappers build a smart-ptr/optional around the instance).

namespace wh::questmodule {

class C_ActivityUnit : public C_Quest {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActivityUnit;
};
static_assert(sizeof(C_ActivityUnit) == 0x150, "C_ActivityUnit adds no data (alloc 336)");

}  // namespace wh::questmodule
