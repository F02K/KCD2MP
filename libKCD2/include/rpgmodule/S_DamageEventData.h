#pragma once
#include <cstdint>
#include "E_BloodZoneId.h"
#include "E_DamageEventType.h"
#include "E_DamageTypeMask.h"
#include "E_SoulSkill.h"
#include "I_Event.h"
#include "../combatmodule/E_CombatAttackType.h"
#include "../combatmodule/E_CombatZoneId.h"
#include "../entitymodule/StabSlashSmashValue.h"
#include "../framework/WUID.h"
#include "../Offsets/vtables/IEntity.h"
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::rpgmodule::S_DamageEventData -- per-hit damage-application context (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Struct name RE-CHOSEN (no RTTI; plain stack POD, no vtable). THE convergence record of the
// damage pipeline: every producer (C_CombatSoul slots 42/43/44/45 + the block-chip free fn
// sub_182CF906C) stack-allocates one, fills it, submits it to the central dispatcher
// S_DamageEventData::Dispatch (IDA-renamed from sub_18072704C, RVA 0x72704C; switch on
// m_type, treats the incoming pointer as its own this), then releases m_event via
// sub_1804FB980. Not wired as a callable C++ forwarder -- nothing in this codebase needs
// to construct+submit a fresh instance, only to read one from an existing hook.
// Ctor sub_180C65E64(this, ownerRef, &victimWuid); write extent +0x00..+0x154.
// sizeof 0x160 from the producer stack reservation (sub_180726DF4 local _BYTE[352]);
// the +0x158..+0x15F tail is unobserved (padding assumption, flagged).
//
// After Dispatch() returns, m_staminaDamage/m_healthDamage hold the FINAL post-mitigation
// values for ALL types (verified per handler), m_nonlethalClampApplied records whether the
// nonlethal clamp branch ran, and m_bloodZoneId is the resolved blood/body zone -- the single
// best hook site for damage observation. There is no general fatal-result flag in this record:
// m_forceFatal is a producer input, and m_nonlethalClampApplied is not its inverse.
//
// NOT a universal per-hit record: fall damage bypasses this context entirely (sub_1807243B0
// builds C_HealthValueEffect/C_ActStaminaValueEffect directly and stamps m_amount -- never
// calls sub_180C65E64 or Dispatch(); its own producer tag is 7, see C_HealthValueEffect.h
// m_damageSourceCode). The ctor call-site set is exactly 5 functions (verified via xrefs to
// sub_180C65E64): melee/missile/collision(x2 contexts)/block-chip/scripted -- no 6th producer.

namespace wh::rpgmodule {

class C_Soul;
class C_HealthValueEffect;
class C_ActStaminaValueEffect;

struct S_DamageEventData {
    _smart_ptr<I_Event> m_event;      // +0x000  copied in by sub_1809DB658 (AddRef), released by sub_1804FB980 (Release) -- see I_Event.h
    C_Soul*  m_pVictim;               // +0x008  resolved from WUID via C_SoulList::LookupByWUID sub_181F985D0 @(qword_1853322A0+128)+0x38
    C_Soul*  m_pAttacker;             // +0x010  resolved by sub_180726FF0 (same registry); may be null
    Offsets::IEntity* m_pVictimEntity;// +0x018  IEntitySystem::GetEntity(victimEntityId) iff entityId!=0, else null (sub_180C660FB..113)
    wh::framework::WUID m_attackerWeaponWuid;          // +0x020  attack weapon (sub_180727C1C arg 2) (rename unverified)
    wh::framework::WUID m_attackerSecondaryWeaponWuid; // +0x028  second item slot (sub_180727C1C arg 4; role UNVERIFIED) (rename unverified)
    wh::framework::WUID m_victimWeaponWuid;            // +0x030  item that made contact (melee desc+136 / blocking item); ewd resolve sub_180728170 + durability sub_1807286E8 (rename unverified)
    wh::framework::WUID m_missileWuid;                 // +0x038  projectile/missile item (sub_180727C1C arg 3); valid => m_type = MissileHit; fed into sub_180726A20 (damage-type-data resolve) by the missile handler @ctx+56 (CONFIRMED, cross-checked against sub_182CF0F64)
    Vec3     m_impactVelocity;        // +0x040  melee: mount-resolved participant-relative velocity (desc+0x34); missile: projectile velocity at contact
    Vec3     m_contactNormal;         // +0x04C  missile contact normal (EventPhysCollision::n); melee zeroes this one
    Vec3     m_hitPosition;           // +0x058  world-space contact point -- melee (desc+0x40) and missile (EventPhysCollision::pt) alike
    int32_t  m_forcedBodyPartId;      // +0x064  ctor -1; direct BodyPart database id (sub_180C625A0 reads when m_forcedBodySubpartId == -1) (reinterpretation unverified)
    int32_t  m_forcedBodySubpartId;   // +0x068  ctor -1; serialized as "forcedBodySubpartId" on the melee descriptor; bypasses only material->subpart resolution
    float    m_victimRotation;        // +0x06C  melee desc+112; serialized "victimRotation" (type/name unverified)
    Vec3     m_attackerVelocity;      // +0x070  collision handler sub_1816C79F8 swaps participants per-context but always places the CURRENT attacker's own velocity here (CONFIRMED)
    Vec3     m_victimVelocity;        // +0x07C  same swap; always the current victim's own velocity (CONFIRMED); sub_18151AFC0 computes norm(delta)
    float    m_requestedHealth;       // +0x088  ScriptedDamage input; copied to m_healthDamage by sub_180C62560 @0x180C6256F
    float    m_requestedStamina;      // +0x08C  ScriptedDamage input; copied to m_staminaDamage
    bool     m_blocking;              // +0x090  with +0x91: both false => attacker-ewd calc skipped (sub_180728170 @0x180728193) (rename unverified)
    bool     m_weakBlock;             // +0x091  gates sub_180C665E8's WeakBlockStamCoef lookup (rpg_param float[193] = +0x304) (CONFIRMED)
    bool     m_perfectBlock;          // +0x092  sub_180C665E8 zeroes the stamina-cost coefficient when set (CONFIRMED); live debug-tested. NOTE: supersedes an earlier "m_isPractice / stat counters 20/21" claim from this same session -- that was wrong, root cause of the mixup not re-traced.
    bool     m_forceBrokenBlock;      // +0x093  sub_180C65B78 ORs this in unconditionally, bypassing the strength/threshold tests, forcing the broken-block branch -> final +0x154 (CONFIRMED)
    bool     m_firstHit;              // +0x094  ctor qword 0x1000000 covers +0x94..+0x9B (rename unverified)
    bool     m_combo;                 // +0x095  selects the +4 rpg_param column in the melee tail (@0x180C64622) (rename unverified)
    bool     m_riposte;               // +0x096  (rename unverified)
    bool     m_intentionalHit;        // +0x097  ctor default true (rename unverified)
    bool     m_freeAttack;            // +0x098  (rename unverified)
    bool     m_flag99;                // +0x099
    bool     m_damageVictimWeapon;    // +0x09A  gates post-step sub_1816930A8 (@0x180C646B8) (rename unverified)
    bool     m_scaleByImpactVelocity; // +0x09B  (rename unverified)
    bool     m_applyAttackerPostHitEffects; // +0x09C  gates post-step sub_182CEE6A4 (@0x180C646C9) (rename unverified)
    bool     m_flag9D;                // +0x09D
    bool     m_forceFatal;            // +0x09E  producer input copied from S_CombatHitData::m_forceFatal; forces m_healthDamage = rpg_param dword[231] (@0x180C64308), not an output result
    bool     m_forceNonlethalClamp;   // +0x09F  forces m_nonlethalClampApplied=true, skipping the margin check (sub_180C62F1C @0x1822A157D)
    bool     m_fail;                  // +0x0A0  melee handler early-out (@0x180727CFD) (rename unverified)
    bool     m_stealthAction;         // +0x0A1  (word store desc+100 covers +0xA0/+0xA1) (rename unverified)
    uint8_t  _padA2[2];               // +0x0A2
    wh::combatmodule::E_CombatAttackType m_attackType; // +0x0A4  ctor -1(None); melee desc+32; keys the qword_1853209B8 attack-type-data lookup whose vfn[3] result lands at m_cannotKill (@0x180727D94/0x180727DA6)
    wh::combatmodule::E_CombatZoneId m_attackZone; // +0x0A8  ctor -1(Undefined); melee desc+40; directional guard/attack zone (star combat system)
    float    m_attackStrength;        // +0x0AC  ctor 1.0; melee desc+44
    float    m_attackEnergy;          // +0x0B0  ctor 1.0; melee desc+48
    float    m_forcedAttackValueCoef; // +0x0B4  ctor 1.0; 2nd scale on the weapon triple (@0x180C63FEF)
    float    m_damageScale;           // +0x0B8  ctor 1.0; 1st scale on the weapon triple (@0x180C63FD0); missile desc+132
    uint32_t m_itemBreakRngSeed;      // +0x0BC  melee desc+116; seeds sub_1808D6478's 214013*x+2531011 roll controlling item destruction (CONFIRMED)
    E_DamageEventType m_type;         // +0x0C0  dispatch discriminator (see E_DamageEventType.h)
    uint32_t _padC4;                  // +0x0C4
    C_ActStaminaValueEffect* m_pStaminaEffect;  // +0x0C8  created by factory sub_180C66280 (owner, victimWuid, 0.0)
    C_HealthValueEffect*     m_pHealthEffect;   // +0x0D0  created by factory sub_180C66194 (pool alloc 0x48)
    uint64_t m_rngSeedA;              // +0x0D8  ctor: global LCG qword_18492D7E0 step
    uint64_t m_rngSeedB;              // +0x0E0  ctor: LCG step + 1
    uint64_t m_rngState;              // +0x0E8  ctor: LCG step + 2; re-stepped per roll (214013*x+2531011)
    E_DamageTypeMask m_damageTypes;   // +0x0F0  ctor 1(Stab); melee desc+36 / missile from item (@0x182CF0F9C); bit-selects m_weaponDamage's 3 components in sub_180C64938 (builder sub_182CF6204 site UNCONFIRMED)
    wh::entitymodule::StabSlashSmashValue m_weaponDamage; // +0x0F4  pre-mitigation weapon damage triple (melee sub_180727DD0 / missile sub_182CEFE38)
    wh::entitymodule::StabSlashSmashValue m_blockIncomingDamage; // +0x100  BlockDamage handler input, incoming half (sub_182CF906C @+0x100..+0x10B)
    wh::entitymodule::StabSlashSmashValue m_blockDefense;        // +0x10C  BlockDamage handler input, defense half (sub_182CF906C @+0x10C..+0x117)
    float    m_impactMagnitude;       // +0x118  VelocityImpact input (collision @0x18151B18D; consumed @0x18151B352)
    // Body-zone resolution (traced @0x180C64018..0x180C64096, all VERIFIED): m_bodyMaterialId is
    // the melee descriptor's serialized origMatId0 plus one.  sub_182CFCC54 maps
    // (victim race, body_material_id) through body_material2subpart, writes a body_subpart_id
    // out-param, and separately writes the armor-selection result to m_armorSurfaceTypeId.
    // m_forcedBodySubpartId bypasses only that first mapping and enters at the body_subpart_id
    // stage.  sub_180C63D80 then resolves
    //   BodySubpart[body_subpart_id].body_part_id
    //     -> BodyPart[body_part_id].blood_zone_id
    // and stores the final E_BloodZoneId in m_bloodZoneId.  qword_1853243C0 and
    // qword_1853242A0 are the runtime C_BodySubpartDatabase and C_BodyPartDatabase respectively;
    // the final value set is data-defined in body_part.xml (see E_BloodZoneId.h).
    int32_t  m_bodyMaterialId;        // +0x11C  body_material2subpart.body_material_id; melee origMatId0 + 1 (@0x180727B10), 0 = unavailable
    int32_t  m_targetSurfaceId;       // +0x120  ctor -1; melee desc+80; missile passes it to the arrow-stick path sub_182CEF6C0
    E_BloodZoneId m_bloodZoneId;      // +0x124  ctor Undefined; FINAL body_part.blood_zone_id; consumed by C_BloodEffect via sub_180C629AC
    float    m_effectiveWeaponDefense; // +0x128  ctor -1.0; victim weapon's defense after E_PerkStat::wde(63) on victim and E_PerkStat::ewd(64) on attacker (sub_180728170)
    E_SoulSkill m_weaponSkill;        // +0x12C  ctor Count(35, =none); resolved by sub_180727FC8 (24=weapon_unarmed sentinel default, clamps out-of-range to Count); 19(marksmanship)/35(Count) special-cased in the reaction tail (@0x180C645BB)
    int32_t  m_armorSurfaceTypeId;    // +0x130  ctor -1; armor-selection out-param written by sub_182CFCC54/sub_182CFCC78; separate domain from body_subpart_id and E_BloodZoneId
    bool     m_cannotKill;             // +0x134  attack-type-data vfn[3] result, keyed by m_attackType (@0x180727DA6)
    bool     m_meleeArmorPath;        // +0x135  set 1 by melee fill (@0x180727BF9); selects sub_180BE16DC vs sub_182D25C74 armor path
    bool     m_nonlethalClampApplied; // +0x136  output: m_cannotKill branch clamped final health damage to leave >=1 health; not a general nonfatal/fatal indicator (sub_180C62F1C @0x180C62FC4)
    uint8_t  _pad137;                 // +0x137
    float    m_staminaDamage;         // +0x138  FINAL stamina damage (negated into m_pStaminaEffect delta @0x180C626FB)
    float    m_healthDamage;          // +0x13C  FINAL health damage (negated into m_pHealthEffect delta @0x180C627CD)
    float    m_blockFactor;           // +0x140  < 1.0 => blocked counters 18/19 (@0x180C6455D); != 0 => stamina-effect tag 5 (@0x180C6433A)
    float    m_maxDefenseValue;       // +0x144  4th float of the result block (sub_180C63DB0); per-zone aggregator max-reduces the selected defense value here
    wh::entitymodule::StabSlashSmashValue m_damageAfterDefense; // +0x148  attack value minus defense value per component (from-behind modifier applied), bit-selected by m_damageTypes and max-reduced in sub_180C64938/sub_180C65B38; central tail multiplies m_maxDefenseValue by Armor/WeaponDefenseToAttackingWeaponStatus before damaging attacker's weapon; this triple then selected by E_DamageTypeMask for victim weapon/armor condition loss
    bool     m_brokenBlock;           // +0x154  OR-aggregated broken-block result (see m_forceBrokenBlock +0x93); => stamina-effect tag 6 (@0x180C6431E)
    uint8_t  _pad155[11];             // +0x155  tail to the 0x160 stack reservation (unobserved, padding assumption)
};
static_assert(sizeof(S_DamageEventData) == 0x160, "S_DamageEventData must be 0x160 (producer stack reservation)");
static_assert(offsetof(S_DamageEventData, m_pVictim) == 0x08, "victim at 0x08");
static_assert(offsetof(S_DamageEventData, m_pAttacker) == 0x10, "attacker at 0x10");
static_assert(offsetof(S_DamageEventData, m_impactVelocity) == 0x40, "impact velocity at 0x40");
static_assert(offsetof(S_DamageEventData, m_contactNormal) == 0x4C, "contact normal at 0x4C");
static_assert(offsetof(S_DamageEventData, m_hitPosition) == 0x58, "hit position at 0x58");
static_assert(offsetof(S_DamageEventData, m_type) == 0xC0, "type at 0xC0");
static_assert(offsetof(S_DamageEventData, m_pHealthEffect) == 0xD0, "health effect at 0xD0");
static_assert(offsetof(S_DamageEventData, m_bodyMaterialId) == 0x11C, "body material id at 0x11C");
static_assert(offsetof(S_DamageEventData, m_bloodZoneId) == 0x124, "blood zone id at 0x124");
static_assert(offsetof(S_DamageEventData, m_effectiveWeaponDefense) == 0x128, "effective weapon defense at 0x128");
static_assert(offsetof(S_DamageEventData, m_weaponSkill) == 0x12C, "weapon skill at 0x12C");
static_assert(offsetof(S_DamageEventData, m_staminaDamage) == 0x138, "stamina damage at 0x138");
static_assert(offsetof(S_DamageEventData, m_healthDamage) == 0x13C, "health damage at 0x13C");
static_assert(offsetof(S_DamageEventData, m_attackType) == 0xA4, "attack type at 0xA4");
static_assert(offsetof(S_DamageEventData, m_damageTypes) == 0xF0, "damage type mask at 0xF0");
static_assert(offsetof(S_DamageEventData, m_weaponDamage) == 0xF4, "weapon damage triple at 0xF4");
static_assert(offsetof(S_DamageEventData, m_cannotKill) == 0x134, "cannot-kill flag at 0x134");
static_assert(offsetof(S_DamageEventData, m_nonlethalClampApplied) == 0x136, "nonlethal clamp flag at 0x136");
static_assert(offsetof(S_DamageEventData, m_damageAfterDefense) == 0x148, "third result triple at 0x148");
static_assert(offsetof(S_DamageEventData, m_brokenBlock) == 0x154, "tail flag at 0x154");
static_assert(offsetof(S_DamageEventData, m_pVictimEntity) == 0x18, "victim entity at 0x18");

}  // namespace wh::rpgmodule
