#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "framework/I_UIHudCrimeList.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudCrimeList -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_UIHudCrimeList@guimodule@wh@@ (TD 0x184CCFCF8).
// Bases: C_UIFlashBase @+0x00 (COL 0x4136E68), IUIElementEventListener @+0x10
// (COL 0x4136E90), wh::framework::I_UIHudCrimeList @+0x58 (COL 0x4136EB8).
// vtables: primary 0x183C33738 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C336F0 (8), crime-list 0x183C336B8 (6). ctor inlined in the rttr
// as_std_shared_ptr creator sub_181886C44 (alloc 0x60, memset-zeroed). NO own members.
//
// The dialogue crime-list overlay ("what did I get caught for"). Init [1]
// sub_1819278A0: base Init, registers the I_UIHudCrimeList subobject with the crime
// feedback object (*(ctx+0x160)+0xA8 -> vf[+0x1A8]() -> vf[1](this+0x58)) and binds
// the "dialog_crime_list" action-map actions ("up"/"down"/...) to selection lambdas
// (action hub *(ctx+0x128)+0x60, register vf[+0x48]).
// ShowCrimeList/HideCrimeList forward to flash "CrimeListData"+"ShowCrimeList" /
// "HideCrimeList".

namespace wh::guimodule {

class C_UIHudCrimeList : public C_UIFlashBase, public wh::framework::I_UIHudCrimeList {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudCrimeList;
    // I_UIHudCrimeList overrides (vtable 0x183C336B8).
    void ShowCrimeList(const void* pCrimeData) override;   // [1] 0x182B8EB30
    void HideCrimeList() override;                         // [2] 0x182B8DA68
};
static_assert(sizeof(C_UIHudCrimeList) == 0x60, "C_UIHudCrimeList must be 0x60");

}  // namespace wh::guimodule
