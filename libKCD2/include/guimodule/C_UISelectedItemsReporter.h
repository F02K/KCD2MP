#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::C_UISelectedItemsReporter -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18 (UNVERIFIED tail).
// -----------------------------------------------
// Selected-items reporter embedded BY VALUE in C_UIModalDialogCombine @+0x280 (vfptr
// written by the Combine ctor sub_181951CE4). Size 0x18 inferred from the Combine
// layout (vfptr @+0x280, two ctor-zeroed qwords @+0x288/+0x290, next Combine member
// byte @+0x298) -- the qword pair MAY belong to Combine instead; boundary UNVERIFIED.
// OPAQUE SHELL: vtable slots and member roles NOT dumped this wave.

namespace wh::guimodule {

class C_UISelectedItemsReporter {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UISelectedItemsReporter;
    virtual ~C_UISelectedItemsReporter();   // [0]; further slots UNVERIFIED

    C_UIFlashBase* m_pScreen;   // +0x08  flash-callable target: vtbl[0]/[1] (ChangeSelection true/false handlers 0x181779978/0x182B13890) read this and drive it as a C_UIFlashBase (vf[12] pre-hook + m_pElement@+0x48 + IUIElement::CallFunction vf[+0x228]) via sub_1817799E4
    uint64_t m_unk10;   // +0x10  ctor 0 [role UNVERIFIED; possibly owned by Combine]
};
static_assert(sizeof(C_UISelectedItemsReporter) == 0x18, "C_UISelectedItemsReporter must be 0x18");

}  // namespace wh::guimodule
