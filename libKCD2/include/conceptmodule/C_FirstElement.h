#pragma once
#include "C_ElementAtBase.h"

// -----------------------------------------------
// wh::conceptmodule::C_FirstElement -- first array element
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8 (adds nothing), vtable
// 0x183B06EB0 (42 slots).  194 corpus uses.
// -----------------------------------------------
// Creator 0x1810D6954; ctor ICF-folded with Last/RandomElement to 0x1810D6A08 --
// do not attribute that EA to one class.  rttr "wh::conceptmodule::FirstElement".
// GetPortValue [12] 0x1815E0778 = GetElement with a constant-0 selector.

namespace wh::conceptmodule {

class C_FirstElement : public C_ElementAtBase {
public:
    RTTR_ENABLE(C_ElementAtBase)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1815E0778
    void EnumerateNodeVariants() override;  // [27] 0x180EF6858
};
static_assert(sizeof(C_FirstElement) == 0xC8, "C_FirstElement adds nothing");

}  // namespace wh::conceptmodule
