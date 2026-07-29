#pragma once
#include <cstdint>
#include <cstddef>
#include "I_DialogueExtension.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueHaggleExtension -- trade-price (haggle) minigame extension
// (KCD2 1.5.6, kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI TD 0x184B4FD10; vtable 0x183E67CF0 (11 slots, I_DialogueExtension contract);
// ctor sub_181E6B480.
// [1] OnStart sub_181E6BC30 reads the 8 haggle ICVars from off_1840AC1D0:
//     shop_min, shop_max, shop_keeper_price, shop_your_price, shop_progress,
//     shop_original_price, negotiation_input_reason, shop_haggle_step
//     then opens the haggle UI (sub_181E6C3C0).
// [6] OnPriceEntered(int price, bool end) sub_181E6C620: price == 0x80000000 => cancel;
//     else fires UI action "shop_input_price"; stores the accepted flag @+0x2C.
// [7] GetResult returns +0x2C (gates the following branch via the "ExtensionAccepted"
//     EE intrinsic).
// CHEAT: the 8 shop_* ICVars are the LIVE haggle state -- set shop_your_price /
// shop_progress / shop_keeper_price to win trades; forcing [6] with a low price submits
// it; +0x2C is the accepted flag. Lua DialogModule.ResetHaggle resets the minigame.

namespace wh::dialogmodule {

class C_DialogInstance;

class C_DialogueHaggleExtension : public I_DialogueExtension {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueHaggleExtension;
    bool     m_active;        // +0x08  extension live
    uint8_t  _pad09[7];       // +0x09
    C_DialogInstance* m_pOwner;  // +0x10  owning dialog (ctor sub_181E6B480 arg a2)
    uint64_t _q18;            // +0x18  [not walked]
    void*    m_pContext;      // +0x20  ctor zeroes; OnStart sub_181E6BC30 reads it as a qword (v27) into the shop-UI submit params; no writer located [type UNVERIFIED]
    uint32_t _d28;            // +0x28  [not walked]
    uint8_t  m_accepted;      // +0x2C  price-accepted result (GetResult) <-- CHEAT
    uint8_t  _pad2D[3];       // +0x2D
};
static_assert(sizeof(C_DialogueHaggleExtension) == 0x30, "C_DialogueHaggleExtension must be 0x30");
static_assert(offsetof(C_DialogueHaggleExtension, m_accepted) == 0x2C);

}  // namespace wh::dialogmodule
