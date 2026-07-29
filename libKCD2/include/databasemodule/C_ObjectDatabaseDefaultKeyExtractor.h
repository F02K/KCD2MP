#pragma once

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<TObject> -- key-extraction policy for
// C_ObjectDatabaseKeyIndexed (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template, policy parameter only (appears exclusively inside
// C_ObjectDatabaseKeyIndexed TDs; no standalone vtable or ctor observed).  Presumably pulls
// the default key field from TObject [semantics UNVERIFIED].  Tag type -- no members.

namespace wh::databasemodule {

template <typename TObject>
struct C_ObjectDatabaseDefaultKeyExtractor {
};

}  // namespace wh::databasemodule
