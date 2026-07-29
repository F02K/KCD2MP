#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PlaceIngredientAction -- drop the held ingredient into the cauldron
// (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_PlaceIngredientAction@playermodule@wh@@ (TD 0x184DD52C0); vtable 0x183A5C4C0; ctor
// sub_1808D3478; factory sub_1808D33D0 (sub-states 9..12, verb 0/2).  Plays AlchemyPutHerb;
// completes on "Detach"/"ActionEnd".  The herb C_AlchemyResource record is created by the apply
// path sub_1808D42D8 (kind = verb 7/8/9) with the non-base ctor sub_1808D4260.

namespace wh::playermodule {

class C_PlaceIngredientAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlaceIngredientAction;

    uint32_t m_evDetach;      // +0x68  ~CRC32("detach")
    uint32_t m_evActionEnd;   // +0x6C  ~CRC32("actionend")
    uint64_t _pad70;          // +0x70
};
static_assert(sizeof(C_PlaceIngredientAction) == 0x78, "C_PlaceIngredientAction must be 0x78");

}  // namespace wh::playermodule
