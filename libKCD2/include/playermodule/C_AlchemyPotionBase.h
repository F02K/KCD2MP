#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "E_AlchemyPotionBaseType.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyPotionBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AVC_AlchemyPotionBase@playermodule@wh@@ (TD 0x184DE1AB8).  One row of
// Libs/Tables/minigame/AlchemyPotionBase.xml (4 rows: Spiritus/Oil/Wine/Water, Quality 0.1).
// RTTR-reflected by sub_18021A000 (CryGUID @+0x08, enum @+0x18, float @+0x1C wrapper immediates).
// Database: C_ObjectDatabase<C_AlchemyPotionBase> ("wh::playermodule::AlchemyPotionBaseDatabase"
// @0x183AF2710, class reg sub_18021A8E0).  The BaseItemId GUIDs are the ones the base-resource
// ctor sub_1806C2B08 selects by pour-verb (13->Spiritus row .. 16->Water row).

namespace wh::playermodule {

class C_AlchemyPotionBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyPotionBase;
    virtual ~C_AlchemyPotionBase() = default;

    CryGUID                       m_baseItemId;   // +0x08  the liquid's item GUID
    E_AlchemyPotionBaseType::Type m_baseType;     // +0x18  ONE BYTE (PotionBase() functor reads u8)
    uint8_t                       _pad19[3];      // +0x19
    float                         m_quality;      // +0x1C  base-quality contribution (0.1 in data)
};
static_assert(sizeof(C_AlchemyPotionBase) == 0x20, "C_AlchemyPotionBase must be 0x20");

}  // namespace wh::playermodule
