#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"

// -----------------------------------------------
// wh::musicmodule::C_MusicModule -- module root (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// NEW module in KCD2. RTTI .?AVC_MusicModule@musicmodule@wh@@  ctor sub_18120FF38
// (alloc 0x18 == sizeof, module bootstrap sub_180C9DA44, registry slot [33] @+0x108).
// The ctor also drains the pending-callback intrusive list qword_185595128 (node vf[1] invoked
// with the audio service obtained via registry+0x08 -> vf+0xB0 -> vf+0x40), then clears it.
//
// C_MusicManager (0x30 == alloc PROVEN, ctor sub_181210008): publishes itself to global
// qword_185490B50; own +0x08 -> heap 0x58 sub-object (ctor sub_181210098); +0x10..+0x28
// ctor-zeroed. Layout beyond that unresolved.

namespace wh::musicmodule {

class C_MusicManager;   // 0x30

class C_MusicModule : public wh::framework::C_BaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MusicModule;
    C_MusicManager* m_pMusicManager;   // +0x10  (global mirror qword_185490B50)
};
static_assert(sizeof(C_MusicModule) == 0x18, "C_MusicModule must be 0x18");

}  // namespace wh::musicmodule
