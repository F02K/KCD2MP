#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_TakeIngredientAction -- grab an ingredient bag/slot (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_TakeIngredientAction@playermodule@wh@@ (TD 0x184DD4788); vtable 0x183A5C308; ctor
// sub_1808D326C; factory sub_1808D31C4 for verbs 7/8/9 (herb slots, 16-byte key memcmp select at
// C_Alchemy+0x120..) and 10/11/12 (special slots via C_Alchemy+0x2B8..).  Params are EMPTY -- the
// slot choice comes from the verb/brew state, not a param (corrected vs the first-wave "(int
// slot)" guess).  Completes on "Attach"/"ActionEnd".

namespace wh::playermodule {

class C_TakeIngredientAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TakeIngredientAction;

    uint32_t m_evAttach;      // +0x68  ~CRC32("attach")
    uint32_t m_evActionEnd;   // +0x6C  ~CRC32("actionend")
    uint64_t _pad70;          // +0x70
};
static_assert(sizeof(C_TakeIngredientAction) == 0x78, "C_TakeIngredientAction must be 0x78");

}  // namespace wh::playermodule
