#pragma once
#include <cstdint>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_StealItemsEffect -- transfer stolen items (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E7A0  vtable 0x18472F3A8.  sizeof 0xD0 (PROVEN: factory sub_182CD4A6C
// operator new(208)).  Derives C_Effect DIRECTLY (ctor sub_182CA7FF4 inlines the C_Effect
// init + owner self-register; no target-wuid slot).
// Ctor (owner, a3, a4, &itemVec): +0x10 = a3, +0x18 = a4, then builds an INLINE-ARENA vector
// of 8-byte elements: arena +0x20 (0x80 = 16 elements), arena-claimed flag +0xA0 (reserve
// helper sub_18046E158 tests/sets arena+0x80 == this+0xA0), arena pointer +0xA8, then the
// vector proper {arena* +0xB0, begin +0xB8, end +0xC0, cap +0xC8} -- reserve(16) claims the
// arena, then bulk-copies the caller's items (sub_182C9CDB0, stride 8).

namespace wh::rpgmodule {

class C_StealItemsEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StealItemsEffect;
    uint64_t m_param10;         // +0x10  ctor arg 3 [role UNVERIFIED -- thief/source WUID candidates]
    uint64_t m_param18;         // +0x18  ctor arg 4 [role UNVERIFIED]
    uint64_t m_arena[16];       // +0x20  inline storage for 16 items
    uint8_t  m_arenaClaimed;    // +0xA0  set when the inline storage backs the vector
    uint8_t  _padA1[7];         // +0xA1
    void*    m_pArena;          // +0xA8  -> &m_arena (ctor 0x182CA8078 stores this+0x20); inline-arena base ptr
    void*    m_allocArena;      // +0xB0  std::vector allocState -> &m_arena (ctor 0x182CA807F); reserve sub_18046E158 reads base+0x80 = m_arenaClaimed to grab the inline buffer
    uint64_t* m_itemsBegin;     // +0xB8  8-byte elements [item WUIDs INFERRED]
    uint64_t* m_itemsEnd;       // +0xC0
    uint64_t* m_itemsCapEnd;    // +0xC8
};
static_assert(sizeof(C_StealItemsEffect) == 0xD0, "C_StealItemsEffect must be 0xD0 (operator new(208))");

}  // namespace wh::rpgmodule
