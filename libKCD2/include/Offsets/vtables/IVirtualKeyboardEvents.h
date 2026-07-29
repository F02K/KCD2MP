#pragma once

// -----------------------------------------------
// IVirtualKeyboardEvents -- CryEngine on-screen (virtual) keyboard show/hide sink.
// -----------------------------------------------
// SDK: CryCommon/IFlashPlayer.h. Global-namespace SDK struct; modelled here in
// Offsets:: with the other flash vtable replicas.
//
// CFlashUIElement implements it as its fourth base (+0x18). Sub-vtable @ 0x184049B80,
// 3 slots: dtor-thunk [0], then two events (0x1835B1B18 / 0x1835B1C34) that invoke
// "cry_onVirtualKeyboard" on the element's flash player (element +0x58). The two
// bodies differ only by the leading bool (0 vs 1) and the second passing a text arg,
// so they read as keyboard-hidden vs keyboard-shown.
//
// [METHOD NAMES UNVERIFIED] coined from the cry_onVirtualKeyboard payload; slot roles
// are certain, exact SDK names are not.

namespace Offsets {

struct IVirtualKeyboardEvents {
protected:
    virtual ~IVirtualKeyboardEvents() {}   // [0] this-adjustor dtor thunk 0x18213D988
public:
    // [1] 0x1835B1B18  cry_onVirtualKeyboard(false)
    virtual void OnVirtualKeyboardHidden() = 0;
    // [2] 0x1835B1C34  cry_onVirtualKeyboard(true, text)
    virtual void OnVirtualKeyboardShown(const char* pText) = 0;
};

}  // namespace Offsets
