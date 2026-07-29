#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::Callbacks::C_NoDataExtensionBase<TFunctor, N> -- the
// vptr-owning root of the Callbacks extension mix-in chain (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x18: vptr + callback-list head @+0x08 + tracking byte @+0x10.
// -----------------------------------------------
// RTTI (instantiations observed): .?AV?$C_NoDataExtensionBase@VFunctor0@@$00@
// Callbacks@xgenaimodule@wh@@ and Functor1/Functor2 variants.  Template -- no own
// TD per-spec.  Layout proven by C_Variable ctor sub_1805B810C (writes vptr@+0,
// 0->+0x08, 0->+0x10) and the C_OverwatchVariableIndex secondary subobject @+0x10
// (1-slot vtable rva 0x3AC7E78; teardown sub_1805B8308).
// MODELING NOTE [U]: the real chain has intermediates (C_Extension<X,TF,1,1> ->
// C_NoDataExtension<TF,1> -> C_PlainCallbackExtension) that add NO data and whose
// slot contribution is not separable; derived classes inherit this base directly
// here.  +0x08 is Callbacks::C_CallbackListHolder<...> (head ptr), +0x10 is
// Callbacks::C_TrackSequencing<1> (byte) -- interiors not expanded [U].

class Functor0;   // global-ns functor tag types (from the mangled instantiations)

namespace wh::xgenaimodule::Callbacks {

template <class TFunctor, int N>
class C_NoDataExtensionBase {
public:
    virtual ~C_NoDataExtensionBase() = default;   // [0] (1-slot subobject vtable observed) [U slot roles beyond dtor]

    void*   m_callbackListHead;   // +0x08  C_CallbackListHolder<...> list head [U interior]
    uint8_t m_trackSeq;           // +0x10  C_TrackSequencing<N> sequencing byte [U interior]
};
static_assert(sizeof(C_NoDataExtensionBase<Functor0, 1>) == 0x18, "Callbacks mix-in base must be 0x18");

}  // namespace wh::xgenaimodule::Callbacks
