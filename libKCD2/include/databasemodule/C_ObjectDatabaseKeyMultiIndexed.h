#pragma once

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseKeyMultiIndexed<TBase, ...> -- multi-key index decorator
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template (C_DocumentRewardPerkDatabase stack). Sibling of the (Sorted)IdIndexed
// decorators. MEMBER LAYOUT UNRESOLVED: the only observed concrete ctor (sub_18153FBE0) writes
// nothing beyond the table layer, so this mirror declares NO members and must not be used for
// sizeof/offset math. (A C_ObjectDatabaseKeyIndexed / C_ObjectTreeDatabase /
// C_ObjectDatabaseDefaultKeyExtractor family also exists -- see C_SoulStateEffectContextDatabase's
// ctor sub_180F9F018 -- none of it RE'd yet.)

namespace wh::databasemodule {

template <typename TBase>
class C_ObjectDatabaseKeyMultiIndexed : public TBase {
public:
};

}  // namespace wh::databasemodule
