#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_IntermissionManager -- intermission manager interface;
// base of C_IntermissionManager (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_IntermissionManager@xgenaimodule@wh@@.  Shape (5 slots)
// read from the sole implementor C_IntermissionManager (vtable rva 0x4014358:
// [0] dtor sub_18062358C, [1]sub_1819D571C [2]sub_180623610 [3]sub_1806233F0
// [4]sub_1834480F4 -- Begin/End/Update mapping NOT confirmed [U]).

namespace wh::xgenaimodule {

class I_IntermissionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_IntermissionManager;
    virtual ~I_IntermissionManager();  // [0]
    virtual void ImVf1() = 0;   // [1] [U role]
    virtual void ImVf2() = 0;   // [2] [U role]
    virtual void ImVf3() = 0;   // [3] [U role]
    virtual void ImVf4() = 0;   // [4] [U role]
};
static_assert(sizeof(I_IntermissionManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
