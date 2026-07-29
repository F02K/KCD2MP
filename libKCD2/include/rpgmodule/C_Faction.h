#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "C_FactionBase.h"
#include "C_FactionAngriness.h"
#include "C_FactionRelation.h"
#include "S_LocationId.h"
#include "S_FactionNeighbor.h"
#include "../framework/S_BoostOptional.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_Faction -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x200 (CONFIRMED: alloc 512).
// -----------------------------------------------
// RTTI .?AVC_Faction@rpgmodule@wh@@ (TD 0x184D4E578)  vtable 0x183B1DF10  ctor sub_1811650B0
// (heap ctor sub_181164F48 wraps the object in a std::shared_ptr)  dtor sub_182CA8520.
// Bases: C_FactionBase (mdisp 0) + std::enable_shared_from_this<C_Faction> (mdisp 0x78; its
// internal weak_ptr is what the dtor _Decwrefs at +0x80). FULLY RESOLVED layout: member names
// from the RTTR registrar sub_18106C5E0 ("wh::rpgmodule::Faction"); offsets from ctor/dtor +
// per-vector element dtors; independently verify-confirmed (offsets sum to 0x200, no misses).
//
// [FUNDAMENTAL vs KCD1] Factions live in an EXTERNAL wh::databasemodule::C_ObjectDatabase<
// std::shared_ptr<C_Faction>> / C_ObjectTreeDatabaseKeyIndexed<..., std::map<CryStringT<char>,
// shared_ptr<C_Faction>>> -- the KCD1 "3 flat_maps inside the manager" model does NOT hold.
// KCD1 S_FactionDef == the RTTR-reflected members below; KCD1 S_FactionPersistentRecord has NO
// KCD2 equivalent (C_Faction serializes directly via RTTR; method "OnAfterDeserialization").
// REPUTATION IS NOT A STORED FLOAT: RTTR "reputation" (vtable slot 16 = sub_180B2C298) is
// COMPUTED by locking + aggregating over m_souls; "InitialReputation" (+0xE0) is the serialized
// definition value. RTTR "Visibility" (C_FactionVisibility) is computed via getter/setter
// sub_182CAE368/sub_182CAE838 -- no direct storage.
// Membership query: vtable +0x70 = sub_180475714 (HasLabel/LooksLikeFaction) walks m_labels
// string-comparing, then recurses to the parent faction.
// PERSISTED (RTTR): Children, UIName, UIDescription, UIOrder, UIIcon, LevelId, Name,
// OverrideMemberNumber, InitialReputation, NumChildsCache, LocationId, Labels, Relations, Souls.
// RUNTIME-ONLY: the esft weak_ptr @0x78, both C_FactionAngriness, m_runtime198, m_neighborCache.

namespace wh::rpgmodule {

// ~0xB8, : C_FactionBase, embeds C_Nervousness vtable @+0x90 (ctor sub_1803F21DC).
// Per-NPC faction membership node -- interior not yet RE'd (backlog).
class C_NPCFactionNode;

class C_Faction
    : public C_FactionBase                            // +0x00  (0x78)
    , public std::enable_shared_from_this<C_Faction>  // +0x78  (0x10 weak_ptr; CHD mdisp 0x78)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Faction;
    std::vector<std::shared_ptr<C_Faction>> m_children;  // +0x88  RTTR "Children" (sub-factions;
                                                          //        strong _Decref sub_1804F6588)
    CryStringT<char> m_uiName;             // +0xA0  RTTR "UIName" (reg offset 160)
    CryStringT<char> m_uiDescription;      // +0xA8  RTTR "UIDescription" (168)
    int32_t  m_uiOrder;                    // +0xB0  RTTR "UIOrder" (176)
    uint32_t _padB4;                       // +0xB4
    CryStringT<char> m_uiIcon;             // +0xB8  RTTR "UIIcon" (184)
    int32_t  m_levelId;                    // +0xC0  RTTR "LevelId" (192; init -1)
    uint32_t _padC4;                       // +0xC4
    CryStringT<char> m_name;               // +0xC8  RTTR "Name" (200; database key)
    wh::framework::S_BoostOptional<uint64_t> m_overrideMemberNumber;  // +0xD0  RTTR
                                           //        "OverrideMemberNumber" (builder sub_180EAD2B0,
                                           //        208). Reader sub_1803F30C4: if set return it,
                                           //        else fall back to m_numChildsCache. CORRECTS
                                           //        the earlier S_FactionDefinitionID@+0xD0 claim.
    float    m_initialReputation;          // +0xE0  RTTR "InitialReputation" (builder sub_18106B888, 224)
    int32_t  m_numChildsCache;             // +0xE4  RTTR "NumChildsCache" (228)
    C_FactionAngriness m_angriness[2];     // +0xE8  (2 x 0x50; ctor sub_1811651F4 flags 1/0;
                                           //        array teardown sub_182CA82CC) RUNTIME
    S_LocationId m_locationId;             // +0x188  RTTR "LocationId" (builder sub_180EB1F8C, 392)
    int32_t  m_visibilityRefCount;         // +0x198  RUNTIME faction-visibility ref-counter: C_FactionVisibility effect Activate/Deactivate (sub_182BFEF4C/sub_182BFF248) does += (sign ? +1 : -1); backs RTTR "Visibility" (get sub_182CAE368 / set sub_182CAE838)
    uint32_t _pad19C;                      // +0x19C
    std::vector<CryStringT<char>> m_labels;      // +0x1A0  RTTR "Labels" (via getter sub_180D92404 /
                                           //        setter sub_182CAC2CC; COW release *elem-12)
    std::vector<C_FactionRelation> m_relations;  // +0x1B8  RTTR "Relations" (0x38 elems; elem dtor
                                           //        sub_18154B5BC)
    std::vector<S_FactionNeighbor> m_neighborCache;  // +0x1D0  RUNTIME spatial cache (rebuilt
                                           //        sub_180467ADC; symmetric; threshold cfg[737])
    std::vector<std::weak_ptr<C_NPCFactionNode>> m_souls;  // +0x1E8  RTTR "Souls" (member NPC
                                           //        nodes; reputation getter locks + aggregates)
};
static_assert(sizeof(C_Faction) == 0x200, "C_Faction must be 0x200");
static_assert(offsetof(C_Faction, m_children) == 0x88, "Children at 0x88");
static_assert(offsetof(C_Faction, m_overrideMemberNumber) == 0xD0, "OverrideMemberNumber at 0xD0");
static_assert(offsetof(C_Faction, m_initialReputation) == 0xE0, "InitialReputation at 0xE0");
static_assert(offsetof(C_Faction, m_angriness) == 0xE8, "angriness at 0xE8");
static_assert(offsetof(C_Faction, m_labels) == 0x1A0, "Labels at 0x1A0");
static_assert(offsetof(C_Faction, m_relations) == 0x1B8, "Relations at 0x1B8");
static_assert(offsetof(C_Faction, m_neighborCache) == 0x1D0, "neighbor cache at 0x1D0");
static_assert(offsetof(C_Faction, m_souls) == 0x1E8, "Souls at 0x1E8");

}  // namespace wh::rpgmodule
