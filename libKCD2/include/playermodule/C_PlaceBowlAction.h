#pragma once
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PlaceBowlAction -- set the herb bowl down (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_PlaceBowlAction@playermodule@wh@@ (TD 0x184DD5410); vtable 0x183F60900; ctor
// sub_182E13218 (finish/bowl phase factory).  Handles the HerbsBow/HerbsBowMilled bowl props.
// [Alloc size + leaf fields UNVERIFIED -- ctor not walked; no sizeof static_assert.  Base layout
// (through m_pManager @+0x60) is certified by C_AlchemyActionBase.]

namespace wh::playermodule {

class C_PlaceBowlAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlaceBowlAction;
    // leaf tail UNVERIFIED (size unknown) -- do not embed/allocate from this header
};

}  // namespace wh::playermodule
