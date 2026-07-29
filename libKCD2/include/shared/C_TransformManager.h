#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::shared::C_TransformManager<T> -- transform-manager template, modeled
// FLAT (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x98 per the sole enumerated
// instantiation (<C_AIObject>, flattened into C_AITransformManager).
// -----------------------------------------------
// Layout from the C_AITransformManager in-place ctor sub_181177E7C: vptr @+0x0,
// +0x08..+0x88 zeroed pool/map fields (e.g. +0x08 list, +0x38 byte flag,
// +0x50 byte flag), two sub_180B941C0 handle inits, tail sub_181177FBC(this).
// 1-slot vtable ([0] deleting dtor); all other virtuals nullsub/inlined.
// Interior field types NOT expanded [U] -- modeled as a sized blob.  Template
// => no RTTI constant (gen_rtti skips templates).

namespace wh::shared {

template <class T>
class C_TransformManager {
public:
    virtual ~C_TransformManager();   // [0] deleting dtor (instantiation-specific)

    uint8_t m_state[0x90];   // +0x08..+0x98  pool/map fields, zero-init [U interior]
};
static_assert(sizeof(C_TransformManager<int>) == 0x98,
              "C_TransformManager instantiations must be 0x98");

}  // namespace wh::shared
