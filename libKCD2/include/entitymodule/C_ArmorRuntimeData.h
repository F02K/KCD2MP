#pragma once
#include <cstdint>
#include "C_EquippableItemRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_ArmorRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC0 (CONFIRMED: new(192)).
// -----------------------------------------------
// RTTI .?AVC_ArmorRuntimeData@entitymodule@wh@@ (TD 0x184C38CB8, COL 0x184143AC8)
// vtable 0x183A38FE0  ctor sub_18046FAB4  factory sub_1808DD4E4 (direct operator new(192)).
// Sibling holder example: C_ActorDirtHelper stores its C_ArmorRuntimeData* @holder+0x18
// (factory sub_1808DD44C).
//
// The +0xA8..+0xAE region is 7 zeroed FLAG BYTES: the ctor writes byte@A8, unaligned dword@A9 and
// word@AD (packed stores) -- typed as bytes here because C++ cannot express the unaligned dword.

namespace wh::entitymodule {

class C_ArmorRuntimeData : public C_EquippableItemRuntimeData   // +0x00  (0xA8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ArmorRuntimeData;
    uint8_t  m_flagA8;       // +0xA8  (=0)
    uint8_t  m_flagsA9[4];   // +0xA9  ctor unaligned dword-write 0 (packed flag bytes)
    uint8_t  m_flagsAD[2];   // +0xAD  ctor word-write 0
    uint8_t  _padAF;         // +0xAF
    void*    m_pB0;          // +0xB0  (=0; role unresolved)
    int32_t  m_armorParam;   // +0xB8  ctor arg a3 (global defaults pass 3 and 7; INFERRED
                             //        quality tier / clothing layer index)
    uint32_t _padBC;         // +0xBC
};
static_assert(sizeof(C_ArmorRuntimeData) == 0xC0, "C_ArmorRuntimeData must be 0xC0");
static_assert(offsetof(C_ArmorRuntimeData, m_armorParam) == 0xB8, "armor param at 0xB8");

}  // namespace wh::entitymodule
