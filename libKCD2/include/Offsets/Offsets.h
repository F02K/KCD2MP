#pragma once
#include <cstdint>
#include "REL.h"   // REL::ID / REL::Relocation / REL::Module (address resolution)

// -----------------------------------------------
// Global singleton offsets (KCD2 / WHGame.dll 1.5.6)
// -----------------------------------------------
// Every getter resolves via REL::ID(<id>), an abstract cross-version id (Steam/GOG;
// see analysis/addresslib/address_library/kcd2_id_registry.csv). Look up a new id via
// analysis/addresslib/lookup_id.py <addr>.
// As more singletons are RE'd, add their GetInstance() to the owning class header
// (mirroring libKCD1); the raw ones the SDK glue needs live here.

namespace Offsets {

// WHGame.dll base address helper (== REL::Module::base()).
uintptr_t GetBase();

// The CryAction framework singleton moved to its RE'd concrete class:
// CCryAction::GetInstance() (include/crysystem/CCryAction.h).

}  // namespace Offsets
