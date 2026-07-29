#pragma once
#include <functional>
#include "C_PortRef.h"
#include "C_TemplatedNode.h"

// -----------------------------------------------
// wh::conceptmodule::C_ElementAtBase -- array element-picker base
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8 (modeled), vtable 0x183B06D58
// (42 slots).  ABSTRACT in practice: no rttr constructor wrapper.
// -----------------------------------------------
// rttr "wh::conceptmodule::ElementAtBase" (reg sub_180C2C72C).  Shared pull core
// GetElement 0x1815E07A4: read Array pin -> variant; require
// type_data.flags & 0x200 (is_sequential_container) else empty variant; make a
// variant_sequential_view; idx = selector(size); out-of-range -> empty; else
// view.get_value(idx) converted to TypeT_0.  Leaves supply the selector:
// ElementAt = [Index pin], FirstElement = 0, LastElement = n-1, RandomElement =
// random (leaf lambdas undecompiled; shape PROVEN by the shared core).

namespace wh::conceptmodule {

class C_ElementAtBase : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides

    rttr::variant GetElement(std::function<size_t(size_t)> const& selector);  // 0x1815E07A4 shared pull core [sig LIKELY]

    C_PortRef m_arrayPort;   // +0x48  rttr "Array" -- In (Template = TypeT)
    C_PortRef m_valuePort;   // +0x88  rttr "Value" -- Out (Template = TypeT_0)
};
static_assert(sizeof(C_ElementAtBase) == 0xC8, "C_ElementAtBase must be 0xC8");

}  // namespace wh::conceptmodule
