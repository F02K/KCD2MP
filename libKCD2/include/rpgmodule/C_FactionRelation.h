#pragma once
#include <cstdint>
#include <set>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionRelation -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// RTTI .?AVC_FactionRelation@rpgmodule@wh@@ (COL 0x18412A370; CHD numBases=1 self only -> STANDALONE
// polymorphic class, own vtable 0x183A39228, NO C++ base). RTTR reflection alias "wh::rpgmodule::
// Relation" (registrar sub_1801B7B40). Element of C_Faction's "Relations" std::vector (@C_Faction
// +0x1B8; element dtor sub_18154B5BC, stride 56). sizeof confirmed at 4 sites (alloc 56, vector
// insert stride 56, element-destruct loop stride 56).
//
// Returned by value (sret) from C_FactionManager::GetRelation(targetId, observerId) = sub_1812198BC:
// resolves both faction NAMES via the GLOBAL name->faction registry qword_185330820 (NOT the
// manager's +0x10/+0x20 compact indices), walks the faction parent hierarchy to compute reputation.

namespace wh::rpgmodule {

// Relation kind stored @+0x18 (real binary enum name E_RelationType, RTTR-registered). Enumerator
// names inferred from the reputation-sign logic in the ctor (reputation<0 -> 2, else predicate?1:0).
enum class E_RelationType : int32_t {
    Neutral = 0,   // inferred
    Friend  = 1,   // inferred
    Enemy   = 2,   // inferred
};

class C_FactionRelation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionRelation;
    virtual ~C_FactionRelation() = default;   // +0x00  own vtable (standalone polymorphic)

    CryStringT<char> m_target;      // +0x08  RTTR "target" (target-faction name)
    void*    m_factionRef;          // +0x10  NON-owning faction-side handle (= *(*(faction+0x88)+0x30);
                                    //        default global qword_185330810); not RTTR-registered; pointee unresolved
    E_RelationType m_type;          // +0x18  RTTR "type"
    uint32_t _pad1C;                // +0x1C
    std::set<uint32_t> m_qualities; // +0x20  RTTR "qualities" (projected to/from a space-separated
                                    //        string); element = 4B quality/tag id (meaning unresolved)
    float    m_reputation;          // +0x30  RTTR "reputation" (getter sub_181A7E440)
    uint32_t _pad34;                // +0x34
};
static_assert(sizeof(C_FactionRelation) == 0x38, "C_FactionRelation must be 0x38");
static_assert(offsetof(C_FactionRelation, m_qualities) == 0x20, "qualities set at 0x20");
static_assert(offsetof(C_FactionRelation, m_reputation) == 0x30, "reputation at 0x30");

}  // namespace wh::rpgmodule
