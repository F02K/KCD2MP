#pragma once
#include "I_Reward.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkReward -- reward granting a perk by name (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5A858  vtable 0x183B07CF8.  sizeof 0x10 (PROVEN: make-helper sub_1810D6D70
// operator new(16); dtor 0x1812EC08C releases the CryString at +0x08 (data-12 header)).
// Give (0x180B45D48): name -> perk GUID via C_PerkDatabase (global qword_18532E150), bails
// on null GUID, else grants via soul-manager (qword_1853322A0+0xA0) vtbl[7](soulWuid, guid).

namespace wh::rpgmodule {

class C_PerkReward : public I_Reward {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkReward;
    CryStringT<char> m_perkName;   // +0x08  perk row name (C_PerkDatabase key)
};
static_assert(sizeof(C_PerkReward) == 0x10, "C_PerkReward must be 0x10 (operator new(16))");

}  // namespace wh::rpgmodule
