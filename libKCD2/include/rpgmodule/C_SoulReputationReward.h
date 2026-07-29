#pragma once
#include "I_Reward.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulReputationReward -- reward changing per-soul reputation
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5A7E8  vtable 0x183B88E60.  sizeof 0x18 (PROVEN: make-helper sub_181437D98
// operator new(24), both members default-constructed to the CryString empty header).
// Same shape as C_FactionReputationReward with the soul-flavored out-helpers
// (0x180EFC65C / 0x1816082D0).

namespace wh::rpgmodule {

class C_SoulReputationReward : public I_Reward {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulReputationReward;
    CryStringT<char> m_reputationChangeName;   // +0x08  [role INFERRED by faction-reward analogy]
    CryStringT<char> m_targetName;             // +0x10  [role INFERRED by faction-reward analogy]
};
static_assert(sizeof(C_SoulReputationReward) == 0x18,
              "C_SoulReputationReward must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
