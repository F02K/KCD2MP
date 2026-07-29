#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyFeedbacks -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// Nested-Type enum wrapper (RTTR, 17 values), registered by sub_18021EBF0; enumerators read
// directly from the registration's (name,value) initializers.
// Stored as int32 on C_AlchemyFeedback::m_type (+0x0C) but written/compared as a BYTE on the
// brew-state feedback slot (brewState+0x19) and in the row lookup sub_182E2BCBC.
//
// Values 5..16 are written by the DSL condition evaluators (per-step feedback); 1..4 are
// AGGREGATE feedbacks chosen at finish by sub_182E169C8 from the correctness flags
// (brewState+0x2C/+0x2D) / failed-step count / spoiled-resource scan (sub_1806C40D4).

namespace wh::playermodule {

struct E_AlchemyFeedbacks {
    enum Type : int32_t {
        None                        = 0,
        WrongIngredients            = 1,   // aggregate: no correct ingredient present (bs+0x2D cleared)
        RightIngredientsWrongBase   = 2,   // aggregate: PotionBase() failed, ingredients ok (bs+0x2C cleared)
        TooManyFailedSteps          = 3,   // aggregate: failedStepCount > tolerance
        DecayedHerbs                = 4,   // aggregate: spoiled ingredient present
        IngredientMilled            = 5,   // IsMilled('g',0) but milled
        IngredientNotMilled         = 6,   // IsMilled('g',1) but not milled
        IngredientDistilled         = 7,   // IsBaseDistilled(0) but distilled
        IngredientNotDistilled      = 8,   // IsBaseDistilled(1) but not distilled
        IngredientOvercooked        = 9,   // boil ratio too high
        IngredientUnderdone         = 10,  // boil ratio too low
        SmallerAmountsOfIngredients = 11,  // IngredientCount too low
        BiggerAmountsOfIngredients  = 12,  // IngredientCount too high
        HighCookingTemperature      = 13,  // weak boil wanted, strong-boiled
        LowCookingTemperature       = 14,  // strong boil wanted, weak-boiled
        ResultMilled                = 15,  // IsResultMilled(0) but milled
        ResultNotMilled             = 16,  // IsResultMilled(1) but not milled
    };
};

}  // namespace wh::playermodule
