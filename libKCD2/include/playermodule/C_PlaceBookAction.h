#pragma once
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PlaceBookAction -- put the recipe book down (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_PlaceBookAction@playermodule@wh@@ (TD 0x184DD4E80).  Plays AlchemyPlaceBook /
// AlchemyCloseBook.  Part of the "alchemy_reading" book verb family.
// [Ctor/alloc size/leaf fields UNVERIFIED -- not walked; no sizeof static_assert.]

namespace wh::playermodule {

class C_PlaceBookAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlaceBookAction;
    // leaf tail UNVERIFIED (size unknown) -- do not embed/allocate from this header
};

}  // namespace wh::playermodule
