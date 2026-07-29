#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/SInputEvent.h"   // Offsets::SInputEvent (the OnInputEvent payload)

// -----------------------------------------------
// Offsets::IInputEventListener -- raw input listener (register via IInput::AddEventListener).
// KCD2 WHGame.dll 1.5.6 (kd7u).  4 slots, NO virtual dtor.
// -----------------------------------------------
// CryEngine SDK CryCommon/IInput.h interface, but the vtable is interfuscator-SHUFFLED, so the
// SDK declaration order {~dtor, OnInputEvent, OnInputEventUI, GetPriority} does NOT match the
// binary -- and there is no dtor slot at all.  Confirmed against two independent impls:
//   C_Game subobject (@+0x18, vtable 0x1840955A0):  [0] 0x1809E4E84  [1] 0x181500E50 (reads
//     C_Game+0x70)  [2] ret 0  [3] 0x180838AE0 ret false.
//   C_UIActionHintManager subobject (vtable 0x183C124F0): [0] 0x1811671E4  [1] 0x180838AE0
//     (default)  [2] 0x18066CD10 (default)  [3] 0x18041A6A0 (overridden -> ret 1).
// OnInputEvent@[0] is VERIFIED load-bearing: the AddEventListener broadcaster sub_18085C13C
// iterates the listener vector @CDXInput+0x80 and calls listener->vtable[0](event); its body
// reads SInputEvent.keyId(+0x10)/deviceId(+0)/keyName(+8).  The broadcast never sorts by
// priority and never deletes the listener, so [1..3] are not load-bearing for an
// AddEventListener listener.
//
// [FUNDAMENTAL vs KCD1] OnInputEvent moved from slot [1] (KCD1 was dtor-first) to slot [0];
// a KCD2 listener CANNOT reuse the KCD1 layout or the game calls the wrong slot.
//
// OnInputEvent's `event` is a `const SInputEvent&` -- VERIFIED (the broadcaster reads
// keyId/state/deviceId).  [1]/[3] keep `const void*`: no known impl reads their arg, so their
// payload type is unverified (the SDK types UI as SInputEvent, but KCD2 leaves it unconfirmed).

namespace Offsets {

struct IInputEventListener {
    virtual bool OnInputEvent(const SInputEvent& event) = 0;   // [0] VERIFIED  return true = consume
    virtual bool OnInputEventUI(const void* event) = 0;        // [1] default false
    virtual int  GetPriority() const = 0;                      // [2] default 0
    virtual bool _vf3(const void* event) = 0;                  // [3] ret false (OnInputEventPostProcess? tentative)
};

}  // namespace Offsets
