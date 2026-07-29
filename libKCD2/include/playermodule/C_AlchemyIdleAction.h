#pragma once
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyIdleAction -- background upper-body idle (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_AlchemyIdleAction@playermodule@wh@@ (TD 0x184DD4A58).  Plays AlchemyIdle /
// AlchemyIdleUpperBody while no verb runs.
// [Ctor/alloc size/leaf fields UNVERIFIED -- not walked; no sizeof static_assert.]

namespace wh::playermodule {

class C_AlchemyIdleAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyIdleAction;
    // leaf tail UNVERIFIED (size unknown) -- do not embed/allocate from this header
};

}  // namespace wh::playermodule
