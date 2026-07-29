#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::I_Reward -- perk-reward interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5AA50, no own vtable (pure abstract, never most-derived).  5 slots read from
// the concrete vtables (C_PerkReward 0x183B07CF8 / C_FactionReputationReward 0x183B77D60 /
// C_SoulReputationReward 0x183B88E60).  [3] is one shared `return this` stub 0x1805F5DA0;
// [2]/[4] construct per-class values into out-params (helpers 0x181190D5C faction=0x181463B34)
// -- exact return types UNVERIFIED, modeled as out-pointer slots.
// Concretes are handed around as std::shared_ptr<I_Reward> (RTTI _Ref_count<I_Reward>).

namespace wh::rpgmodule {

class I_Reward {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Reward;
    virtual ~I_Reward() = default;                            // [0] 0x00
    // [1] 0x08  apply the reward: C_PerkReward 0x180B45D48 resolves its perk name in
    //     C_PerkDatabase and gives the perk to `target`'s soul via the soul-manager vtbl.
    virtual bool Give(void* target, CryStringT<char>* context, int a4) = 0;
    virtual void* _vf2(void* out) = 0;                        // [2] 0x10  fills out-value [UNVERIFIED]
    virtual I_Reward* _vf3() = 0;                             // [3] 0x18  shared `return this` stub
    virtual void* _vf4(void* out) = 0;                        // [4] 0x20  fills {qword, value} pair [UNVERIFIED]
};
static_assert(sizeof(I_Reward) == 0x08, "I_Reward is a vfptr-only interface");

}  // namespace wh::rpgmodule
