#pragma once
#include <cstdint>
#include <vector>
#include "I_DebugDraw.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SubbSwitchingLogic : I_DebugDraw -- subbrain switching
// logic, embedded member of C_AIBrainMultiSubb @+0x110 (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x88 (spans MultiSubb +0x110..+0x198; the MultiSubb dtor resets
// the vptr back to I_DebugDraw::vftable).
// -----------------------------------------------
// RTTI mangled .?AVC_SubbSwitchingLogic@xgenaimodule@wh@@.  Base I_DebugDraw
// proven by the ctor/dtor vptr swap (MultiSubb ctor sub_180418854 stores
// &C_SubbSwitchingLogic::vftable @0x1804188A1; dtor sub_180D4712C restores
// I_DebugDraw @0x180D471FB).  Own vtable slots beyond the interface pair NOT
// enumerated [U]; interior of the +0x28..+0x80 block zero-init only [U roles].
// Offsets below are within this object (MultiSubb offset - 0x110).

namespace wh::xgenaimodule {

class C_AIBrainMultiSubb;

class C_SubbSwitchingLogic : public I_DebugDraw {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SubbSwitchingLogic;
    void DebugDraw() override;   // [1] I_DebugDraw impl [U body address]

    std::vector<int32_t> m_indices;      // +0x08  freed as 4*count (int elems) [U element meaning]
    C_AIBrainMultiSubb*  m_ownerBrain;   // +0x20  back-ptr, = enclosing brain (ctor)
    uint8_t              m_state[0x58];  // +0x28..+0x80  memset-0 field block [U roles]
    bool                 m_bool80;       // +0x80  [U role]
    uint8_t              _pad81[7];      // +0x81
};
static_assert(sizeof(C_SubbSwitchingLogic) == 0x88, "C_SubbSwitchingLogic must be 0x88 (MultiSubb +0x110..+0x198)");

}  // namespace wh::xgenaimodule
