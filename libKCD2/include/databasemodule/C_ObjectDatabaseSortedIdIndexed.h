#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseSortedIdIndexed<TBase, TId> -- sorted-id-index decorator
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template (C_PerkDatabase = C_ObjectDatabaseSortedIdIndexed<C_ObjectTableDatabase<
// S_Perk, S_PerkDBData>, CryGUID> -- full untruncated name; C_ReputationChangeDatabase uses the
// same shape). Adds ONLY the sorted index vector @+0x98 (ctor-zeroed {begin,end,cap}); the
// unordered_maps seen at +0xB0 in some databases are CONCRETE-class members, not template ones
// (C_PerkDatabase's ctor zeroes plain storage there instead -- cross-checked across four DBs).
// Element typing of the index is [INFERRED] from the TId arg (sorted id -> row refs).

namespace wh::databasemodule {

template <typename TBase, typename TId>
class C_ObjectDatabaseSortedIdIndexed : public TBase {
public:
    std::vector<void*> m_sortedIndex;   // +0x98  ctor zeroes; element type UNRESOLVED (TId-keyed)
};

}  // namespace wh::databasemodule
