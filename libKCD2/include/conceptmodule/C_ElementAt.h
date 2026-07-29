#pragma once
#include "C_ElementAtBase.h"
#include "C_TypedPortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_ElementAt -- indexed array access
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x108, vtable 0x183B070C8 (42 slots).
// 287 corpus uses.
// -----------------------------------------------
// Ctor 0x1810D68E8, creator 0x1810D6830; rttr "wh::conceptmodule::ElementAt"
// (reg sub_18109F6D8).  Editor text: "Returns the element at a specified index in a
// sequence. Zero based index."  GetPortValue [12] 0x1815E0698: read the Index pin;
// negative -> empty variant; else GetElement with the constant selector.

namespace wh::conceptmodule {

class C_ElementAt : public C_ElementAtBase {
public:
    RTTR_ENABLE(C_ElementAtBase)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1815E0698
    void EnumerateNodeVariants() override;  // [27] 0x180EF6E30

    C_TypedPortRef<int> m_index;   // +0xC8  rttr "Index" -- In (zero-based)
};
static_assert(sizeof(C_ElementAt) == 0x108, "C_ElementAt must be 0x108");

}  // namespace wh::conceptmodule
