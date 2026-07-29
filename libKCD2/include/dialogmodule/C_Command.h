#pragma once
#include <cstdint>
#include "I_DialogCommand.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_Command -- timeline-command base (KCD2 1.5.6, kd7u).
// sizeof 0x10.  Abstract (no COL).
// -----------------------------------------------
// RTTI .?AVC_Command@data@dialogmodule@wh@@ (TD 0x184B531F8). Adds the shared timeline
// state under the I_DialogCommand 10-slot contract. time_a/time_b are half-precision
// (fp16) floats: [3]/[5] widen them to float, [4] ResolveTime normalizes vs the sequence
// duration (ctx+0x38) into +0x0C, [6] ResetTime copies +0x0A -> +0x0C.
// Field roles of +0x08/+0x0E/+0x0F UNVERIFIED (flag/type bytes; C_CameraCommand ctor sets
// +0x0F = 6, the animation block uses 0xFF).

namespace wh::dialogmodule::data {

class C_Command : public I_DialogCommand {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Command;
    uint16_t m_field08;   // +0x08  flags/type [role UNVERIFIED]
    uint16_t m_timeA;     // +0x0A  fp16: config timeline placement (getter [3])
    uint16_t m_timeB;     // +0x0C  fp16: resolved timeline placement (getter [5])
    uint8_t  m_flag0E;    // +0x0E  [role UNVERIFIED]
    uint8_t  m_flag0F;    // +0x0F  [role UNVERIFIED; Camera ctor = 6, Anim block = 0xFF]
};
static_assert(sizeof(C_Command) == 0x10, "C_Command must be 0x10");

}  // namespace wh::dialogmodule::data
