#pragma once
#include <cstdint>
#include "guimodule/C_UIMenuBase.h"
#include "playermodule/I_UIKeybinds.h"

// -----------------------------------------------
// wh::guimodule::C_UIKeybinds -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_UIKeybinds@guimodule@wh@@ (TD 0x184CD99F8).
// Bases: C_UIMenuBase @+0x00 (COL 0x4135400 -- so it binds the shared "Menu" movie,
// GetElementName [11] = 0x181A8F230 "Menu"), IUIElementEventListener @+0x10
// (COL 0x4135428), playermodule::I_UIKeybinds @+0x58 (COL 0x4135450).
// vtables: primary 0x183A40AE0 (14), listener 0x183A40B58 (8), keybinds 0x183A40BA0
// (2). ctor sub_1806003C8 -- NO own data members.
//
// NOT a module screen (no rttr constructor_wrapper / _Ref_count TD): lives EMBEDDED
// in C_UISettings at +0x338 (C_UISettings ctor sub_180600544 @0x18060064c; next member
// at settings+0x398 pins sizeof = 0x60), i.e. at C_UIMenu +0x510.
// Init [1] sub_1806127D4: base Init, publishes the I_UIKeybinds subobject into the
// playermodule object (*(*(ctx+0x128)+0x40)+0x1E8 = this+0x58; Deinit [2]
// sub_182BA6F48 clears it) and registers the inbound flash dispatcher
// "OnKeybindsChangeInput" -> {this, sub_182BA7FC4}. Outbound: "KeybindsSetConflict" /
// "KeybindsChangeInput" (interface impls).
//
// [MODERATE vs KCD1] KCD1 C_UIKeybinds was a CUIManager event system; KCD2 demotes it
// to a value member of the settings page behind a playermodule interface.

namespace wh::guimodule {

class C_UIKeybinds : public C_UIMenuBase, public wh::playermodule::I_UIKeybinds {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIKeybinds;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x1806127D4
    void Deinit() override;                     // [2] 0x182BA6F48
    // I_UIKeybinds overrides.
    void SetConflict(int a2, const CryStringT<char>& action, int a4) override;    // [K0] 0x182BA97C0
    void ChangeInput(int a2, const CryStringT<char>& action, int a4,
                     const CryStringT<char>& key) override;                       // [K1] 0x182BA9B50
};
static_assert(sizeof(C_UIKeybinds) == 0x60, "C_UIKeybinds must be 0x60");

}  // namespace wh::guimodule
