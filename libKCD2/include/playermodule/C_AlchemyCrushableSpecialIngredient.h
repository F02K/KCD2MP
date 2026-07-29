#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyCrushableSpecialIngredient -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI .?AVC_AlchemyCrushableSpecialIngredient@playermodule@wh@@ (TD 0x184DE0C90).  One row of
// Libs/Tables/minigame/AlchemyCrushableSpecialIngredient.xml (5 rows: frankincense/coal/salt/
// charcoal/pearl) -- special ingredients that can be milled in the mortar, with the mortar-content
// models to swap in.  RTTR-reflected by sub_1802180B0 (CryGUID @+0x08, CryStringT @+0x18/+0x20).
// The 2nd inline CryGUID set_value wrapper in the registration is a getter-only alias (no stored
// 2nd GUID).  Database: "wh::playermodule::AlchemyCrushableSpecialIngredientDatabase" @0x183AF2480
// (class reg sub_1802187B0).  [Size 0x28 is the field-model size; row stride not independently
// observed -- UNVERIFIED tail.]

namespace wh::playermodule {

class C_AlchemyCrushableSpecialIngredient {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyCrushableSpecialIngredient;
    virtual ~C_AlchemyCrushableSpecialIngredient() = default;

    CryGUID          m_itemId;        // +0x08  special-ingredient item GUID
    CryStringT<char> m_mortarModel;   // +0x18  .cgf shown in the mortar before crushing
    CryStringT<char> m_crushedModel;  // +0x20  .cgf shown after crushing
};
static_assert(sizeof(C_AlchemyCrushableSpecialIngredient) == 0x28,
              "C_AlchemyCrushableSpecialIngredient must be 0x28");

}  // namespace wh::playermodule
