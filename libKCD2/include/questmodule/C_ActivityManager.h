#pragma once
#include <cstdint>
#include "../conceptmodule/C_NodeManager.h"
#include "../REL/Relocation.h"

// -----------------------------------------------
// wh::questmodule::C_ActivityManager -- global activity registry (KCD2 1.5.6, kd7u).
// sizeof 0x20 (alloc 32).
// -----------------------------------------------
// Own vtable 0x183ED64B0. LAZY GLOBAL SINGLETON: getter sub_181F43610 double-check-locks on
// unk_18492EB40 and stores the instance in qword_1855A77E0 -- GetInstance() below forwards
// to that getter, so it also CONSTRUCTS the manager on first call.
// CHEAT: +0x18 seeds the manager's MT19937 (5489 = the mt19937 default) used for
// random-activity selection.

namespace wh::questmodule {

class C_ActivityManager : public conceptmodule::C_NodeManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActivityManager;
    uint8_t  m_sub08[0x10];   // +0x08  sub_1803F7538 subobject [shape not walked]
    uint64_t m_rngSeed;       // +0x18  ctor 5489 (MT19937 default seed)

    [[nodiscard]] static C_ActivityManager* GetInstance()
    {
        using Fn = C_ActivityManager* (__fastcall*)();
        static REL::Relocation<Fn> fn{ REL::ID(66) };   // lazy-construct getter
        return fn();
    }
};
static_assert(sizeof(C_ActivityManager) == 0x20, "C_ActivityManager must be 0x20 (alloc 32)");

}  // namespace wh::questmodule
