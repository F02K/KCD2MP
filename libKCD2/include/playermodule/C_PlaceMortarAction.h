#pragma once
#include <cstdint>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PlaceMortarAction -- set the held herbs into the mortar (KCD2 1.5.6, kd7u).
// sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_PlaceMortarAction@playermodule@wh@@ (TD 0x184DD4A98); vtable 0x183A5BB90; ctor
// sub_1808D1808; factory sub_1808D1914 (brew states 2/3, verb 0/6).  Feeds the mortar bucket[2]
// that the mill writer sub_1806C4384 operates on.  [Tail fields +0x68.. not individually verified.]

namespace wh::playermodule {

class C_PlaceMortarAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlaceMortarAction;

    uint64_t _pad68[2];   // +0x68  [contents UNVERIFIED]
};
static_assert(sizeof(C_PlaceMortarAction) == 0x78, "C_PlaceMortarAction must be 0x78");

}  // namespace wh::playermodule
