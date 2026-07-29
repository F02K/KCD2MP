#pragma once
#include <cstdint>
#include "C_Command.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_AnimationCommand -- animation timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B534C8; vtable 0x183A93368 (14 slots); ctor sub_180CF6E28 (alloc 24);
// deleting-dtor [0] sub_180EB7FC8. Appends 4 slots [10..13] to the I_DialogCommand
// contract: getters/setters for the second fp16 time pair (+0x14/+0x16, ctor -1).
// The base +0x0F flag byte is 0xFF for the animation command block.

namespace wh::dialogmodule::data {

class C_AnimationCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AnimationCommand;
    virtual void _vf10();   // [10] get/set 2nd fp16 pair [+0x14/+0x16; exact split UNVERIFIED]
    virtual void _vf11();   // [11]
    virtual void _vf12();   // [12]
    virtual void _vf13();   // [13]

    uint32_t m_animId;      // +0x10
    uint16_t m_time2A;      // +0x14  fp16, ctor -1
    uint16_t m_time2B;      // +0x16  fp16, ctor -1
};
static_assert(sizeof(C_AnimationCommand) == 0x18, "C_AnimationCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
