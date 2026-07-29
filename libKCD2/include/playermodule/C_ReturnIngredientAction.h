#pragma once
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_ReturnIngredientAction -- put a held ingredient back (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ReturnIngredientAction@playermodule@wh@@ (TD 0x184DD5200); vtable 0x183B87A38.
// [Ctor/alloc size/leaf fields UNVERIFIED -- not walked; no sizeof static_assert.  Base layout is
// certified by C_AlchemyActionBase; params type inferred Empty by the sibling Take/Return pattern
// (UNVERIFIED).]

namespace wh::playermodule {

class C_ReturnIngredientAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReturnIngredientAction;
    // leaf tail UNVERIFIED (size unknown) -- do not embed/allocate from this header
};

}  // namespace wh::playermodule
