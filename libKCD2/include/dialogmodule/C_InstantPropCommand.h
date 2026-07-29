#pragma once
#include <cstdint>
#include "C_PropCommand.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_InstantPropCommand -- instant prop attach/detach command
// (KCD2 1.5.6, kd7u).  sizeof 0x20 (alloc 32).
// -----------------------------------------------
// RTTI TD 0x184B52860; vtable 0x183A937A0 (12 slots); ctor sub_180CF67E8 (alloc 32);
// deleting-dtor [0] sub_180EB7F8C. Overrides [11] IsInstant = return 1.

namespace wh::dialogmodule::data {

class C_InstantPropCommand : public C_PropCommand {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InstantPropCommand;
    bool    m_instantFlag;   // +0x18  [role UNVERIFIED]
    uint8_t _pad19[7];       // +0x19
};
static_assert(sizeof(C_InstantPropCommand) == 0x20, "C_InstantPropCommand must be 0x20 (alloc 32)");

}  // namespace wh::dialogmodule::data
