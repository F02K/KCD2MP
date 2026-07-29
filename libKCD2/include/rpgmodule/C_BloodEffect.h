#pragma once
#include <cstddef>
#include <cstdint>
#include "C_SoulEffect.h"
#include "E_BloodZoneId.h"

// -----------------------------------------------
// wh::rpgmodule::C_BloodEffect -- soul blood/dirt visual state change (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D601F0  vtable 0x183A8B830.  sizeof 0x20 (PROVEN: factory sub_180C62C7C
// operator new(32)).  Serializer sub_180E7C0C names the payloads "bloodZone" and "damage";
// apply vfn sub_180AD7BCC bounds-checks bloodZone against 0..5 and passes damage as a float.

namespace wh::rpgmodule {

class C_BloodEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BloodEffect;
    E_BloodZoneId m_bloodZoneId;  // +0x18  ctor arg 4; serialized as "bloodZone"
    float         m_damage;       // +0x1C  ctor arg 5; serialized as "damage"
};
static_assert(sizeof(C_BloodEffect) == 0x20, "C_BloodEffect must be 0x20 (operator new(32))");
static_assert(offsetof(C_BloodEffect, m_bloodZoneId) == 0x18, "blood zone id at 0x18");
static_assert(offsetof(C_BloodEffect, m_damage) == 0x1C, "damage at 0x1C");

}  // namespace wh::rpgmodule
