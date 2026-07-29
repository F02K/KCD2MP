#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipeProduct -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipeProduct@playermodule@wh@@ (TD 0x184DD56D8).  One <AlchemyProduct> row
// (quality tier -> product item + max yield).  RTTR-reflected by sub_18021BAE0; offsets from the
// property_wrapper immediates (CryGUID @+0x08, ints @+0x18/+0x1C via sub_1806DD3D0(...,24/28)).
// Stored INLINE in C_AlchemyRecipe::m_products (stride 0x20) [inferred by symmetry -- §1.2].
// Quality/MaxYield wrapper TYPE (int vs uint) not read from the deferred finalizer [UNVERIFIED
// signedness; XML domain 1..4 / 1..3].

namespace wh::playermodule {

class C_AlchemyRecipeProduct {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeProduct;
    virtual ~C_AlchemyRecipeProduct() = default;

    CryGUID m_productItemId;   // +0x08  potion item GUID granted at this quality tier
    int32_t m_quality;         // +0x18  quality tier 1..4 (banded from the brew-result coefficient)
    int32_t m_maxYield;        // +0x1C  max phials at this tier
};
static_assert(sizeof(C_AlchemyRecipeProduct) == 0x20, "C_AlchemyRecipeProduct must be 0x20");
static_assert(offsetof(C_AlchemyRecipeProduct, m_quality) == 0x18, "quality at 0x18");

}  // namespace wh::playermodule
