#pragma once
#include "I_Reward.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionReputationReward -- reward changing faction reputation
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5AB90  vtable 0x183B77D60.  sizeof 0x18 (PROVEN: make-helper sub_1813D0A58
// operator new(24), both members default-constructed to the CryString empty header).
// Give (0x182CEDA20): resolves +0x10 to a faction id (sub_180782208), then soul-manager
// (qword_1853322A0+0xA0) vtbl[5](soulWuid, copyOf+0x08, factionId).

namespace wh::rpgmodule {

class C_FactionReputationReward : public I_Reward {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionReputationReward;
    CryStringT<char> m_reputationChangeName;   // +0x08  passed to the manager call [role INFERRED]
    CryStringT<char> m_factionName;            // +0x10  resolved to faction id in Give [role INFERRED]
};
static_assert(sizeof(C_FactionReputationReward) == 0x18,
              "C_FactionReputationReward must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
