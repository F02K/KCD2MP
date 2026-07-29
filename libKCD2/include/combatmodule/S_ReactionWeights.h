#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_ReactionWeights -- KCD2 WHGame.dll 1.5.6 (kd7u). sizeof 0x14.
// -----------------------------------------------
// Melee-defense reaction weight set: one float per mutually-exclusive defensive reaction. Used as
// the OWNED heap override C_CombatAutomationDefense::m_pWeightOverride (+0x38, heap 20B; a per-
// float value < 0 means "no override for this slot"). The defense evaluate path (sub_180C56048)
// accumulates these weights, rolls the LCG, and picks one action.
//
// [SURVIVED -- no KCD2 TD needed; it is a 20-byte POD, never had a TD]. Field NAMES are the KCD2
// debug-dump order (sub_1827683BC "DEF %s: #%d, NO/NB/PB/MS/Dodge/MoveRes"): NoReaction, Block,
// PerfectBlock, MasterStrike, Dodge. [vs KCD1] KCD1 named the same 5 slots noBlock / normalBlock
// / perfectBlock / specialPerfectBlock / dodge; KCD2 renumbering of the master-strike reaction is
// reflected in the "MasterStrike" name.

namespace wh::combatmodule {

struct S_ReactionWeights {
    float noReaction;     // +0x00  do nothing (take the hit)
    float block;          // +0x04  ordinary directional block
    float perfectBlock;   // +0x08  perfect block
    float masterStrike;   // +0x0C  master strike (riposte perfect block)
    float dodge;          // +0x10  dodge
};
static_assert(sizeof(S_ReactionWeights) == 0x14, "S_ReactionWeights must be 0x14");

}  // namespace wh::combatmodule
