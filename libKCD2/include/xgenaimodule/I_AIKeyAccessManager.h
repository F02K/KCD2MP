#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_AIKeyAccessManager -- AI key-access service interface;
// base of the stateless C_AIKeyAccessManager singleton (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_AIKeyAccessManager@xgenaimodule@wh@@.  Shape (6 slots)
// read from the sole implementor C_AIKeyAccessManager (vtable rva 0x400EF38:
// [0] dtor sub_18328155C, [1]sub_180CC5FD8 [2]sub_18070B324 [3]sub_183281804
// [4]sub_180CC6184 [5]sub_180709358 -- RequestKey/ReleaseKey/query mapping NOT
// confirmed [U]).

namespace wh::xgenaimodule {

class I_AIKeyAccessManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AIKeyAccessManager;
    virtual ~I_AIKeyAccessManager();  // [0]
    virtual void KeyVf1() = 0;   // [1] [U role]
    virtual void KeyVf2() = 0;   // [2] [U role]
    virtual void KeyVf3() = 0;   // [3] [U role]
    virtual void KeyVf4() = 0;   // [4] [U role]
    virtual void KeyVf5() = 0;   // [5] [U role]
};
static_assert(sizeof(I_AIKeyAccessManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
