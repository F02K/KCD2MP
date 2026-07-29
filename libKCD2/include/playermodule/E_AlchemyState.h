#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyState -- the brewing session's interaction state (KCD2 WHGame.dll
// 1.5.6, kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// [SYNTHETIC NAME] -- stored in S_AlchemyBrewState::m_primaryState / m_subState (C_Alchemy abs
// +0x2A0/+0x2A4); read via C_Alchemy::GetEffectiveState (sub_1808D237C: primary, or the sub-state
// while primary == Idle).  The value domain is the shared verb/station code space (0..21, see
// C_Alchemy.h and E_AlchemyStationKind): a non-idle state means "verb N's object is in hand /
// station N interaction active", proven by the (state, verb) action factory sub_1808D1FF8 --
// state == verb builds C_ReturnIngredientAction (put the held thing back), and states 10-12
// validate the special slot [state-10] that idle verbs 10-12 take from.  Enumerator names derive
// from what the factory creates in each state (table_enable_and_verbs.md §3); codes never
// observed in the factory switch are omitted.

namespace wh::playermodule {

struct E_AlchemyState {
    enum Type : int32_t {
        HoldingBowl     = 0,   // verb 2/6 -> C_PlaceBowlAction
        HoldingMortar   = 2,   // verb 0/6 -> C_PlaceMortarAction (what distinguishes 2 vs 3: UNVERIFIED)
        HoldingMortar2  = 3,
        HoldingPot      = 5,   // verb 4 -> C_UseRetortAction (distill), verb 6 -> C_UsePotAction (pour phial)
        HoldingHerb1    = 7,   // verb 0/2/6 -> C_PlaceIngredientAction; herb slot = state-7
        HoldingHerb2    = 8,
        HoldingHerb3    = 9,
        HoldingSpecial1 = 10,  // verb 0/2/6 -> C_PlaceIngredientAction; special slot = state-10
        HoldingSpecial2 = 11,
        HoldingSpecial3 = 12,
        Idle            = 21,  // interactive idle (primary AND sub-state both 21): the idle verb map applies
    };
};

}  // namespace wh::playermodule
