#pragma once
#include <cstdint>
#include "I_IntermissionManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_IntermissionManager : I_IntermissionManager --
// intermission (cutscene/transition) manager SINGLETON at qword_185492170
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xC0 (getter sub_1817897FC
// @0x18178984c: qword_18549D378(192,...), += 0xC0, ctor sub_181789898).
// No external registration in the ctor.
// -----------------------------------------------
// RTTI TD rva 0x4FF5BC8.  Vtable rva 0x4014358 (5 slots: [0] dtor
// sub_18062358C, [1]sub_1819D571C [2]sub_180623610 [3]sub_1806233F0
// [4]sub_1834480F4 -- Begin/End/Update mapping [U]).  Layout closely parallels
// C_NPCMessageDispatcher but larger; container fields typed as blobs [U].

namespace wh::xgenaimodule {

class C_IntermissionManager : public I_IntermissionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_IntermissionManager;
    ~C_IntermissionManager() override;   // [0] sub_18062358C
    void ImVf1() override;   // [1] sub_1819D571C [U role]
    void ImVf2() override;   // [2] sub_180623610 [U role]
    void ImVf3() override;   // [3] sub_1806233F0 [U role]
    void ImVf4() override;   // [4] sub_1834480F4 [U role]

    int32_t  m_08;            // +0x08  ctor: 258 (0x102) [U meaning]
    uint8_t  _pad0C[4];       // +0x0C
    uint64_t m_10;            // +0x10  ctor: 0 [U role]
    uint64_t m_18;            // +0x18  ctor: 0 [U role]
    uint8_t  m_zero20[0x30];  // +0x20..+0x50  3x OWORD zeroed (vector-like) [U types]
    int32_t  m_50;            // +0x50  ctor: -1 [U role]
    int32_t  m_54;            // +0x54  ctor: 0 [U role]
    uint8_t  m_zero58[0x60];  // +0x58..+0xB8  zeroed containers/vectors [U types]
    I_IntermissionInvoker* m_current;  // +0xB8  active invoker; wr 0x180623445 (queue head @+0x58) / 0x180623851 (next via sub_1806238E8), cleared 0x18062383a; vf4 derefs -> calls its vf[1]
};
static_assert(sizeof(C_IntermissionManager) == 0xC0, "must be 0xC0 (alloc 192 at sub_1817897FC)");

}  // namespace wh::xgenaimodule
