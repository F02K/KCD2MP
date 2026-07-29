#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_DamageEventType -- damage-application dispatch discriminator (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Enum name RE-CHOSEN (no RTTI/string source found). Values recovered from the WRITERS of
// S_DamageEventData::m_type (+0xC0) and the dispatch switch in S_DamageEventData::Dispatch
// (IDA-renamed from sub_18072704C) @0x18072706B:
//   1 -> sub_180727C38 (melee)      set by the melee context fill sub_180727B0C @0x180727C00
//   2 -> sub_182CF0F64 (missile)    set by sub_180727C1C @0x18222B0D6 iff m_missileWuid valid
//   3 -> sub_18151AFC0 (collision)  set by C_CombatSoul::OnCollisionHit sub_1816C79F8 (both contexts)
//   4 -> sub_182CF0E88 (block)      set by the block-chip producer sub_182CF906C @0x182CF92B3
//   5 -> sub_180C62560 (scripted)   set by C_CombatSoul::OnScriptedHit sub_1807258DC
//   6 -> sub_18151B324 (velocity)   re-tagged by the collision handler @0x18151B183 before it
//                                   calls its velocity-impact core; no independent type-6 producer
//                                   (VERIFIED: exactly 5 functions call the ctor sub_180C65E64)
//
// Fall damage does NOT go through this enum/context at all: sub_1807243B0 builds
// C_HealthValueEffect/C_ActStaminaValueEffect directly (producer tag 7, see
// C_HealthValueEffect.h m_damageSourceCode) and never calls sub_180C65E64 or Dispatch().

namespace wh::rpgmodule {

enum class E_DamageEventType : int32_t {
    None           = 0,  // ctor default (sub_180C65E64 @0x180C65FE3)
    MeleeHit       = 1,
    MissileHit     = 2,
    Collision      = 3,  // mutual event: one context per participant
    BlockDamage    = 4,  // chip damage through a block (arm zone) + blocking-item durability loss
    ScriptedDamage = 5,  // Lua soul:DealDamage / actor:TakeDamage path
    VelocityImpact = 6,  // norm(deltaV)-driven damage; internal collision re-tag only, not independently produced
};

}  // namespace wh::rpgmodule
