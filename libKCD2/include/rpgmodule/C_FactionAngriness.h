#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::C_FactionAngriness -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50.
// -----------------------------------------------
// Faction angriness/aggro accumulator -- the KCD2 equivalent of KCD1's runtime S_Angriness
// (the S_Angriness*DefinitionID structs in KCD2 RTTI are separate definition-ID handles, NOT
// this class). TWO are embedded back-to-back in C_Faction @+0xE8/+0x138, RUNTIME-only (never
// serialized): ctor sub_1811651F4 builds them with m_flag20 = 1 / 0 (two channels; which is
// which is UNRESOLVED); array teardown sub_182CA82CC -> sub_180697F80(base, 0x50, 2, elemDtor
// sub_182CA85C4 -> sub_182CA82E4).

namespace wh::rpgmodule {

class C_Faction;

class C_FactionAngriness {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionAngriness;
    virtual ~C_FactionAngriness() = default;   // slot order UNVERIFIED
    int32_t    m_value;       // +0x08  angriness value; ctor default = global tuning table
                              //        sub_180649D30()[721] (value not dereferenced)
    uint32_t   _pad0C;        // +0x0C
    uint64_t   m_decayDeadline;// +0x10  decay-deadline timestamp (game-clock ms *(*(sub_1809155C8()+0x1A8)+0x48)); vtbl[2] 0x180474D94 sets = nodeTime + cfg[727]*3600000; setter vtbl[3] 0x1803F3910 resets to 0 when m_value set (ctor 0)
    C_Faction* m_pOwner;      // +0x18  back-ptr (ctor arg)
    bool       m_flag20;      // +0x20  channel flag: instance @+0xE8 = true, @+0x138 = false
    uint8_t    _pad21[7];     // +0x21
    uint64_t   m_signal28[5]; // +0x28..+0x50  CONFIRMED intrusive circular-sentinel observer/
                              //        signal list head (C_Signal-style; built sub_1803DF440 --
                              //        allocs a 16B node and self-links head<->node; torn down
                              //        sub_182CA82E4). Individual node fields not tiled.
};
static_assert(sizeof(C_FactionAngriness) == 0x50, "C_FactionAngriness must be 0x50");

}  // namespace wh::rpgmodule
