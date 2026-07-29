#pragma once
#include <cstdint>
#include <boost/container/flat_map.hpp>
#include "E_DerivedStat.h"
#include "E_PerkStat.h"
#include "I_Soul.h"
#include "S_ModifierNode.h"
#include "C_SoulPropertyNotifier.h"
#include "C_CombatSoul.h"
#include "C_InventorySoul.h"
#include "C_SoulRPGStats.h"
#include "S_SoulRegistry.h"
#include "C_SoulBuffList.h"
#include "../entitymodule/I_ItemCollectionListener.h"
#include "../framework/CryDeferrable.h"
#include "../framework/WUID.h"
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_Soul -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD20 (pool stride 3360, PROVEN).
// -----------------------------------------------
// RTTI .?AVC_Soul@rpgmodule@wh@@ (TD 0x184D72390)  primary vtable 0x183F44F28 (~60 slots, NOT
// enumerated)  ctor 0x1803F1C7C  base dtor 0x180966104.  Allocated from the C_SoulList slot pool
// (sub_181FE2CC0: addr = 3360*slotIdx + poolBase; pool block = 3360*64).
// KCD1 C_Soul was 0xC38; KCD2 grew +0xE8.
//
// [FUNDAMENTAL vs KCD1] KCD1's standalone S_SoulStatBlock / S_StatModifierBlock moved INSIDE the
// +0x498 RPG aggregate; S_SoulWaitConfig-style timing folded into the +0xAF8 buff list. No
// faction/reputation scalar lives in C_Soul (external managers keyed by the soul WUID, as KCD1).
//
// The +0xA8 block: the C_Soul deep map typed it only as a zeroed 0x48 POD ("derived-stat cache"
// candidate, LOW). The buff-hierarchy deep map INDEPENDENTLY proves CommitModifiers
// (sub_18063B7C8) sorted-inserts modifier nodes into sub-lists at C_Soul+0xA8 and sets the dirty
// byte @+0xE8 -- so it is typed here as the soul-side SORTED modifier list heads (cross-dossier
// synthesis, MED confidence, flagged).

namespace wh::rpgmodule {

class C_DogSoulComponent;           // dog-archetype component (vtable via dtor sub_180964110)
class C_SkillTeacherSoulComponent;  // 0xF0 skill-teacher component (ctor sub_180A1A4A8)
class C_SoulAbilityPerk;            // the ability granter (C_SoulAbilityPerk.h)

// 0x10 mailbox subscription (ctor sub_1803F1F40(this, soul); callback sub_180F42930; msg id 102).
struct S_SoulMailboxSub {
    C_Soul* m_pOwner;    // +0x00
    int32_t m_msgId;     // +0x08  = 102
    uint8_t m_flag0C;    // +0x0C
    uint8_t _pad0D[3];   // +0x0D
};
static_assert(sizeof(S_SoulMailboxSub) == 0x10, "S_SoulMailboxSub must be 0x10");

class C_Soul
    : public I_Soul                                      // +0x00  primary vtable 0x183F44F28
    , public wh::entitymodule::I_ItemCollectionListener  // +0x08  secondary vtable 0x183F45508
    , public UnsafeOp::CryDeferrableSlot                 // +0x10  stateful 0x10 deferrable (RTTI CHD base)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Soul;

