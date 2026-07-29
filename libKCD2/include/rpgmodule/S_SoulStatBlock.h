#pragma once
#include <cstdint>
#include "C_PerkList.h"
#include "E_SoulStat.h"
#include "E_SoulSkill.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::S_SoulStatBlock -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x268 (SYNTHETIC name).
// -----------------------------------------------
// The per-soul stat/skill/level/perk sheet (built by sub_18070F3E8). No RTTI on the block itself
// (name inferred from KCD1's S_SoulStatBlock role). C_Soul's RPG aggregate holds TWO of these:
//   block#1 @C_Soul+0x560 = the AUTHORITATIVE/LIVE sheet -- every RTTR + Lua stat/skill getter and
//     requirement check reads it via the hardcoded C_Soul-relative offsets 1388 (=block+12, stats)
//     and 1512 (=block+136, skills).
//   block#2 @C_Soul+0x840 = a full identical second instance (base/snapshot or "silent" sheet;
//     serialized together, block#2 with notify-flag 0). Exact role UNRESOLVED (flagged).
//
// *** THIS IS THE PRIMARY CHEAT SURFACE ***  Base stat value id = read as *(uint32*)(block+12+8*id)
// (E_SoulStat, 10 entries); base skill value id = *(uint32*)(block+136+8*id) (E_SoulSkill, 35).
// The final stat/skill returned to gameplay adds the S_ModifierNode chain at C_Soul+0xA8 (stats) /
// C_Soul+0xB0 (skills) then clamps to config. DERIVED stats (Health/Stamina/...) are NOT stored --
// computed on demand by sub_180648B18 from base stats + config (GetDerivedStat, id < 218).

namespace wh::rpgmodule {

class C_Soul;

// One stat/skill cell (8B). dword0 = base VALUE (all readers use only this); dword1 zeroed by the
// element ctor, never observed being read -- possibly XP / cached-modified / reserved (UNVERIFIED).
struct S_StatCell {
    uint32_t value;   // +0x00  base value (the number gameplay reads)
    uint32_t _unk04;  // +0x04  reserved/unused: element ctor sub_180C23340 zeroes the whole 8B cell; every located reader (sub_180469D7C stats, sub_18046F854 skills) and writer (sub_180670788 addss/movss) touches only +0x00 (value); never observed non-zero
};
static_assert(sizeof(S_StatCell) == 0x8, "S_StatCell must be 0x8");

// Bespoke 0x28 "stat-change staging" record (reset by sub_18046F6CC on level change).
struct S_StatChangeStaging {
    float               value0;   // +0x00  (=0)
    uint32_t            _pad04;    // +0x04
    wh::framework::WUID wuid;      // +0x08  INVALID sentinel (qword_185332358 = 0)
    int64_t             a;         // +0x10  (=-1)
    int64_t             b;         // +0x18  (=-1)
    float               value1;   // +0x20  (=0)
    uint32_t            _pad24;    // +0x24
};
static_assert(sizeof(S_StatChangeStaging) == 0x28, "S_StatChangeStaging must be 0x28");

// Per-category {value, cap} pair of the +0x1D8 pool (defaults value{0,2,2,2}, cap FLT_MAX).
struct S_StatPoolEntry {
    float value;   // +0x00
    float cap;     // +0x04  init FLT_MAX
};
static_assert(sizeof(S_StatPoolEntry) == 0x8, "S_StatPoolEntry must be 0x8");

struct S_SoulStatBlock {
    C_Soul*  m_pOwner;                    // +0x000  owner backref
    bool     m_flag08;                    // +0x008  (=1; enabled/dirty)
    uint8_t  _pad09[3];                   // +0x009
    S_StatCell m_stats[10];               // +0x00C  base stat values (E_SoulStat; reader sub_180469D7C)
    uint32_t _pad5C;                      // +0x05C  (stats end at 0x5C; changeStaging is 8-aligned at 0x60)
    S_StatChangeStaging m_changeStaging;  // +0x060  (0x28)
    S_StatCell m_skills[35];              // +0x088  base skill values (E_SoulSkill; reader sub_18046F854)
    C_Soul*  m_pSkillsOwner;              // +0x1A0  skills sub-object owner backref
    float    m_levelBlock[6];             // +0x1A8  [0]=Level (level-up-notified), [1]/[5]=caps; per-index roles partly inferred
    float    m_reserved1C0[2];            // +0x1C0  reserved level-block slots 6-7: contiguous with m_levelBlock float array, addressed by setter sub_18046F6CC as a1[id+106]; progression reset sub_18046F5CC enumerates only indices 0-5, never these
    float    m_reserved1C8[4];            // +0x1C8  reserved level-block slots 8-11: ctor OWORD-zeroes (xmmword_185583B38=0); setter sub_18046F6CC never called with idx>5 by any located path; no reader found
    S_StatPoolEntry m_pool[4];            // +0x1D8  per-category mult/cap (semantic name inferred)
    C_PerkList m_perks;                   // +0x1F8  (0x70; fills block to 0x268)
};
static_assert(sizeof(S_SoulStatBlock) == 0x268, "S_SoulStatBlock must be 0x268");
static_assert(offsetof(S_SoulStatBlock, m_stats) == 0x0C, "stats at block+12");
static_assert(offsetof(S_SoulStatBlock, m_skills) == 0x88, "skills at block+136");
static_assert(offsetof(S_SoulStatBlock, m_perks) == 0x1F8, "perks at block+504");

}  // namespace wh::rpgmodule
