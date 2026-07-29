#pragma once

// -----------------------------------------------
// IActionMapEventListener -- CryAction action-map event sink (KCD2 WHGame.dll 1.5.6, kd7u).
// 2-slot vtable, global namespace (SDK-style).
// -----------------------------------------------
// Verified on two independent classes: wh::entitymodule::C_MissileWeaponPlayerController
// (primary vtable 0x183A80E30: [0] dtor, [1] sub_182AA91C4) and wh::playermodule::C_ActionSets
// (primary vtable 0x183F60D18: [0] dtor 0x182E14244, [1] OnActionMapEvent 0x18085B2B4 --
// handles SActionMapEvent type 3 = action-map enabled/disabled, type 2 = refresh).
// Registered/removed via IActionMapManager vtbl+424 / vtbl+432.  The event parameter is a
// CryEngine SActionMapEvent [layout UNVERIFIED -- kept untyped].

struct IActionMapEventListener {
    virtual ~IActionMapEventListener() = default;   // [0]
    virtual void OnActionMapEvent(void* event) = 0; // [1]
};
