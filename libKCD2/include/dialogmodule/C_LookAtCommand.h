#pragma once
#include <cstdint>
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_LookAtCommand -- look-at timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x30 (alloc 48).
// -----------------------------------------------
// RTTI TD 0x184B53378; vtable 0x183A93508 (10 slots); ctor sub_180CF6B70 (alloc 48),
// field init sub_180CF6C04; deleting-dtor [0] sub_180EB8070. Defaults: attach point
// "MiddleCenter", weight 0.8.

namespace wh::dialogmodule::data {

class C_LookAtCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LookAtCommand;
    uint16_t m_word10;                // +0x10  [role UNVERIFIED]
    uint8_t  _pad12[6];               // +0x12
    CryStringT<char> m_attachPoint;   // +0x18  ctor "MiddleCenter"
    CryStringT<char> m_targetName;    // +0x20
    float    m_weight;                // +0x28  ctor 0.8f
    uint8_t  _pad2C[4];               // +0x2C
};
static_assert(sizeof(C_LookAtCommand) == 0x30, "C_LookAtCommand must be 0x30 (alloc 48)");

}  // namespace wh::dialogmodule::data
