#pragma once
#include "I_XGenAIModulePatch.h"

// -----------------------------------------------
// wh::xgenaimodule::C_Patch1_3 : I_XGenAIModulePatch : framework::I_Patch -- the
// 1.3 savegame/DB migration patch (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD rva 0x4FE8FC8; vtable 0x18400F9D0, 6 slots -- overrides all inherited
// slots, adds none: [0] sub_181F23620 (shared trivial dtor), [1] sub_1820BDC90,
// [2] sub_1820BD7E0, [3] sub_1820BDDB0 (-> &unk_18558D440), [4]/[5] sub_1820BDD30
// (shared body). Installer sub_1820BDD80. Which save fields it rewrites: UNRESOLVED
// (low cheat value). No members observed; modeled vptr-only -- true allocation size
// UNVERIFIED (heap patch step, never embedded).

namespace wh::xgenaimodule {

class C_Patch1_3 : public I_XGenAIModulePatch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Patch1_3;
    void _vf1() override;   // [1] sub_1820BDC90 [role UNVERIFIED]
    void _vf2() override;   // [2] sub_1820BD7E0 [role UNVERIFIED]
    void _vf3() override;   // [3] sub_1820BDDB0 -> static patch descriptor
    void _vf4() override;   // [4] sub_1820BDD30
    void _vf5() override;   // [5] sub_1820BDD30 (same body as [4])
};
static_assert(sizeof(C_Patch1_3) == 0x08, "C_Patch1_3 modeled vptr-only (alloc size UNVERIFIED)");

}  // namespace wh::xgenaimodule
