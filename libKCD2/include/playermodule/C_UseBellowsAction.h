#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"
#include "I_AlchemyMannequinListener.h"

// -----------------------------------------------
// wh::playermodule::C_UseBellowsAction -- squeeze the bellows (KCD2 1.5.6, kd7u).  sizeof 0x98.
// -----------------------------------------------
// RTTI .?AVC_UseBellowsAction@playermodule@wh@@ (TD 0x184DD4AD8).  Vtables: primary 0x183F60260
// (the first-wave "leaf vtable 0x183F60220" was actually this class's I_ActionImpl SECONDARY --
// corrected), I_ActionImpl 0x183F60220, listener 0x183F60200 (at +0x68).  ctor sub_18161FC24;
// factory sub_18161FB78 (verb 18, never permission-blocked).  Installs its Mannequin listener via
// sub_1808C0194 into GameContext[48]+984 with callback sub_1806C64F0.  Plays AlchemyUseBellow;
// completion on "Attach"/"Detach".  Fire effect: the completion path raises the fire intensity
// (sub_1806C4BA0 -> sub_1806C5274 reads StrongBoilFireIntensity -> sub_1807391E0 writes
// C_Alchemy+0x320); repeated squeezes while distilling count toward BellowsUseCountToDistill.

namespace wh::playermodule {

class C_UseBellowsAction
    : public C_AlchemyActionBase<S_EmptyParams>
    , public I_AlchemyMannequinListener {   // +0x68 listener subobject (vtable 0x183F60200)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UseBellowsAction;

    uint32_t m_evAttach;    // +0x70  ~CRC32("attach")
    uint32_t m_evDetach;    // +0x74  ~CRC32("detach")
    uint64_t _pad78[4];     // +0x78  listener node/state [contents UNVERIFIED]
};
static_assert(sizeof(C_UseBellowsAction) == 0x98, "C_UseBellowsAction must be 0x98");
static_assert(offsetof(C_UseBellowsAction, m_evAttach) == 0x70, "crc fields after the listener base");

}  // namespace wh::playermodule
