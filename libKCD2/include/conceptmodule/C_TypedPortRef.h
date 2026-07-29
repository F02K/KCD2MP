#pragma once
#include "C_PortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_TypedPortRef<T> -- typed node-side port handle
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x40 (adds NOTHING over C_PortRef).
// -----------------------------------------------
// 4922 instantiations in the image.  Slot-by-slot vtable dump of <bool> 0x183A460A8,
// <S_Trigger> 0x183A45E70, <S_TimeSpan> 0x183A429B8, <E_TimeType> 0x183A42A78: all
// 23 slots byte-identical except [5]/[7] (the per-T rttr trio) -- the T is ONLY an
// rttr type identity plus non-virtual templated Get()/Set() accessors, fully inlined
// into node code [bodies UNVERIFIED].  Value path: C_PortRef::GetValue() ->
// rttr::variant::convert<T>() at the consumer (constants stay raw std::string in the
// variant until this conversion).  Embedded in hosts at 0x40 stride (e.g. C_Effect
// +0x40 IsActive, C_If +0x80/+0xC0/+0x100).

namespace wh::conceptmodule {

template <typename T>
class C_TypedPortRef : public C_PortRef {
public:
    RTTR_ENABLE(C_PortRef)   // [5]/[7] per-instantiation trio
};
static_assert(sizeof(C_TypedPortRef<bool>) == 0x40, "C_TypedPortRef<T> adds nothing over C_PortRef");

}  // namespace wh::conceptmodule
