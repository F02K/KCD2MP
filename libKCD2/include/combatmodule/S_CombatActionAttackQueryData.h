#pragma once
#include <cstdint>
#include "S_CombatQueryData.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatActionAttackQueryData -- KCD2 WHGame.dll 1.5.6 (kd7u). sizeof 0xA0.
// -----------------------------------------------
// The query object that drives attack-candidate enumeration. Built from the attacker's (and
// opponent's) live combat state, then handed to the candidate enumerator (sub_180687F3C, which
// fills an S_AttackCandidateResultHolder). Derives from the CRTP base
// S_CombatQueryData<S_CombatActionAttackQueryData> at +0x00 (RTTI-confirmed). vtable 0x183A45488.
//
// ctor sub_1806881C8 (RVA 0x6881C8): installs the vtable, allocates the +0x28 intrusive-list
// sentinel (sub_180452BA4, a 0x30 node -- freed with size 0x30 by consumers), and defaults a run
// of id fields to -1. The ctor writes no field past +0x9C -> sizeof 0xA0.
//
// [vs KCD1] KCD1's query was 0xC0 with a fully-named field set and its list/tree node at +0xA8.
// KCD2 compacted it to 0xA0 and moved the list node to +0x28. The proven fields are the vtable,
// the attacker actor @+0x08, and the +0x28 list head; the ctor otherwise only sets defaults. The
// per-field SEMANTICS of the id/flag block are filled by the query BUILDERS (e.g. inside the
// automation-attack path sub_18170D350, which writes +0x08 attacker, +0x18/+0x1C flags, +0x64/
// +0x68/+0x84 ids, +0x98=2) and are NOT fully recovered here -- names below are tentative and
// marked UNVERIFIED. Field defaults (ctor) are annotated.

namespace wh::combatmodule {

class C_CombatActor;

struct S_CombatActionAttackQueryData : S_CombatQueryData<S_CombatActionAttackQueryData> {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActionAttackQueryData;
    // +0x00 vtable (from the S_CombatQueryData base)
    C_CombatActor* m_pAttackerActor;   // +0x08  ctor 0; set to the attacker actor by builders
    C_CombatActor* m_pOpponentActor;   // +0x10  ctor 0  (tentative; KCD1 parity)
    uint8_t   m_flag18;                // +0x18  ctor 0
    int32_t   m_field1C;               // +0x1C  filler 0x180687E60: = sub_180688C58(attacker+0x1F8) (uint32 from actor state+0x758 field+0x50); ctor 0
    uint8_t   m_flag20;                // +0x20  ctor 0
    int32_t   m_field24;               // +0x24  filler 0x180687E83: = sub_180688C58(opponent+0x1F8) (uint32; opponent parallel of m_field1C); ctor 0
    std::map<uint64_t, CryStringT<char>> m_stringMap;  // +0x28  std::_Tree: head sentinel @+0x28, size @+0x30. Node value 0x10 = {8B trivial key @+0, CryStringT<char> @+8}. Filled during enumeration; key type inferred
    uint8_t   m_flag38;                // +0x38  ctor 0
    int32_t   m_id3C;                  // +0x3C  ctor -1
    int32_t   m_field40;               // +0x40  filler 0x180687EA3: = (uint8)sub_1804A8CE0(opponent+0x1F8) + 1 (small enum-like ordinal); ctor 0
    int32_t   m_id44;                  // +0x44  ctor -1
    int32_t   m_id48;                  // +0x48  ctor -1
    int32_t   m_id4C;                  // +0x4C  ctor -1
    uint8_t   m_flag50;                // +0x50  ctor 0
    int32_t   m_field54;               // +0x54  filler 0x180687F17: = vfunc[+0x20] of (ctx+0x3B0)+0x10 (ctor leaves indeterminate)
    uint8_t   m_flag58;                // +0x58  ctor 0
    uint8_t   m_flag5C;                // +0x5C  ctor 0
    int32_t   m_field60;               // +0x60  filler 0x180687ED6: = vfunc[+0xB0] of (opponent+0x1F8)+0x990 (opponent branch; ctor leaves indeterminate)
    int32_t   m_ids64[12];             // +0x64..+0x90  ctor all -1 (zone/sub-id key block; per-field roles UNVERIFIED)
    uint8_t   m_flag94;                // +0x94  ctor 0
    int32_t   m_field98;               // +0x98  filler 0x180687EFC / builder 0x18170D3ED: constant = 2 (enum candidate {2}); ctor 0
    int32_t   m_id9C;                  // +0x9C  ctor -1
    // ---- vtable overrides (installed by the engine ctor) ----
    void*     GetTypeName(void* out) override;       // [0] 0x275AACC
    S_CombatActionAttackQueryData* GetSelf() override; // [1] 0x5F5DA0
    void*     GetCacheKeyData(void* out) override;   // [2] 0x275A87C
    uint64_t  GetTypeNameHash() override;            // [3] 0x277CB58

    S_CombatActionAttackQueryData();   // engine ctor sub_1806881C8 (RVA 0x6881C8)
};
static_assert(sizeof(S_CombatActionAttackQueryData) == 0xA0, "S_CombatActionAttackQueryData must be 0xA0");

}  // namespace wh::combatmodule
