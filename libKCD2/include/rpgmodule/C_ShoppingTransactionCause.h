#pragma once
#include "C_Cause.h"
#include "S_ShoppingTransactionData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ShoppingTransactionCause -- "ShoppingTransaction" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x80 (PROVEN: create fn sub_180785CA0 tracked-alloc 128, += 0x80).  RTTI TD rva
// 0x4D5F370, vtable rva 0x3A52580 (4 slots): [0] dtor rva 0x19B0EA8 (own body -- releases
// the 3 vectors), [1] nullsub, [2] Accept rva 0x9DC174 -> visitor+224, [3] GetName rva
// 0xE7C3B8 -> "ShoppingTransaction" (verified).  Factory caller sub_180785B8C.

namespace wh::rpgmodule {

class C_ShoppingTransactionCause : public C_Cause<S_ShoppingTransactionData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ShoppingTransactionCause;
};
static_assert(sizeof(C_ShoppingTransactionCause) == 0x80,
              "C_ShoppingTransactionCause must be 0x80 (alloc 128 at sub_180785CA0)");

}  // namespace wh::rpgmodule
