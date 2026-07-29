#pragma once
#include "../CryEngine/CryCommon/Cry_Math.h"   // Vec3

// -----------------------------------------------
// wh::playermodule::I_UIFastTravel -- fast-travel UI callback interface.
// KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_UIFastTravel@playermodule@wh@@ (TD 0x184C92888).  Sole implementor found:
// guimodule::C_UIMap (base subobject @+0x18, vtable 0x183ED8F08, 5 slots, no dtor slot).
// C_UIMap::Init registers it with the per-player C_FastTravel via sub_180ED10A8, which
// stores the pointer at C_FastTravel+0x38 -- i.e. this interface IS the "travel executor"
// C_FastTravel::StartFastTravel kicks through vtable slot [0].
//
// [FUNDAMENTAL vs KCD1] Replaces KCD1's four SUIEventSenderBlock signals on C_UIMap
// (+0x20 AnswerRandomEvent / +0x60 ComputePath / +0xA0 Start / +0xE0 Stop) that
// C_FastTravel::BindToUIMap subscribed to; KCD2 wires one typed interface instead.
//
// Slot evidence (C_UIMap impls):
//   [0] 0x182B00C28  asks C_FastTravel to compute a path (sub_182DE27D0) and caches the
//       resulting node-id list + flags on the UIMap (fields +0x5D8/+0x5E0/+0x5F8).
//   [1] 0x182AFEAE0  travel started: sets m_isFastTravelling, audio state
//       "apse_fast_travel_running"=1, starts the "UIMap" fader, full-UI mode on,
//       flash "FastTravelStarted"(destName, bool).
//   [2] 0x182AFEA18  travel finished: mirrors [1], flash "FastTravelFinished".
//   [3] 0x182B02938  per-tick update: flash "SetPlayer"(x, y, headingDeg, mapName),
//       "UpdateFastTravelProgress"(int), "SetTime"(timeOfDay, rpg_params 279/281/282).
//   [4] 0x182AFDE1C  random-event prompt: forwards into the embedded
//       C_UIModalDialogRandomEvent (sub_182B15268)  [KCD1 "AnswerRandomEvent" analogue].
//
// Parameter lists are recovered from the C_UIMap bodies only -- exact machine signatures
// of [0]/[3]/[4] carry extra context arguments; treat them as UNVERIFIED beyond what is
// documented per slot.

namespace wh::playermodule {

class I_UIFastTravel {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIFastTravel;
    // [0] set the fast-travel destination: forwards dest to the C_FastTravel path query
    //     (sub_182DE27D0), which stores it in C_FastTravel.m_destination + fills m_pathNodeIds and
    //     arms the path; returns the travel-gate status (0 = armed OK) [name INFERRED]
    virtual char SetDestination(const Vec3& dest, char a3, char forced) = 0;      // C_UIMap: 0x182B00C28
    // [1] travel started (a2 forwarded to flash "FastTravelStarted" bool arg) [name INFERRED]
    virtual void OnFastTravelStarted(bool a2) = 0;                                // C_UIMap: 0x182AFEAE0
    // [2] travel finished/cancelled [name INFERRED]
    virtual void OnFastTravelFinished() = 0;                                      // C_UIMap: 0x182AFEA18
    // [3] per-tick progress update (pos/dir drive flash "SetPlayer"; remaining ->
    //     "UpdateFastTravelProgress") [name INFERRED; tail args UNVERIFIED]
    virtual void OnFastTravelUpdate(const Vec3* pos, const Vec3* dir, int remaining, ...) = 0;  // C_UIMap: 0x182B02938
    // [4] show the random-event (ambush) prompt via C_UIModalDialogRandomEvent
    //     [name INFERRED; arg roles UNVERIFIED -- text + option/callback context]
    virtual void ShowRandomEventDialog(CryStringT<char>* text, void* a3, void* a4, void* a5) = 0;  // C_UIMap: 0x182AFDE1C
};
static_assert(sizeof(I_UIFastTravel) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::playermodule
