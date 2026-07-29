#include "rpgmodule/C_FactionManager.h"
#include "Offsets/Offsets.h"

// C_FactionManager engine-function forwarders.
// The manager is a function-local static OBJECT at 0x1854B88C0; the accessor
// sub_180471F74 returns &it and runs the TLS-guarded lazy-init (ctor sub_181330B8C,
// atexit dtor sub_1821817E0) on first call.

namespace wh { namespace rpgmodule {

C_FactionManager* C_FactionManager::GetInstance()
{
    using Fn = C_FactionManager* (__fastcall*)();
    static REL::Relocation<Fn> fn{ REL::ID(2) };  // -> &C_FactionManager
    return fn();
}

}}  // namespace wh::rpgmodule
