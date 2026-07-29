#pragma once
#include <cstdint>
#include <vector>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_PrepareItemEffect -- item-preparation effect base (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E9B8  vtable 0x18472F338.  sizeof 0x40 (INFERRED: no most-derived factory;
// both derived classes C_DryHerbEffect / C_PrepareFoodEffect are operator new(80) and begin
// their own members at +0x40).  Derives C_Effect DIRECTLY (base ctor sub_182CA7EF8 writes the
// C_Effect vftable + owner self-register inline, no target-wuid slot).
// Ctor (this, owner, &itemVec): zeroes +0x10..+0x27, copy-constructs the vector at +0x28
// (sub_1804187BC, stride-8 memmove; callers destroy their argument with vector dtor
// sub_1803E6C3C afterwards).

namespace wh::rpgmodule {

class C_PrepareItemEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PrepareItemEffect;
    std::vector<uint64_t> m_vec10;   // +0x10  ctor zeroes {begin,end,cap}; dtor sub_182CA86CC frees it via the same 8-byte vector-dtor sub_1803E6C3C used for m_items; element 8-byte, uint64_t INFERRED (mirrors m_items)
    std::vector<uint64_t> m_items;   // +0x28  8-byte elements [item WUIDs INFERRED]
};
static_assert(sizeof(C_PrepareItemEffect) == 0x40,
              "C_PrepareItemEffect must be 0x40 (derived new(80) classes start members at +0x40)");

}  // namespace wh::rpgmodule
