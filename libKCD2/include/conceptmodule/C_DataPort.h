#pragma once
#include "C_EdgePort.h"

// -----------------------------------------------
// wh::conceptmodule::C_DataPort -- value-carrying port layer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30 (adds no data), vtable
// 0x183E25390 (32 slots).
// -----------------------------------------------
// Value flow is PULL, synchronous, depth-guarded (counter dword_1854807B8 vs a CVar
// limit): GetValue [16] = 0x1806926E4 self-AddRefs and dispatches GetValueImpl [31].
// Base GetValueImpl 0x18069254C: if HasOwnValue, ask the owning node
// (C_Node::GetPortValue, vcall+0x60); if the result is invalid, walk upstream edges
// (ForEachConnected dir 1) and take the first producer's value.  Values never cache
// in the port; the typed value lives in the producing node or a constant's variant.
// rttr creators alloc 0x30 (sub_18264B48C / sub_18264BB00).

namespace wh::conceptmodule {

class C_DataPort : public C_EdgePort {
public:
    RTTR_ENABLE(C_EdgePort)                    // [5] 0x181A6D2F0, [7] 0x1826534DC
    rttr::variant GetValue() override;         // [16] 0x1806926E4: AddRef self, dispatch [31]
    virtual rttr::variant GetValueImpl(_smart_ptr<I_Port> const& self);  // [31] base 0x18069254C: owner GetPortValue, else pull upstream
};
static_assert(sizeof(C_DataPort) == 0x30, "C_DataPort adds no data");

}  // namespace wh::conceptmodule
