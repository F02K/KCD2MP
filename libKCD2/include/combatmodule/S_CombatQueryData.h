#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatQueryData<TDerived> -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// CRTP base for the combat "query" request objects handed to the action-candidate selectors.
// Data-less; owns the vtable (subobject @+0x00). RTTI-confirmed single base of
// S_CombatActionAttackQueryData (TD .?AU?$S_CombatQueryData@U..., 0x4AFA948).
//
// [vs KCD1] KCD1's base had 2 slots (GetTypeNameHash, GetCacheKey). The KCD2 concrete vtable
// (S_CombatActionAttackQueryData @0x183A45488) has 4 slots. Names below are inferred from the
// concrete bodies and are tentative:
//   [0] sub_18275AACC(this,out): writes a hash/name into *out            -> GetTypeName
//   [1] sub_1805F5DA0(this):     returns this                            -> GetSelf
//   [2] sub_18275A87C(this,out): out[0]=this; hashes out+1               -> GetCacheKeyData
//   [3] sub_18277CB58():         constant FNV-seeded + golden-ratio hash -> GetTypeNameHash
// (RVAs: 0x275AACC / 0x5F5DA0 / 0x275A87C / 0x277CB58.)

namespace wh::combatmodule {

template <class TDerived>
struct S_CombatQueryData {
    virtual void*     GetTypeName(void* out) = 0;        // [0]  (tentative)
    virtual TDerived* GetSelf() = 0;                     // [1]  (tentative)
    virtual void*     GetCacheKeyData(void* out) = 0;    // [2]  (tentative)
    virtual uint64_t  GetTypeNameHash() = 0;             // [3]  constant type-name hash
};

}  // namespace wh::combatmodule
