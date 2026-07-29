#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashBodyPart -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (family pattern: analysis/guimodule_port/
// flash_leaves.md §1). make_shared 56 in builder sub_182B0C418 (ctor 0x182B6171C).
// FillUIArgs override 0x182B62FEC (member offsets/types proven from its append calls).
// Consumer: character/wounds body view [context INFERRED]. Member names descriptive
// (semantic names UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashBodyPart : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashBodyPart;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B62FEC

    CryStringT<char> m_name;    // +0x18
    float            m_value;   // +0x20
    int32_t          m_state;   // +0x24  [may be an enum]
};
static_assert(sizeof(C_UIFlashBodyPart) == 0x28, "C_UIFlashBodyPart must be 0x28 (make_shared 56)");

}  // namespace wh::guimodule
