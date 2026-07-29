#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::C_UIDragAndDropHelper -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_UIDragAndDropHelper@guimodule@wh@@ (TD 0x184C9ECB0). Single COL, own
// vtable 0x183D156C0 (slot roles NOT dumped this wave). NOT a C_UIBase -- standalone
// drag-and-drop coordinator embedded BY VALUE in C_UIApseInventory @+0x9B0 (inline ctor
// 0x180bc8bda; size pinned by next embed C_UIApseQAMInteraction @+0xA30).
//
// OPAQUE SHELL beyond the ctor-proven anchors: only the vfptr and the CryStringT @+0x10
// are proven; everything else UNVERIFIED (G4a scoped it as a helper, not a deliverable).

namespace wh::guimodule {

class C_UIDragAndDropHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIDragAndDropHelper;
    virtual ~C_UIDragAndDropHelper();   // [0] vtable 0x183D156C0; further slots UNVERIFIED

    uint64_t         m_unk08;        // +0x08  [role UNVERIFIED]
    CryStringT<char> m_str10;        // +0x10  ctor empty (dragged-item name? UNVERIFIED)
    uint8_t          _opaque18[0x68]; // +0x18..+0x80  UNVERIFIED
};
static_assert(sizeof(C_UIDragAndDropHelper) == 0x80, "C_UIDragAndDropHelper must be 0x80");

}  // namespace wh::guimodule
