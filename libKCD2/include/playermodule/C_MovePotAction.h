#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_MovePotAction -- swing the cauldron on/off the fire (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_MovePotAction@playermodule@wh@@ (TD 0x184DD53D8); vtable 0x183A5C150; ctor
// sub_1808D254C; factory sub_1808D24A4 (verbs 6 and 20 = the two directions; also input handler
// #6 writes pending verb 20 directly).  Completes on "ActionEnd".  The pot position gates boiling
// (fire intensity ramp in C_PotEffectAction) and the alchemyPotUpPosition helper joint.

namespace wh::playermodule {

class C_MovePotAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MovePotAction;

    uint32_t m_evActionEnd;   // +0x68  ~CRC32("actionend")
    uint32_t _pad6C;          // +0x6C
    uint64_t _pad70;          // +0x70
};
static_assert(sizeof(C_MovePotAction) == 0x78, "C_MovePotAction must be 0x78");

}  // namespace wh::playermodule
