#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::guimodule::GetItemUiPresentation -- item class guid -> display strings
// (KCD2 WHGame.dll 1.5.6, utem).
// -----------------------------------------------
// Forwarder for sub_180515434 (REL::ID 30029), the resolver the HUD ItemTransfer toast
// marshaller (C_UIHudEvents I_UIHudEvents[0] "ShowItemTransfer", 0x180515124) uses: looks the
// class up in the item-class registry (qword_185325820 via sub_180468340), then vf+96(priceCtx)
// -> outPrice, vf+200(1) -> class data {+0xA0 ui-name loc key -> outName, +0x90 icon -> outIcon},
// and LOCALIZES outName in place (sub_1803C235C) unless UI flag 0x20 is set.  On a registry
// miss the out-params are left untouched -- pre-initialize them.
//
// THE GUID MUST BE THE CLASS-DEF GUID: S_ItemClass::m_guid (item->m_pClassData->m_guid), the
// registry's key.  C_Item+0x38 is the item's INSTANCE guid -- the registry misses it (2026-07-18
// live test: outputs came back untouched).  "class data" above = vf[25] GetAsPlayerItemClass():
// the S_PlayerItemClass view holding m_iconId (+0x90) / m_uiName (+0xA0).

namespace wh::guimodule {

// Resolve an item class to its (localized) display name, icon name and price.
void GetItemUiPresentation(const CryGUID& itemClass, uint32_t priceContext,
                           CryStringT<char>& outName, CryStringT<char>& outIcon,
                           float& outPrice);

}  // namespace wh::guimodule
