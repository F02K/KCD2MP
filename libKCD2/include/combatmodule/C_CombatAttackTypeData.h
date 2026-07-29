#pragma once
#include <cstdint>
#include "I_CombatAttackTypeData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAttackTypeData : I_CombatAttackTypeData -- KCD2 1.5.6 (kd7u). sizeof 0x60.
// -----------------------------------------------
// combat_attack_type lookup row -> Data (FK-resolver layer). POLYMORPHIC. vtable 0x183BA65C8.
// Data->Data copy-ctor sub_1814FBBA0 (RVA 0x14FBBA0; the Row->Data converter is sub_1814FBD10);
// the DB's vector-relocate loop sub_1814FBD90
// strides 0x60 (v3 += 12 qwords) -> sizeof VERIFIED 0x60. dtor sub_1814FBF30.
//
// [vs KCD1] KCD1 was 0x50; KCD2 grew to 0x60. Each mn-tag-backed string is now
// {CryStringT (8)} + {runtime-resolved tag block (0x10)} + {int32 resolved tag} = 0x20, so mn_tag
// occupies 0x20..0x3F and opp_mn_tag occupies 0x40..0x5B. The 0x10 runtime block is filled at
// tag-resolution time (mn-tag string -> animation tag CRC/Vec4); its internal layout is
// UNVERIFIED, modeled as an opaque byte block.

namespace wh::combatmodule {

class C_CombatAttackTypeData : public I_CombatAttackTypeData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAttackTypeData;
    ~C_CombatAttackTypeData() override;                    // [0] 0x14FBF30
    float   GetRelativeStaminaCost() const override;       // [1] 0x1A7E030
    int32_t GetRpgDamageTypeId() const override;           // [2] 0x1A72850
    bool    GetCannotKill() const override;                // [3] 0x1350EE0

    int32_t          combat_attack_type_id;      // +0x08
    int32_t          _pad0C;                      // +0x0C
    CryStringT<char> combat_attack_type_name;     // +0x10
    int32_t          rpg_damage_type_id;          // +0x18
    float            relative_stamina_cost;       // +0x1C  (default 1.0)
    CryStringT<char> mn_tag;                       // +0x20
    uint8_t          _mnTagRuntime28[0x10];       // +0x28  runtime-resolved tag block (semantics UNVERIFIED)
    int32_t          _mnTagResolvedId38;          // +0x38  resolved animation-tag id
    int32_t          _pad3C;                      // +0x3C
    CryStringT<char> opp_mn_tag;                   // +0x40
    uint8_t          _oppMnTagRuntime48[0x10];    // +0x48  runtime-resolved tag block (semantics UNVERIFIED)
    int32_t          _oppMnTagResolvedId58;       // +0x58  resolved animation-tag id
    bool             cannot_kill;                 // +0x5C
    uint8_t          _pad5D[3];                   // +0x5D
};
static_assert(sizeof(C_CombatAttackTypeData) == 0x60, "C_CombatAttackTypeData must be 0x60");

}  // namespace wh::combatmodule
