#pragma once

// -----------------------------------------------
// wh::entitymodule::I_ActorSpeakCallback -- actor speak-state notification sink
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface, 4 slots.
// -----------------------------------------------
// Primary base of dialogmodule::C_DialogInstance (mdisp 0, shares primary vtable
// 0x183E63A40, 4 slots). Actors notify the live conversation object when they start/stop
// speaking. Slot roles observed on the C_DialogInstance impl:
//   [0] scalar-deleting dtor (sub_181534F6C)
//   [1] role UNVERIFIED
//   [2] sub_180956CC8  marks the speak-record byte (full+600 vs full+584)
//   [3] sub_180689268  delegates to [2]
// Slot names/signatures UNVERIFIED -- mirrored for layout/slot count only.

namespace wh::entitymodule {

class I_ActorSpeakCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ActorSpeakCallback;
    virtual ~I_ActorSpeakCallback();   // [0]
    virtual void _vf1() = 0;           // [1] role UNVERIFIED
    virtual void _vf2() = 0;           // [2] marks speak-record (start/stop speaking)
    virtual void _vf3() = 0;           // [3] delegates to [2]
};
static_assert(sizeof(I_ActorSpeakCallback) == 0x08);

}  // namespace wh::entitymodule
