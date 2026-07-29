#pragma once
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PickBookAction -- pick the recipe book up (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_PickBookAction@playermodule@wh@@ (TD 0x184DD4D58).  Plays AlchemyPickBook.  Part of
// the "alchemy_reading" book verb family with C_OpenBookAction (verb 19) / C_PlaceBookAction.
// [Ctor/alloc size/leaf fields UNVERIFIED -- not walked; no sizeof static_assert.]

namespace wh::playermodule {

class C_PickBookAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickBookAction;
    // leaf tail UNVERIFIED (size unknown) -- do not embed/allocate from this header
};

}  // namespace wh::playermodule
