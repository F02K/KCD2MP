#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyVerb -- the alchemy interaction code (KCD2 WHGame.dll 1.5.6, kd7u).
// Enum wrapper, Type : int32.
// -----------------------------------------------
// [SYNTHETIC NAME] -- "verb" is this project's term (see C_Alchemy.h): the code passed to
// C_Alchemy::PerformVerb and stored in m_pendingVerb / m_performedVerbs / m_lastVerb.  The domain
// is the shared station code space (verb == E_AlchemyStationKind bucket index == E_AlchemyState
// carry code): a verb means "interact with station N", and what that does depends on the current
// state -- the (state, verb) action factory sub_1808D1FF8 picks the action class
// (table_enable_and_verbs.md §3).  Names describe the IDLE-state action; while holding an object,
// verbs 0/2/6 place/pour it and verb == state returns it.  Code 17 never appears in the factory
// switch; 21 = None sentinel (m_pendingVerb drained) and the bucket count.

namespace wh::playermodule {

struct E_AlchemyVerb {
    enum Type : int32_t {
        TakeBowl      = 0,   // C_TakeObjectAction(0), gate bucket 0 non-empty; place target while holding
        TurnSandglass = 1,   // C_UseHourGlassAction
        UseMortar     = 2,   // mill (C_UseMortarAction); mortar empty + pot >= 2 -> gunpowder finish
        UseMortar2    = 3,   //   (C_FinishRecipeFromMortarAction); what distinguishes 2 vs 3: UNVERIFIED
        Distill       = 4,   // holding pot: C_UseRetortAction (retort INPUT station)
        TakePot       = 5,   // C_TakeObjectAction(1), gate pot >= 2 (retort OUTPUT station)
        UsePot        = 6,   // idle: C_MovePotAction; holding bowl/mortar/ingredient: place; holding pot: pour phial
        TakeHerb1     = 7,   // C_TakeIngredientAction from herb slot (verb - 7)
        TakeHerb2     = 8,
        TakeHerb3     = 9,
        TakeSpecial1  = 10,  // C_TakeIngredientAction from special slot (verb - 10)
        TakeSpecial2  = 11,
        TakeSpecial3  = 12,
        PourSpiritus  = 13,  // 13 + E_AlchemyPotionBaseType; C_UseResourceAction, gate pot bucket EMPTY
        PourOil       = 14,
        PourWine      = 15,
        PourWater     = 16,
        UseBellows    = 18,  // C_UseBellowsAction
        OpenBook      = 19,  // C_OpenBookAction
        MovePotBack   = 20,  // C_MovePotAction, second direction
        None          = 21,  // sentinel / bucket count
    };
};

}  // namespace wh::playermodule
