#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_SwitchPotParams.h"

// -----------------------------------------------
// wh::playermodule::C_SwitchPotHalterAction -- pot-halter raise/lower on the parallel channel
// (KCD2 1.5.6, kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_SwitchPotHalterAction@playermodule@wh@@ (TD 0x184DD5300); ctor sub_1815FC738;
// factory sub_1815FC678; enqueue helper sub_1815FC5F0 (assigns m_params.m_potName / m_raise from
// the request).  Sets m_actionSequenceId = 4 => runs in DIRECTOR SLOT 4 (parallel to the main
// slot-1 verb channel), so the halter can move while another verb plays.
// S_SwitchPotParams is 0x10, so the base tail shifts: m_pAlchemy @+0x60, m_pManager @+0x68.

namespace wh::playermodule {

class C_SwitchPotHalterAction : public C_AlchemyActionBase<S_SwitchPotParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SwitchPotHalterAction;

    uint64_t _pad70[2];   // +0x70  tail [contents UNVERIFIED]
};
static_assert(sizeof(C_SwitchPotHalterAction) == 0x80, "C_SwitchPotHalterAction must be 0x80");
static_assert(offsetof(C_SwitchPotHalterAction, m_pAlchemy) == 0x60, "shifted tail (0x10 params)");

}  // namespace wh::playermodule
