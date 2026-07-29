#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipeStepType -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipeStepType@playermodule@wh@@ (TD 0x184DE0EF0).  One row of
// Libs/Tables/minigame/AlchemyRecipeStepType.xml: Trivial RewardXP=0 / Easy 12 / Hard 24.
// RTTR-reflected by sub_18021C5E0 (CryStringT @+0x08, uint @+0x10 from the wrapper immediates).
// Database: C_ObjectDatabase<C_AlchemyRecipeStepType> ("wh::playermodule::
// AlchemyRecipeStepTypeDatabase" @0x183AF2560, RTTR class reg sub_18021CC90).
// [Size 0x18 is the field-model size; row stride not independently observed -- UNVERIFIED tail.]

namespace wh::playermodule {

class C_AlchemyRecipeStepType {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeStepType;
    virtual ~C_AlchemyRecipeStepType() = default;

    CryStringT<char> m_name;       // +0x08  "Trivial" / "Easy" / "Hard"
    uint32_t         m_rewardXP;   // +0x10  per-step XP weight (0/12/24)
    uint32_t         _pad14;       // +0x14
};
static_assert(sizeof(C_AlchemyRecipeStepType) == 0x18, "C_AlchemyRecipeStepType must be 0x18");

}  // namespace wh::playermodule
