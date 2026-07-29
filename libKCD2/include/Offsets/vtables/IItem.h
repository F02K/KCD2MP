#pragma once
#include <cstddef>
#include "IActor.h"   // IGameObjectExtension data model + IGameObjectProfileManager

// -----------------------------------------------
// IItem -- KCD2 WHGame.dll 1.5.6 (utem).  Uncertified-vtable stand-in.
// -----------------------------------------------
// RTTI .?AUIItem@@ (CryAction SDK): IItem <- IGameObjectExtension <- IComponent (+ esft) --
// the interface the framework item extensions derive through CGameObjectExtensionHelper<T,
// IItem>.  The vtable is INTERFUSCATOR-SHUFFLED and, unlike IActor's, has NOT been slot-
// certified: only the destructor is declared, and it exists purely to give the base its vptr.
// NEVER call or delete through this type.  Data layout is the shared extension storage
// (IGameObjectExtension, verified by the chain base ctor sub_181354AB4 / the inlined copy in
// C_PickableItem's ctor sub_1803F08EC).

namespace Offsets {

struct IItem : IGameObjectExtension {
    virtual ~IItem() = default;   // vptr placeholder ONLY -- slot order uncertified
};
static_assert(sizeof(IItem) == 0x40, "vptr + IGameObjectExtension storage");

}  // namespace Offsets
