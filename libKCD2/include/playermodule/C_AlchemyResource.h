#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// wh::playermodule::C_AlchemyResource -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI .?AVC_AlchemyResource@playermodule@wh@@ (TD 0x184DE1808); vtable 0x183A5CE78.  One live
// cauldron/station record of the brew state -- THE struct the recipe-condition DSL evaluates and
// an autobrew driver reads/steers.  Alloc 0x28 via the pool allocator (qword_18549D378(40,..)).
//
// Ctors: BASE liquid = sub_1806C2B08(this, pourVerb 13..16) -- sets m_isBase=1 and the liquid item
// GUID by verb (13 Spiritus / 14 Oil / 15 Wine / 16 Water, = AlchemyPotionBase.xml BaseItemId);
// created by the C_UseResourceAction "Use" anim event (sub_1806C29D8) then AddResource(kind 6).
// NON-base (herb/special) = sub_1808D4260(this, item) -- item list via sub_1808D3DC4/sub_1808D534C,
// isBase via sub_180B5D9C0; created by the herb/special apply paths (sub_1808D42D8 kinds 7-9,
// sub_182E19644 kinds 10-12).  m_quality from sub_1806C4144 (fresh/dry/decayed herb + base params).
//
// Mutation writers (VERIFIED instructions):
//   mill    sub_1806C4384: every record in mortar bucket[2]  -> m_flags |= Milled
//   distill sub_182E2B148: every record in retort bucket[4]  -> m_flags |= Distilled (then 4->5 move)
//   boil    sub_18073920C: every record in pot bucket[6]     -> m_weakBoilSeconds  += dt (WeakBoiling)
//                                                             / m_strongBoilSeconds += dt (StrongBoiling)
// DSL readers: sub_182E2C70C (milled), sub_182E2C6EC (flag test), sub_182E2C110 (boil seconds ->
// sandglass turns, / the HourglassTimeout RPG param 10s = S_Constants base+0x5F4),
// sub_182E2C5FC (find base record).
//
// Vtable (8 slots @0x183A5CE78): [0] deleting dtor sub_1808D422C; [2]/[3] sub_182A20708 returns the
// always-true C_Item predicate functor; [4] returns true (validity query); [5]/[7] trivial getters;
// [6] adjustor dtor.  Slot [1] holds a data pointer (0x184144388) -- extent/MI split UNVERIFIED, so
// virtuals are NOT declared here; access the object through its data fields only.

namespace wh::playermodule {

class C_AlchemyResource {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyResource;

    // Bitmask flags -- the writers OR these in; a record can be both milled and distilled.
    enum E_Flags : uint32_t {
        Milled    = 1u << 0,   // set by the mortar mill writer
        Distilled = 1u << 1,   // set by the retort distill writer
    };

    virtual ~C_AlchemyResource() = default;   // [0] deleting dtor sub_1808D422C (further slots not modeled, see banner)

    void*    m_pItemContext;       // +0x08  item/GUID list (registry ctx sub_1808D3DC4; GUID via sub_1804695B4 -> {+8,+0x10})
    E_Flags  m_flags;              // +0x10  bit0 milled, bit1 distilled (OR-combined, so not always a named enumerator)
    float    m_weakBoilSeconds;    // +0x14  accrued while pot state == WeakBoiling
    float    m_strongBoilSeconds;  // +0x18  accrued while pot state == StrongBoiling
    uint32_t _pad1C;               // +0x1C
    bool     m_isBase;             // +0x20  1 = base liquid record (DSL FindBase keys on this)
    uint8_t  _pad21[3];            // +0x21
    float    m_quality;            // +0x24  quality contribution (base 0.1 / fresh-dry-decayed herb params)
};
static_assert(sizeof(C_AlchemyResource) == 0x28, "C_AlchemyResource must be 0x28");
static_assert(offsetof(C_AlchemyResource, m_flags) == 0x10, "flags at 0x10");
static_assert(offsetof(C_AlchemyResource, m_quality) == 0x24, "quality at 0x24");

}  // namespace wh::playermodule
