#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::rpgmodule::C_PerkList -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// RTTI .?AVC_PerkList@rpgmodule@wh@@ (TD 0x184D539B0; COL @vtable-8 = 0x184129BF0; vtable 0x183A49FA0).
// Single-inheritance from wh::rpgmodule::I_EventListener (CHD base array @0x1843DC100, 2 entries both
// mdisp 0 -> one vtable). Embedded at the tail of each S_SoulStatBlock (block+0x1F8), so it fills
// the stat block to exactly 616 bytes. Three std::vectors (verify pass corrected the earlier
// "two {begin,end} + one vector" reading); element types of #1/#2 UNRESOLVED (flagged).

namespace wh::rpgmodule {

class C_Soul;
class I_PerkEffect;   // owned polymorphic element of m_effects (vfuncs @+16/+24; freed sub_181AB5490)

// Minimal event-listener base (vtable-only; slot layout not enumerated).
class I_EventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_EventListener;
    virtual ~I_EventListener() = default;
};

class C_PerkList : public I_EventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkList;
    C_Soul*  m_pOwner;                       // +0x08  owner soul
    std::vector<uint32_t> m_vec1;            // +0x10  trivially-destructible elems (reset = clear); element type UNRESOLVED
    std::vector<void*>    m_vec2;            // +0x18? -> actually +0x28; see note. elems destroyed via sub_18046C388; type UNRESOLVED
    std::vector<I_PerkEffect*> m_effects;    // +0x40  OWNED polymorphic perk effects (vfunc dispatch; free sub_181AB5490)
    void*    m_handle58;                     // +0x58  OWNED ptr to a lazily-alloc'd 0x70-byte stat-accum cache (alloc sub_180C3F09C: pointee+0=byte flag, pointee+8=back-ptr to this C_PerkList, 35-entry WORD accumulator @+0x24; reset sub_1811240E8; freed sub_181AB5160 in dtor sub_180966C0C) -- struct unnamed (no RTTI)
    uint8_t  m_flags60;                      // +0x60  flags (ctor &=0xF8; checked &2)
    uint8_t  _pad61[3];                      // +0x61
    int32_t  m_counter68;                    // +0x68  (ctor 0)
    uint32_t _pad6C;                         // +0x6C
};
static_assert(sizeof(C_PerkList) == 0x70, "C_PerkList must be 0x70");
static_assert(offsetof(C_PerkList, m_vec2) == 0x28, "m_vec2 at 0x28");
static_assert(offsetof(C_PerkList, m_effects) == 0x40, "m_effects at 0x40");

}  // namespace wh::rpgmodule