    // ---- engine-function forwarders (src/rpgmodule/C_Soul.cpp) ----
    // sub_180648B18(soul, statId, 0) -- native 218-value derived-stat evaluator.
    float GetDerivedStat(E_DerivedStat statId) const;
    // Lua HasAbility semantics (handler 0x182CF7D88): worker sub_1809DCC70 searches the
    // sorted ability block @+0x320 (also true when the all-abilities cvar is set); ids
    // 0 / 73 additionally require derived stat 186 / 187 > 0. KCD2 ability ids differ
    // from KCD1's E_SoulAbility -- not yet enumerated, hence the raw uint32_t.
    bool HasAbility(uint32_t abilityId) const;
    // vtable slot +0x340 impl sub_18046F81C: modifier-adjusted skill level (base @+0x498
    // aggregate slot 8*skillId, modifier visitor over m_sortedModifierLists[1], clamped to
    // [0, S_Constants maxLevel]) divided by maxLevel -> [0,1].  Skill 6 = alchemy: this is THE
    // skill input of the brew-tolerance lerp (I_RPGMinigames::GetBrewTolerance).  The flag
    // rides into the visitor context; the game's tolerance path leaves it UNINITIALIZED
    // (proven ignored on the skill path) -- pass false.
    float GetSkillFraction(uint32_t skillId, bool visitorFlag = false) const;
    // vtable slot +0x310 impl sub_180649F1C: folds every active m_sortedModifierLists[2] node
    // matching statId onto seed, then the id-specific clamp (40 unfloored, 83 >= -1,
    // {7,8,9,23,24,26,36,53} [0,1], everything else >= 0).  The alchemy grader
    // (SelectProductByQuality_182E149A4) calls it with statId apq(74), seed = the summed
    // quality contributions, ctx = null.
    float GetPerkStatModifier(E_PerkStat statId, float seed = 0.0f, void* ctx = nullptr) const;
    Offsets::IEntity* GetBoundEntity() const;  // resolves m_entityGuid via IEntitySystem::FindEntityByGuid+GetEntity; null if unbound

