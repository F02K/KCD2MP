#pragma once

#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// Input value types — KCD-accurate, self-contained (Offsets namespace)
// -----------------------------------------------
// The CryEngine SDK copies of these live (global) in CryCommon/IInput.h, but
// KCD's SInputEvent layout DIFFERS from the SDK (it dropped the SDK's
// `wchar_t inputChar` slot, shifting every field after `state` down 8 bytes).
// We keep our own in `Offsets::` so it can't collide with the SDK struct.

namespace Offsets {

enum EDeviceId : int {          // COPIED-FROM-SDK (IInput.h)
    eDI_Keyboard    = 0,
    eDI_Mouse       = 1,
    eDI_XI          = 2,        // XInput pad
    eDI_Headmounted = 3,
    eDI_Unknown     = 0xff,
};

enum EInputState : int {        // COPIED-FROM-SDK; low bits match EActionActivationMode
    eIS_Unknown  = 0,
    eIS_Pressed  = 1 << 0,
    eIS_Released = 1 << 1,
    eIS_Down     = 1 << 2,
    eIS_Changed  = 1 << 3,
    eIS_UI       = 1 << 4,
};

// EKeyId bases (COPIED-FROM-SDK). The sys base (0x1000) and the xinput
// connect/disconnect ids (0x21E/0x21F) are VERIFIED against the binary's
// keyId filter in PostInputEvent.
enum EKeyId : uint32_t {
    eKI_Escape = 0, eKI_1, eKI_2, eKI_3, eKI_4, eKI_5, eKI_6, eKI_7, eKI_8, eKI_9, eKI_0,
    eKI_Minus, eKI_Equals, eKI_Backspace, eKI_Tab,
    eKI_Q, eKI_W, eKI_E, eKI_R, eKI_T, eKI_Y, eKI_U, eKI_I, eKI_O, eKI_P,
    eKI_LBracket, eKI_RBracket, eKI_Enter, eKI_LCtrl,
    eKI_A, eKI_S, eKI_D, eKI_F, eKI_G, eKI_H, eKI_J, eKI_K, eKI_L,
    eKI_Semicolon, eKI_Apostrophe, eKI_Tilde, eKI_LShift, eKI_Backslash,
    eKI_Z, eKI_X, eKI_C, eKI_V, eKI_B, eKI_N, eKI_M,
    eKI_Comma, eKI_Period, eKI_Slash, eKI_RShift,
    eKI_NP_Multiply, eKI_LAlt, eKI_Space, eKI_CapsLock,
    eKI_F1, eKI_F2, eKI_F3, eKI_F4, eKI_F5, eKI_F6, eKI_F7, eKI_F8, eKI_F9, eKI_F10,
    eKI_NumLock, eKI_ScrollLock,
    eKI_NP_7, eKI_NP_8, eKI_NP_9, eKI_NP_Substract,
    eKI_NP_4, eKI_NP_5, eKI_NP_6, eKI_NP_Add,
    eKI_NP_1, eKI_NP_2, eKI_NP_3, eKI_NP_0,
    eKI_F11, eKI_F12, eKI_F13, eKI_F14, eKI_F15,
    eKI_Colon, eKI_Underline, eKI_NP_Enter, eKI_RCtrl, eKI_NP_Period, eKI_NP_Divide,
    eKI_Print, eKI_RAlt, eKI_Pause, eKI_Home, eKI_Up, eKI_PgUp,
    eKI_Left, eKI_Right, eKI_End, eKI_Down, eKI_PgDn, eKI_Insert, eKI_Delete,
    eKI_LWin, eKI_RWin, eKI_Apps, eKI_OEM_102,

    eKI_Mouse1 = 256, eKI_Mouse2, eKI_Mouse3, eKI_Mouse4, eKI_Mouse5, eKI_Mouse6, eKI_Mouse7, eKI_Mouse8,
    eKI_MouseWheelUp, eKI_MouseWheelDown, eKI_MouseX, eKI_MouseY, eKI_MouseZ, eKI_MouseLast,

    eKI_XI_DPadUp = 512, eKI_XI_DPadDown, eKI_XI_DPadLeft, eKI_XI_DPadRight,
    eKI_XI_Start, eKI_XI_Back, eKI_XI_ThumbL, eKI_XI_ThumbR,
    eKI_XI_ShoulderL, eKI_XI_ShoulderR, eKI_XI_A, eKI_XI_B, eKI_XI_X, eKI_XI_Y,
    eKI_XI_TriggerL, eKI_XI_TriggerR, eKI_XI_ThumbLX, eKI_XI_ThumbLY,
    eKI_XI_ThumbLUp, eKI_XI_ThumbLDown, eKI_XI_ThumbLLeft, eKI_XI_ThumbLRight,
    eKI_XI_ThumbRX, eKI_XI_ThumbRY,
    eKI_XI_ThumbRUp, eKI_XI_ThumbRDown, eKI_XI_ThumbRLeft, eKI_XI_ThumbRRight,
    eKI_XI_TriggerLBtn, eKI_XI_TriggerRBtn,
    eKI_XI_Connect    = 0x21E,   // VERIFIED anchor (binary sys-key filter)
    eKI_XI_Disconnect = 0x21F,   // VERIFIED anchor

    eKI_SYS_Commit           = 0x1000,  // VERIFIED anchor (KI_SYS_BASE)
    eKI_SYS_ConnectDevice    = 0x1001,
    eKI_SYS_DisconnectDevice = 0x1002,

    eKI_Unknown = 0xffffffff,
};

struct SInputSymbol;   // opaque

// KCD SInputEvent. Offsets 0x00/0x04/0x08/0x10/0x14/0x18 VERIFIED
// (C_UIActionHintManager::OnInputEvent, sub_1803E6900). pSymbol/deviceIndex/
// size are INFERRED from SDK order minus the removed inputChar slot.
struct SInputEvent {
    EDeviceId     deviceId;     // 0x00  VERIFIED
    EInputState   state;        // 0x04  VERIFIED
    const char*   keyName;      // 0x08  VERIFIED (TKeyName == const char*)
    EKeyId        keyId;        // 0x10  VERIFIED
    int           modifiers;    // 0x14  VERIFIED
    float         value;        // 0x18  VERIFIED
    int           _pad1C;       // 0x1C  (align)
    SInputSymbol* pSymbol;      // 0x20  INFERRED
    uint8_t       deviceIndex;  // 0x28  INFERRED (controller index)
    uint8_t       _pad29[7];    // 0x29
};
static_assert(offsetof(SInputEvent, deviceId)  == 0x00, "");
static_assert(offsetof(SInputEvent, state)     == 0x04, "");
static_assert(offsetof(SInputEvent, keyName)   == 0x08, "");
static_assert(offsetof(SInputEvent, keyId)     == 0x10, "");
static_assert(offsetof(SInputEvent, modifiers) == 0x14, "");
static_assert(offsetof(SInputEvent, value)     == 0x18, "");

}  // namespace Offsets
