#pragma once
#include "I_AIKeyAccessManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIKeyAccessManager : I_AIKeyAccessManager -- stateless
// AI key-access service SINGLETON at qword_185496140 (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08 (getter sub_1813C08C8 @0x1813c0912:
// qword_18549D378(8,...), += 8; ctor inlined -- writes only the vptr).
// -----------------------------------------------
// RTTI TD rva 0x4FE8F88.  Vtable rva 0x400EF38 (6 slots: [0] dtor
// sub_18328155C, [1]sub_180CC5FD8 [2]sub_18070B324 [3]sub_183281804
// [4]sub_180CC6184 [5]sub_180709358).

namespace wh::xgenaimodule {

class C_AIKeyAccessManager : public I_AIKeyAccessManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIKeyAccessManager;
    ~C_AIKeyAccessManager() override;   // [0] sub_18328155C
    void KeyVf1() override;   // [1] sub_180CC5FD8 [U role]
    void KeyVf2() override;   // [2] sub_18070B324 [U role]
    void KeyVf3() override;   // [3] sub_183281804 [U role]
    void KeyVf4() override;   // [4] sub_180CC6184 [U role]
    void KeyVf5() override;   // [5] sub_180709358 [U role]
};
static_assert(sizeof(C_AIKeyAccessManager) == 0x08, "stateless: vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
