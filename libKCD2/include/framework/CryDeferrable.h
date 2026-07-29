#pragma once
#include <cstdint>

// -----------------------------------------------
// UnsafeOp::CryDeferrable<N> / UnsafeOp::ICryDeferrable -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Empty, non-polymorphic deferred-operation tag/interface mixins: zero data, no vtable.
// Reused as MI bases across many Warhorse classes (13-14 RTTI referrers). CryDeferrable<N> derives
// ICryDeferrable. As trailing empty MI bases each occupies exactly 1 byte under MSVC x64 (i.e. do NOT
// apply __declspec(empty_bases)); they carry no state and their ctors write nothing.
// RTTI: .?AVICryDeferrable@UnsafeOp@@ (TD 0x184AFE060); .?AV?$CryDeferrable@$0A@@UnsafeOp@@ (TD 0x184AFE090).

namespace UnsafeOp {

struct ICryDeferrable {};                    // empty leaf interface

template<int N>
struct CryDeferrable : ICryDeferrable {};    // empty; C_Actor uses the N == 0 specialization

// STATEFUL polymorphic 0x10 variant of the deferrable mixin, distinct from the empty tag above.
// Seen in wh::rpgmodule::C_Soul: once as an RTTI-listed BASE @+0x10 and once more as a plain
// MEMBER @+0x20 (not in the CHD). ctor sub_181FDE920 writes {vptr, m_deferSlot = -1}; dtor
// sub_181FDE9B0 unqueues the pending deferred op. Whether this is a different CryDeferrable
// template instantiation or a separate class is UNVERIFIED -- the name here is SYNTHETIC.
struct CryDeferrableSlot {
    virtual ~CryDeferrableSlot() = default;
    int32_t m_deferSlot = -1;   // +0x08  deferred-op queue slot, -1 = not queued
    uint8_t _pad0C[4];          // +0x0C
};
static_assert(sizeof(CryDeferrableSlot) == 0x10, "CryDeferrableSlot must be 0x10");

}  // namespace UnsafeOp
