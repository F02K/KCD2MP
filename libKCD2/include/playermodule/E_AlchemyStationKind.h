#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyStationKind -- KCD2 WHGame.dll 1.5.6 (utem).  Enum wrapper,
// Type : int32.
// -----------------------------------------------
// [SYNTHETIC NAME] -- station/bucket indices of the brew state (== the creating verb code;
// shared code space with E_AlchemyVerb / E_AlchemyState).  Values verified from the mutation
// writers: 2 mortar (mill sub_1806C4384) / 4 retort-distill INPUT (sub_182E2B148, then moved
// 4->5 by sub_1806C4D5C) / 5 retort OUTPUT / 6 pot-base (pour sub_1806C29D8; boil accrual
// sub_18073920C) / 7-9 herb slots (add sub_1808D42D8) / 10-12 special slots (sub_182E19644).
// Count = the 21-bucket array bound (S_AlchemyBrewState::m_buckets).

namespace wh::playermodule {

struct E_AlchemyStationKind {
    enum Type : int32_t {
        Mortar        = 2,
        RetortInput   = 4,
        RetortOutput  = 5,
        Pot           = 6,
        HerbSlot1     = 7,
        HerbSlot2     = 8,
        HerbSlot3     = 9,
        SpecialSlot1  = 10,
        SpecialSlot2  = 11,
        SpecialSlot3  = 12,
        Count         = 21,
    };
};
static_assert(sizeof(E_AlchemyStationKind::Type) == 4, "bucket index domain is int32");

}  // namespace wh::playermodule
