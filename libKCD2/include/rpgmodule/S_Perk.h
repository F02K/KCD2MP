#pragma once
#include <cstdint>
#include <cstddef>
#include "S_PerkDefinitionId.h"
#include "E_SoulStat.h"
#include "E_SoulSkill.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_Perk -- parsed perk-definition row (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// Row type of C_PerkDatabase. Built from the raw 120-byte rpg/perk table row by the converter
// sub_1809409B8 (called per row by ParseRows sub_18115C994; parsed vector stride 120 and
// GetRowCount's /120 magic divisor confirm the size). The raw row shares this layout with the
// five string columns as raw char* (converted through sub_180942380).
// Field-name evidence: the +0x48/+0x4C/+0x54 clamps reproduce KCD1's S_PerkDefinition
// statSelector/skillSelector/visibility semantics exactly (10 = no stat, 35 = no skill
// (E_SoulSkill count -- the same sentinel C_SkillXPEffect inits), visibility 0..3). Remaining
// names are KCD1-analog candidates, flagged.

namespace wh::rpgmodule {

struct S_Perk {
    S_PerkDefinitionId m_id;      // +0x00  perk id (raw copy; C_Perk::m_id key)
    S_PerkDefinitionId m_id10;    // +0x10  second 16B id [metaperk/parent candidate, UNVERIFIED]
    CryStringT<char> m_str20;     // +0x20  } five string columns (localized/processed via
    CryStringT<char> m_str28;     // +0x28  } sub_180942380); column roles NOT mapped --
    CryStringT<char> m_str30;     // +0x30  } ui name / description / icon / script hook
    CryStringT<char> m_str38;     // +0x38  } candidates [UNVERIFIED]
    CryStringT<char> m_str40;     // +0x40  }
    E_SoulStat  m_statSelector;   // +0x48  clamp >10 -> Count(10)=none (sub_1809409B8; data uses 0..3)
    E_SoulSkill m_skillSelector;  // +0x4C  clamp >=36 -> Count(35)=none (sub_1809409B8)
    uint32_t m_unk50;             // +0x50  raw copy [level-requirement candidate, UNVERIFIED]
    uint32_t m_visibility;        // +0x54  clamp >3 -> 0; 4-value visibility enum, names UNVERIFIED
    uint64_t m_id58[2];           // +0x58  16B id [buff/effect candidate, UNVERIFIED]
    uint8_t  m_flag68;            // +0x68  raw byte copy
    uint8_t  _pad69[3];           // +0x69
    uint32_t m_unk6C;             // +0x6C  raw copy
    int32_t  m_unk70;             // +0x70  converter sub_1809409B8 0x180940B65: if(v<0) v=1 (SIGNED int32 confirmed); count/mode with min 1 [role UNVERIFIED]
    uint32_t _pad74;              // +0x74
};
static_assert(sizeof(S_Perk) == 0x78, "S_Perk must be 0x78 (parsed vector stride 120)");
static_assert(offsetof(S_Perk, m_statSelector) == 0x48, "stat selector at 0x48");

}  // namespace wh::rpgmodule
