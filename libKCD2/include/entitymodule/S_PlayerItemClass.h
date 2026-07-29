#pragma once
#include <cstddef>
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "S_PickableItemClass.h"

// -----------------------------------------------
// wh::entitymodule::S_PlayerItemClass -- player-facing item classes: the UI presentation view
// (KCD2 WHGame.dll 1.5.6).  sizeof 0xB0.  E_ItemType 25 (intermediate).
// -----------------------------------------------
// Vtable 0x183A4F018, ctor sub_180754BD8.  THIS is what S_ItemClass::GetAsPlayerItemClass
// (vf[25]) returns -- GetItemUiPresentation (0x180515434) reads m_iconId/m_uiName off that
// view; classes outside this branch yield the shared static default at 0x1855DD210.
namespace wh::entitymodule {

class S_PlayerItemClass : public S_PickableItemClass {
public:
    CryStringT<char> m_iconId;  // +0x90  icon name ("IconId") -- GetItemUiPresentation outIcon
    CryStringT<char> m_unk98;   // +0x98  secondary string [role UNVERIFIED]
    CryStringT<char> m_uiName;  // +0xA0  display-name localization key ("UIName") -- localized
                                //        into GetItemUiPresentation outName
    bool     m_flagA8;          // +0xA8  ctor 0
    bool     m_flagA9;          // +0xA9  ctor 0
    bool     m_flagAA;          // +0xAA  ctor 1
    uint8_t  _padAB[5];         // +0xAB
};

static_assert(sizeof(S_PlayerItemClass) == 0xB0, "S_PlayerItemClass must be 0xB0");
static_assert(offsetof(S_PlayerItemClass, m_iconId) == 0x90, "icon at 0x90");
static_assert(offsetof(S_PlayerItemClass, m_uiName) == 0xA0, "ui name at 0xA0");

}  // namespace wh::entitymodule
