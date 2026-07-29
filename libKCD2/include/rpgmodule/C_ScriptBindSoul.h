#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::rpgmodule::C_ScriptBindSoul -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (CONFIRMED: alloc 120).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindSoul@rpgmodule@wh@@ (COL->CHD = [self, CScriptableBase], mdisp 0)
// vftable 0x183B55338  ctor sub_1812E27C4  owned by C_RPGModule @+0xF8.
//
// The ctor does NOT call SetGlobalName: the 51 functions below populate m_pMethodsTable used as
// the per-soul USERDATA method set/metatable (soul objects obtained e.g. via entity.soul /
// GetSoul), not a literal global "Soul" table.
//
// Lua fn table (51 fns; registrar sub_1816759EC; handler = the delegate target, NOT the generic
// invoker trampoline; args = the registered CryEngine arg-name string):
//   GetId                     0x182CF4D54  ()
//   GetState                  0x1817FE708  (state)
//   SetState                  0x182CFD18C  (state, value)
//   GetStatLevel              0x182CF7518  (stat)                 -> impl sub_182CD6820, rejects stat >= 10
//   GetStatProgress           0x182CF7520  (stat)
//   AdvanceToStatLevel        0x182CEE570  (stat, level)          CHEAT
//   AddStatXP                 0x182CEE4A8  (stat, xp)             CHEAT -> impl sub_182CD3A00
//   GetNextLevelStatXP        0x182CF6178  (stat, level)
//   GetDerivedStat            0x180BE3D54  (derivedStat)          -> impl sub_180BE3834, rejects id >= 218
//   HaveSkill                 0x180BE25B8  (skill)
//   GetSkillLevel             0x182CF71E8  (skill)                -> impl sub_182CD6590, rejects skill >= 35
//   GetSkillProgress          0x182CF71F0  (skill)
//   HasAbility                0x182CF7D88  (ability)
//   AdvanceToSkillLevel       0x182CEE4B8  (skill, level)         CHEAT
//   AddSkillXP                0x182CEE3CC  (skill, xp)            CHEAT
//   GetNextLevelSkillXP       0x182CF60EC  (skill, level)
//   AddBuff                   0x182CEDF10  (buff_id)              CHEAT
//   RemoveBuff                0x182CFC93C  (buff_instance)
//   RemoveAllBuffsByGuid      0x182CFC88C  (buff_guid)
//   AddPerk                   0x182CEE1E4  (perk_id)              CHEAT
//   RemovePerk                0x182CFCA98  (perk_id)
//   AddAllCodexPerks          0x182CEDE94  ()                     CHEAT
//   AddMetaRoleByName         0x182CEE140  (name)
//   RemoveMetaRoleByName      0x182CFC9F4  (name)
//   DealDamage                0x180EE018C  (stamina, health)
//   HealBleeding              0x18168530C  (heal, bodyPartId)
//   AddInjury                 0x182CEE014  (injury, bodyPartName)
//   ModifyPlayerReputation    0x182CF8C68  (repChangeName, propagateToFaction)
//   GetRelationship           0x182CF6C98  (otherSoulWuid)
//   CalculateBarterDominance  0x1817AC034  (shopkeeperSoulWuid)
//   IsInCombatDanger          0x18117F144  ()
//   IsInCombatMode            0x18117F1E8  ()
//   IsInTenseCircumstance     0x18117F18C  ()
//   GetNameStringId           0x182CF60A8  ()
//   GetReadCaptionObjectText  0x182CF6AA4  ()
//   IsPublicEnemy             0x18117F24C  ()
//   IsLegalToLoot             0x18117F288  ()
//   IsDialogRestricted        0x18082AAD4  ()
//   RestrictDialog            0x1804D0CF0  (bRestrict)
//   GetMetaRoles              0x182CF59D4  ()
//   GetSchedule               0x182CF6F04  ()
//   GetSocialClass            0x182CF73B4  ()
//   GetArchetype              0x1819300F4  ()
//   GetGender                 0x1814B0BD4  ()
//   GetGatherMult             0x182CF4A90  ()
//   GetFactionID              0x182CF47CC  ()
//   HasPerk                   0x1804D15F8  (perkDefId, usePerk)
//   HasScriptContext          0x180BE239C  (contextName)
//   OnCompanionEvent          0x182CF9490  (companion, eventType)
//   CanEatItem                0x182CF115C  (itemClassIdDef)
//   EatItem                   0x182CF24B4  (itemClassIdDef)
// stat/skill string args resolve via E_SoulStat / E_SoulSkill. GetDerivedStat is anomalous:
// its string converter uses E_PerkStat's 103-code registry, but sub_180BE3834 passes the
// resulting ordinal to the native E_DerivedStat evaluator (218-value domain). See both enums.

namespace wh::rpgmodule {

class C_ScriptBindSoul : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindSoul;
    Offsets::IScriptSystem* m_pSS2;        // +0x60  raw ctor pSS (*a2; base +0x50 holds the vf[+0x278] accessor result)
    void*                   m_pGlobalSys;  // +0x68  = qword_18492D8A0 snapshot: a global vtable-bearing engine singleton (accessor 0x180479B4C calls its vf[14] then vf[74]) [exact class UNVERIFIED]
    C_SoulList*             m_pOwner;      // +0x70  owning soul list (ctor a4 = module->m_pSoulList @C_RPGModule+0x80)
};
static_assert(sizeof(C_ScriptBindSoul) == 0x78, "C_ScriptBindSoul must be 0x78");
static_assert(offsetof(C_ScriptBindSoul, m_pOwner) == 0x70, "owner at 0x70");

}  // namespace wh::rpgmodule
