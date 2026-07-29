#include "playermodule/C_FastTravel.h"
#include "Offsets/Offsets.h"

// C_FastTravel engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, kd7u).

namespace wh::playermodule {

void C_FastTravel::StartTravel()
{
    // 0x182DE28F4 -- the real start of an armed travel.  Gate sub_182DDE634, set up FT mode
    // (control/HUD lockdown, m_isFastTraveling+0x48=1, build executor), then tail-call
    // sub_182DE0DE8, whose ctx+0x130->+0x98 vtable[67] executor moves the player + skips time.
    // Arm first via C_UIMap::SetDestination; NOP if not ready.
    using Fn = char (__fastcall*)(C_FastTravel*);
    static REL::Relocation<Fn> fn{ REL::ID(101) };
    fn(this);
}

void C_FastTravel::CancelFastTravel()
{
    // 0x182DDFBD0: stop/cancel entry.
    using Fn = void (__fastcall*)(C_FastTravel*);
    static REL::Relocation<Fn> fn{ REL::ID(100) };
    fn(this);
}

bool C_FastTravel::IsFastTraveling() const
{
    // 0x180534E5C: m_isFastTraveling (+0x48) || skip-time session active byte (+0x110 -> +0x68).
    using Fn = bool (__fastcall*)(const C_FastTravel*);
    static REL::Relocation<Fn> fn{ REL::ID(7) };
    return fn(this);
}

}  // namespace wh::playermodule
