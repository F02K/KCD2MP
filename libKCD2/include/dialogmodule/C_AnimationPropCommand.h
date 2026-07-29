#pragma once
#include <cstdint>
#include "C_PropCommand.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_AnimationPropCommand -- animated prop command
// (KCD2 1.5.6, kd7u).  sizeof 0x28 (alloc 40).
// -----------------------------------------------
// RTTI TD 0x184B532F0; vtable 0x183A93710 (12 slots); ctor sub_180CF68B8 (alloc 40);
// deleting-dtor [0] sub_180EB7FF8. Overrides [11] IsInstant = return 0. Carries a
// 16-byte animation lookup key at +0x18 (dialogue-attachment/timing DB key; internal
// split UNVERIFIED).

namespace wh::dialogmodule::data {

class C_AnimationPropCommand : public C_PropCommand {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AnimationPropCommand;
    uint8_t m_animKey[0x10];   // +0x18  16-byte anim DB lookup key [split UNVERIFIED]
};
static_assert(sizeof(C_AnimationPropCommand) == 0x28, "C_AnimationPropCommand must be 0x28 (alloc 40)");

}  // namespace wh::dialogmodule::data
