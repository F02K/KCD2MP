#include "playermodule/C_ActionSets.h"
#include "Offsets/Offsets.h"

// C_ActionSets engine-function forwarders (KCD2 WHGame.dll 1.5.6, kd7u;
// ids = kcd2 address library).

namespace wh::playermodule {

void C_ActionSets::TriggerAction(CryStringT<char> action)
{
    // 0x18085A360: m_active lookup -> functor _Copy() -> _Do_call() (nullary).  The name is
    // BY VALUE (MSVC callee-destroys: the impl tail-releases it @0x18085A3CD; game callers
    // copy-construct and never release -- 0x181FFEF20).  A const& pointer type here once
    // double-freed the string heap: three distinct "corruption" crashes before it was traced.
    using Fn = void (__fastcall*)(C_ActionSets*, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(45781) };
    fn(this, action);
}

}  // namespace wh::playermodule
