#pragma once
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include "C_CombatActorObject.h"
#include "../framework/HashPrimitives.h"
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionManager : C_CombatActorObject -- KCD2 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// ctor sub_1815647D8. Stored at C_CombatActor+0x3A8. Merged vtable 0x183E3FA78 (GetSubsystemId ->
// 3, GetName -> "ActionManager"; the manager adds NO virtuals of its own). Grew from KCD1 0x60 by
// two NEW factories (Guard, Dodge).
//
// The +0x10..+0x50 region is ONE stock MSVC std::unordered_map -- VERIFIED from the insert path
// sub_18091662C (_Xlength_error "unordered_map/set too long", 32-byte nodes, load-factor test
// against the 1.0f at +0x10, FNV-1a + hash_combine over the two key halves) and the dtor
// sub_181645B48. KCD1's reading of +0x10 as a lone float m_timeScale was this map's load factor.
// Key = 8-byte composite of two uint32 ids; value = int32 (node pair ctor sub_1823CE55C zero-inits
// exactly a DWORD). Semantic names of the key halves are unresolved.

namespace wh::combatmodule {

class C_CombatActionAttackFactory;
class C_CombatActionGuardFactory;
class C_CombatActionPerfectBlockFactory;
class C_CombatActionDodgeFactory;

// 8-byte composite key of m_activeActions. Low half = a table-resolved id, high half read from the
// dispatched entry (+336->+8); semantic names unresolved.
struct S_ActionMapKey {
    uint32_t idLow;    // +0x00
    uint32_t idHigh;   // +0x04
    bool operator==(const S_ActionMapKey& o) const { return idLow == o.idLow && idHigh == o.idHigh; }
};

// The real hasher (sub_18091662C): FNV-1a over each 4-byte half, folded with boost hash_combine.
struct S_ActionMapKeyHash {
    uint64_t operator()(const S_ActionMapKey& k) const {
        return wh::shared::hash_combine(wh::shared::fnv1a(&k.idLow, 4), wh::shared::fnv1a(&k.idHigh, 4));
    }
};

class C_CombatActorActionManager : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionManager;
    std::unordered_map<S_ActionMapKey, int32_t, S_ActionMapKeyHash> m_activeActions;  // +0x10  (0x40)
    _smart_ptr<C_CombatActionAttackFactory>       m_pAttackFactory;        // +0x50  CreateAndDispatch entry point
    _smart_ptr<C_CombatActionGuardFactory>        m_pGuardFactory;         // +0x58  NEW in KCD2
    _smart_ptr<C_CombatActionPerfectBlockFactory> m_pPerfectBlockFactory;  // +0x60  (KCD1 +0x58, relocated)
    _smart_ptr<C_CombatActionDodgeFactory>        m_pDodgeFactory;         // +0x68  NEW in KCD2
};
static_assert(sizeof(C_CombatActorActionManager) == 0x70, "C_CombatActorActionManager must be 0x70");
static_assert(offsetof(C_CombatActorActionManager, m_pAttackFactory) == 0x50, "factory block offset");

}  // namespace wh::combatmodule
