#pragma once
#include "I_AIPuppet.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PuppetAdapter : I_AIPuppet -- abstract adapter base for the
// formation puppets (KCD2 WHGame.dll 1.5.6, kd7u).  Modeled vptr-only (0x08).
// -----------------------------------------------
// RTTI TD rva 0x4F47298; own vtable 0x184015B28 (rva 0x4015B28) -- written only
// TRANSIENTLY by the two derived ctors (C_FormationAnchorPuppet sub_1832A0390 @
// 0x1832A03A0, C_FormationSpinePointPuppet inline @0x1832A56E0) before the leaf
// vtable overwrite; never most-derived, no own create site. Which of the 25 slots it
// implements vs leaves pure is UNRESOLVED (its vtable content was not dumped), so no
// overrides are declared here -- the class stays abstract in this mirror, matching
// the binary. Both known leaves carry a WUID at +0x08; whether that member belongs to
// the adapter or to each leaf is UNVERIFIED (leaf ctors inline everything), so it is
// declared on the leaves.

namespace wh::xgenaimodule {

class C_PuppetAdapter : public I_AIPuppet {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PuppetAdapter;
};
static_assert(sizeof(C_PuppetAdapter) == 0x08, "C_PuppetAdapter modeled vptr-only");

}  // namespace wh::xgenaimodule
