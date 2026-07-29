#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"
#include "../game/S_GameContext.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogModule -- dialog module spine (KCD2 1.5.6, kd7u).
// sizeof 0x20 (alloc 32).
// -----------------------------------------------
// RTTI TD 0x184B4DF90. Base: framework::C_BaseModule (-> I_ModuleMessageListener, shared
// primary vtable 0x183E64C38, 10 slots = C_BaseModule shape [0..6] + RTTR trio [7..9]).
// ctor sub_18120FEB8; registrar sub_180C9DA44 @0x180c9dc5f (alloc 32, publishes into
// S_GameContext +0x150 via sub_180EF3AFC(ctx, 1)). Slots:
// [1] OnModuleMessage sub_181141B80 (switch on *(msg+8): 0x12/0x47/0x48/0x4C generic
//     tick, 0x25 payload msg, 0x36, 0x3F -- all delegate to m_pManager),
// [2] Init sub_180E2F1E0 (allocs the 0x1F0 C_DialogManager, registers all wh_dlg_*
//     CVars via sub_180E3DE3C, subscribes message ids 8/15),
// [3] Deinit sub_1828041DC, [4] Update sub_180533000 (forwards to manager),
// [5] GetModuleId = 14 (sub_181A7D850), [6] GetModuleName = "DialogModule"
//     (sub_181A71AD0).

namespace wh::dialogmodule {

class C_DialogManager;

class C_DialogModule : public framework::C_BaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogModule;
    C_DialogManager* m_pManager;   // +0x10  set by Init; deleted by Deinit; Update target
    uint8_t          _pad18[8];    // +0x18  storage for a data-free RAII console-command guard + tail padding; module dtor sub_1828034B4 does `add rcx,18h; call sub_1827F2BDC` (0x1828034C7), and sub_1827F2BDC ignores 'this' -- it RemoveCommand's wh_dlg_reloadFunctions/ReloadDB/SetAnimDurationScales via the global IConsole. No typed data member; never written by any code.

    [[nodiscard]] static C_DialogModule* GetInstance()
    {
        return game::S_GameContext::GetInstance()->m_pDialogModule;
    }
};
static_assert(sizeof(C_DialogModule) == 0x20, "C_DialogModule must be 0x20 (alloc 32)");

}  // namespace wh::dialogmodule
