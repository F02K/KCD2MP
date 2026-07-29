#pragma once
#include <cstdint>
#include "guimodule/S_CutsceneEventDesc.h"

// -----------------------------------------------
// wh::guimodule::C_CutsceneEvent -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CE1BC8; vtable 0x183EF7D18 (slot roles NOT dumped -- UNVERIFIED beyond
// the dtor). ctor sub_182BB7AA0; creator sub_182BB6B2C (`sub_1804F75C0(0x40)`) --
// NOTE: that creator actually constructs the SUBCLASS C_CustomTextEvent (TD 0x184CE2458,
// vtable 0x184726960) over this layout; C_CustomTextEvent adds no data and is not
// separately modeled this wave.

namespace wh::guimodule {

class C_CutsceneEvent {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutsceneEvent;
    virtual ~C_CutsceneEvent();   // [0]; further slots UNVERIFIED

    S_CutsceneEventDesc m_desc;   // +0x08  (0x30)
    int32_t  m_int38;             // +0x38  ctor 0 [role UNVERIFIED]
    uint8_t  _pad3C[4];           // +0x3C
};
static_assert(sizeof(C_CutsceneEvent) == 0x40, "C_CutsceneEvent must be 0x40 (creator sub_182BB6B2C)");

}  // namespace wh::guimodule
