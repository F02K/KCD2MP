#pragma once
#include "C_ElementAtBase.h"

// -----------------------------------------------
// wh::conceptmodule::C_RandomElement -- random array element
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8 (adds nothing), vtable
// 0x183E37708 (42 slots).  16 corpus uses.
// -----------------------------------------------
// Creator 0x1810D6628; ctor ICF-folded to 0x1810D6A08 (shared with First/Last).
// rttr "wh::conceptmodule::RandomElement".  GetPortValue [12] 0x1815E074C =
// GetElement with a random selector.

namespace wh::conceptmodule {

class C_RandomElement : public C_ElementAtBase {
public:
    RTTR_ENABLE(C_ElementAtBase)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1815E074C
    void EnumerateNodeVariants() override;  // [27] 0x18270A76C
};
static_assert(sizeof(C_RandomElement) == 0xC8, "C_RandomElement adds nothing");

}  // namespace wh::conceptmodule
