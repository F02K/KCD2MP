#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_SoulSkill -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum, 35 values.
// -----------------------------------------------
// Recovered from the C_RPGUtils enum converters: name->id vtable+0x90 = sub_180BE28F4 (via
// C_SkillDatabase qword_18532FDC0), id->name sub_180C69644 (also vtable+0xA0 sub_182CF726C /
// +0x98 sub_18055DDD8). VERIFIED both directions, all 35 values. The Lua Soul:GetSkillLevel
// handler (sub_182CD6590 via thunk sub_182CF71E8) rejects id >= 0x23.
//
// This enum indexes S_SoulStatBlock::m_skills[35] (base value = *(uint32*)(block + 0x88 + 8*id),
// i.e. C_Soul + 1512 + 8*id on the live block @C_Soul+0x560).
//
// The *_obsolete enumerators (5, 9, 18, 21, 25) are retained-but-deprecated KCD1 skills that
// still occupy live ids (pickpocketing/smithing/bow/mace/herbalism were reworked in KCD2).

namespace wh::rpgmodule {

enum class E_SoulSkill : uint32_t {
    stealth               = 0,
    horse_riding          = 1,
    fencing               = 2,
    bard                  = 3,
    thievery              = 4,
    pickpocketing_obsolete = 5,
    alchemy               = 6,
    cooking               = 7,
    craftsmanship         = 8,
    smithing_obsolete     = 9,
    fishing               = 10,
    mining                = 11,
    first_aid             = 12,
    drinking              = 13,
    survival              = 14,
    defense               = 15,
    weapon_sword          = 16,
    heavy_weapons         = 17,
    weapon_bow_obsolete   = 18,
    marksmanship          = 19,   // bow/ranged
    weapon_shield         = 20,
    weapon_mace_obsolete  = 21,
    weapon_dagger         = 22,
    weapon_large          = 23,
    weapon_unarmed        = 24,   // fists
    herbalism_obsolete    = 25,
    scholarship           = 26,
    tailoring             = 27,
    armourer              = 28,
    weaponsmithing        = 29,
    shoemaking            = 30,
    gunsmithing           = 31,
    bowyery               = 32,
    gambling              = 33,
    houndmaster           = 34,

    Count                 = 35,   // reject threshold in every skill handler (SYNTHETIC enumerator)
};

}  // namespace wh::rpgmodule
