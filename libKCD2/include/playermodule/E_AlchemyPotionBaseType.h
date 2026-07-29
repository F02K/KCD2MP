#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyPotionBaseType -- KCD2 WHGame.dll 1.5.6 (kd7u/utem).  Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper, registered by sub_18021E410; enumerators certified from the
// registration's {name,len,value} tuples.  STORAGE IS ONE BYTE (corrected from int32): the
// C_AlchemyRecipe base-material getter (vtbl+0x58 = 0x181A8BBC0) is `return *(u8*)(this+12)`,
// and the PotionBase() DSL functor (0x182E2B480) compares it against `*(u8*)(potionBaseRow+0x18)`
// -- both sites byte-wide (the RTTR enumeration_wrapper's "4" is the enumerator VALUE width,
// not the field width).  Used by C_AlchemyRecipe::m_baseMaterial (+0x0C) and
// C_AlchemyPotionBase::m_baseType (+0x18).
//
// NOTE: the pour-verb codes 13..16 map onto this enum in the SAME order (verb = 13 + Type):
// C_AlchemyResource ctor sub_1806C2B08 selects the base-liquid GUID by verb, and the GUIDs match
// AlchemyPotionBase.xml rows 13->Spiritus, 14->Oil, 15->Wine, 16->Water.

namespace wh::playermodule {

struct E_AlchemyPotionBaseType {
    enum Type : uint8_t {
        Spiritus = 0,
        Oil      = 1,
        Wine     = 2,
        Water    = 3,
    };
};

static_assert(sizeof(E_AlchemyPotionBaseType::Type) == 1,
              "BaseMaterial/BaseType are ONE-BYTE fields (getter 0x181A8BBC0 reads u8)");

}  // namespace wh::playermodule