    UnsafeOp::CryDeferrableSlot m_deferred2;   // +0x20  second deferrable -- MEMBER, not in the CHD
    wh::framework::WUID m_selfWuid;            // +0x30  ctor inits INVALID (-1, qword_185332358); factory
                                               //        writes slotIdx | 0x05 << 56 (soul WUID tag = 5)
    EntityGUID m_entityGuid;                   // +0x38  IEntity::GetGuid() (NOT CryGUID/WUID) -- sub_180AD7EE4
    CryStringT<char> m_name;                   // +0x40  init "<not-initialized-soul>"
    CryGUID  m_guid;                           // +0x48  SetGuid sub_1803F1A60; registered in the global
                                               //        GUID->Soul map @(qword_1853322A0 + 128)
    C_SoulPropertyNotifier m_propertyNotifier; // +0x58  (0x50) THE property-changed signal C_Actor+0xF0 connects to
    S_ModifierNode* m_sortedModifierLists[8];  // +0xA8  soul-side SORTED modifier lists (CommitModifiers inserts
                                               //        keyed on eOp; 8 heads mirror the 8 staging lists --
                                               //        cross-dossier synthesis, MED, see header comment)
    bool     m_modifierListsDirty;             // +0xE8  set by the unsorted cat7 commit
    uint8_t  _padE9[7];                        // +0xE9
    C_CombatSoul    m_combatSoul;              // +0x0F0  (0xA8; 7 signals + 2 smart-ptrs)
    C_InventorySoul m_inventorySoul;           // +0x198  (0x148; MI, 3 vtables). Soul primary vf +0x1E0
                                               //         (slot 60) returns &m_inventorySoul -- the equip/
                                               //         unequip route (0x1808ED9E4 / 0x1808C73D0)
    C_DogSoulComponent* m_pDogComponent;       // +0x2E0  owned; alloc'd when archetype-desc(+0xCB8)[+0x18]==6 via sub_180964FDC; dtor sub_180964110 stores C_DogSoulComponent vtable
    void*    m_pOwned2E8;                      // +0x2E8  owned ptr, POD pointee (dtor sub_1809662EC just frees via sub_181AB5160); pointee class unresolved
    C_SkillTeacherSoulComponent* m_pSkillTeacher; // +0x2F0  owned 0xF0 obj; ctor sub_180A1A4A8 stores C_SkillTeacherSoulComponent vtable; dtor sub_1809662FC deletes via vslot+0x10
    void*    m_pOwned2F8;                      // +0x2F8  owned ptr to 8-byte soul-listener holding C_Soul* (ctor sub_1809649C8 when archetype-desc(+0xCB8)[+0x18]==1; dtor sub_180964298 unregisters + frees)
    uint64_t m_unk300[2];                      // +0x300  OWORD zero-init
    uint32_t m_unk310;                         // +0x310
    uint32_t _pad314;                          // +0x314
    uint64_t m_lock318;                        // +0x318  lock/handle (unknown_libname_5 init; dtor sub_180966374)
    boost::container::flat_map<uint32_t, C_SoulAbilityPerk*>
             m_soulAbilities;                  // +0x320  granted soul-ability id -> granter (first
                                               //         grant wins); C_SoulAbilityPerk::ApplyEffects
                                               //         emplaces RAW perk_soul_ability ids -- no
                                               //         validation, custom table ids resolve
    C_Soul*  m_soulAbilitiesOwner;             // +0x338  ctor sub_1803F1EBC; HasAbility's worker
                                               //         reads special-case sources (18/62) via it
    S_SoulRegistry m_registry;                 // +0x340  (0x158; relationship/reputation candidate, UNCONFIRMED)
    C_SoulRPGStats m_rpgStats;                 // +0x498  (0x660; stats/skills/perks/companions)
    C_SoulBuffList m_buffList;                 // +0xAF8  (0x198; OWNS the buff instances)
    uint32_t m_stateC90;                       // +0xC90  init 2 (state/enum, meaning unresolved)
    uint32_t _padC94;                          // +0xC94
    uint64_t m_unkC98[2];                      // +0xC98  OWORD zero-init
    uint64_t m_unkCA8[2];                      // +0xCA8  OWORD zero-init
    const void* m_pDescriptorCB8;              // +0xCB8  non-owning; interned from registry qword_1853302B0 by id@soul+0x374 (sub_1803F354C), default &qword_185584090; [+0x18]=archetype/race enum (6->dog,1->listener)
    const void* m_pDescriptorCC0;              // +0xCC0  non-owning; interned from registry qword_185330030 by id@soul+0x370 (sub_1803F354C), default &dword_1855840F0
    const void* m_pDescriptorCC8;              // +0xCC8  non-owning; interned from registry qword_1853322A0 by id@soul+0x368 (sub_1803F36E0), default &unk_18493C848 (=0xFFFFFFFF null record)
    void*    m_weakRefCD0[2];                  // +0xCD0  intrusive weak-observer hook {subject+0x10, subject}; sub_1803F28C8 links into subject+0x78/+0x80 with refcounts; dtor sub_18096638C + TaskStack-like unlink
    uint64_t m_unkCE0[2];                      // +0xCE0  non-owning 16-byte pair, zero-init (ctor sub_1823C92A0 duplicates a source handle into both slots); NO dtor -> not owning; exact type unresolved
    uint32_t m_flagsCF0;                       // +0xCF0  bit5 = byte_1856698FA (ctor sub_1803F1ED4)
    uint32_t _padCF4;                          // +0xCF4
    void*    m_pOwnedCF8;                      // +0xCF8  owned, lazily-alloc'd heap object (ctor 0); dtor sub_180966574 destroys a 16-byte-element sub-container@pointee+0x20 then frees
    void*    m_pManagerD00;                    // +0xD00  non-owning cached ptr to the global soul-manager singleton qword_1854B95A0 (sub_180966488/sub_18063BC1C register the soul and cache the singleton); never freed
    S_SoulMailboxSub m_mailboxSub;             // +0xD08  (0x10; msg id 102)
    void*    m_srwLock;                        // +0xD18  SRWLOCK (ctor sub_180BBD62C zeroes Ptr then InitializeSRWLock)
};
static_assert(sizeof(C_Soul) == 0xD20, "C_Soul must be 0xD20");
static_assert(offsetof(C_Soul, m_sortedModifierLists) == 0xA8, "sorted modifier lists at 0xA8");
static_assert(offsetof(C_Soul, m_soulAbilities) == 0x320, "soul-ability map at 0x320");
static_assert(offsetof(C_Soul, m_soulAbilitiesOwner) == 0x338, "flat_map is {data,size,cap} = 0x18");
static_assert(offsetof(C_Soul, m_rpgStats) == 0x498, "rpg stats at 0x498");
static_assert(offsetof(C_Soul, m_buffList) == 0xAF8, "buff list at 0xAF8");

}  // namespace wh::rpgmodule
