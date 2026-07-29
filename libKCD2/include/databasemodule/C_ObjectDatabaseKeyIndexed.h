#pragma once

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseKeyIndexed<TBase, TExtractor> -- single-key index
// decorator (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template; decorates both stacks: over C_ObjectTableDatabase (e.g. the
// combatmodule C_CombatWeaponCombinationData database) and over C_ObjectTreeDatabase (e.g.
// C_SoulStateEffectContextDatabase), always paired with C_ObjectDatabaseDefaultKeyExtractor<Row>.
// LAYOUT RESOLVED EMPTY for the tree stack: C_AlchemyRecipeDatabase (0x80) places its MI
// interface vptr directly at +0x78 = the C_ObjectTreeDatabase extent, so this layer adds NO
// data there (ctor sub_1819AAF18 initializes nothing beyond the tree fields).  Its by-key
// lookup is index-free: std::lower_bound over the key-SORTED inline rows of the base's +0x30
// vector, key read through the row vtable (C_AlchemyRecipeDatabase impl sub_1808C3C0C via
// helper sub_181FFDEA0).  [Table-stack instantiations unaudited -- keep sizeof math to
// per-concrete static_asserts.]

namespace wh::databasemodule {

template <typename TBase, typename TExtractor>
class C_ObjectDatabaseKeyIndexed : public TBase {
public:
};

}  // namespace wh::databasemodule
