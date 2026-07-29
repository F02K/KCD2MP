#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "E_AlchemyFeedbacks.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyFeedback -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI .?AVC_AlchemyFeedback@playermodule@wh@@ (TD 0x184DE17D0).  One row of
// Libs/Tables/minigame/AlchemyFeedback.xml.  RTTR-reflected by sub_180218D50.  Size stride-
// VERIFIED: the (type,itemId)->row lookup sub_182E2BCBC scans 40-byte rows at the feedback-DB
// rows global xmmword_185332A20.
//
// MetaroleId is a CryStringT NAME (e.g. "ALCHEMY_TOO_MANY_FAILED_STEPS"), NOT a GUID -- proven by
// the emit path sub_182E2C61C (copies the CryStringT at row+0x10 and triggers the metarole bark
// via sub_18051A948 unless it equals the "none" sentinel).  ItemId is an optional per-item
// override key (empty GUID on the generic rows); the lookup matches Type@+0x0C AND ItemId@+0x18.
// A 5th reflected (int, set_value) property exists in the registration but has no storage in the
// 0x28 row -- computed/getter-only [name UNVERIFIED].

namespace wh::playermodule {

class C_AlchemyFeedback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyFeedback;
    virtual ~C_AlchemyFeedback() = default;

    int32_t                   m_id;          // +0x08  <AlchemyFeedback Id>
    E_AlchemyFeedbacks::Type  m_type;        // +0x0C  matched against the evaluator's feedback byte
    CryStringT<char>          m_metaroleId;  // +0x10  bark metarole name
    CryGUID                   m_itemId;      // +0x18  optional per-item override (0 = generic row)
};
static_assert(sizeof(C_AlchemyFeedback) == 0x28, "C_AlchemyFeedback must be 0x28");
static_assert(offsetof(C_AlchemyFeedback, m_metaroleId) == 0x10, "metarole at 0x10");

}  // namespace wh::playermodule
