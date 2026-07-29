#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatInputClass -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum.
// -----------------------------------------------
// combat_input_class id. Registered by C_CombatInputClassDatabase::Init (sub_18155CF10,
// RVA 0x155CF10): sets each id-holder global to -1, then inserts the class NAME into a
// name-keyed std::map<CryStringT, std::function<void(C_CombatInputClassData const&)>>
// (qword_185320FB8) whose per-name onLoad callback resolves the numeric id FROM the loaded
// data row at table-load time. So the numeric id is the combat_input_class.tbl row id, NOT a
// value hard-coded in WHGame.dll.
//
// [VERIFIED from binary] The registered NAME SET and REGISTRATION ORDER are identical to KCD1:
//   attack_light, attack_heavy, attack_special, move_left, move_right, move_back,
//   move_forward, block  (id-holder globals dword_185321010..dword_18532102C, ascending).
// [VALUES VERIFIED against KCD2 game data] Libs/Tables/combat/combat_input_class.xml extracted
// from the KCD2 Tables.pak (analysis/game_data/combat_tables/, extract_kcd2_combat_tables.py)
// assigns exactly this order 0..7 sequentially ("none" = -1) -- identical to KCD1.
//
// Stored in C_CombatInputClassData.combat_input_class_id (+0x08 in KCD2) and matched by the
// attack-query builder. For guaranteed-correct runtime comparison read the live id globals.

namespace wh::combatmodule {

enum class E_CombatInputClass : int32_t {
    None          = -1,   // "none"           (unregistered -1 sentinel)
    AttackLight   = 0,    // "attack_light"   dword_185321010
    AttackHeavy   = 1,    // "attack_heavy"   dword_185321014
    AttackSpecial = 2,    // "attack_special" dword_185321018
    MoveLeft      = 3,    // "move_left"      dword_18532101C
    MoveRight     = 4,    // "move_right"     dword_185321020
    MoveBack      = 5,    // "move_back"      dword_185321024
    MoveForward   = 6,    // "move_forward"   dword_185321028
    Block         = 7,    // "block"          dword_18532102C
};

}  // namespace wh::combatmodule
