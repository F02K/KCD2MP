#pragma once
#include "C_ElementAtBase.h"

// -----------------------------------------------
// wh::conceptmodule::C_LastElement -- last array element
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8 (adds nothing), vtable
// 0x183B07720 (42 slots).  17 corpus uses.
// -----------------------------------------------
// Creator 0x1810D6224; ctor ICF-folded to 0x1810D6A08 (shared with First/Random).
// rttr "wh::conceptmodule::LastElement".  GetPortValue [12] 0x18270A3B0 =
// GetElement with an n-1 selector.

namespace wh::conceptmodule {

class C_LastElement : public C_ElementAtBase {
public:
    RTTR_ENABLE(C_ElementAtBase)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18270A3B0
    void EnumerateNodeVariants() override;  // [27] 0x180EF6958
};
static_assert(sizeof(C_LastElement) == 0xC8, "C_LastElement adds nothing");

}  // namespace wh::conceptmodule
