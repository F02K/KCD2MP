#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAttackType -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum.
// -----------------------------------------------
// combat_attack_type id. Registered by C_CombatAttackTypeDatabase::Init (sub_1814FBFF0,
// RVA 0x14FBFF0): each id-holder global is set to -1, then the type NAME is inserted into a
// name-keyed std::map (qword_185320968) whose per-name onLoad callback resolves the numeric id
// FROM the loaded C_CombatAttackTypeData row at table-load. No ids are hard-coded in the DLL.
//
// [VALUES VERIFIED against KCD2 game data] Ids below are the row ids of
// Libs/Tables/combat/combat_attack_type.xml extracted from the KCD2 Tables.pak
// (analysis/game_data/combat_tables/, extract_kcd2_combat_tables.py). The KCD2 table keeps ALL
// TEN KCD1 rows with unchanged ids -- the earlier "10 -> 5 shrink" reading was about the BINARY
// resolver set only: sub_1814FBFF0 registers id-resolvers for just 5 names (stab, punch, kick,
// slash, backoff; id globals dword_1853209D4..dword_1853209E0). The other 5 rows remain live
// table data reachable through FK columns (e.g. combat_action_attack.attack_type_id).

namespace wh::combatmodule {

enum class E_CombatAttackType : int32_t {
    None    = -1,   // "none" -- unregistered -1 sentinel (a directional combo step carries this)
    Stab    = 0,    // "stab"    binary-resolved (dword_1853209D4); rpg_damage_type 5
    Slash   = 1,    // "slash"   binary-resolved (dword_1853209DC); rpg_damage_type 6
    Smash   = 2,    // "smash"   [table-only in KCD2; no binary resolver]; rpg_damage_type 4
    Throw   = 3,    // "throw"   [table-only]; cannot_kill
    Kick    = 4,    // "kick"    binary-resolved (dword_1853209D8); cannot_kill
    Punch   = 5,    // "punch"   binary-resolved; cannot_kill
    Hook    = 6,    // "hook"    [table-only]; cannot_kill
    Direct  = 7,    // "direct"  [table-only]; cannot_kill
    Bite    = 8,    // "bite"    [table-only]
    Backoff = 9,    // "backoff" binary-resolved (dword_1853209E0); cannot_kill
};

}  // namespace wh::combatmodule
