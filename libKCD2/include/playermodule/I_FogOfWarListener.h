#pragma once

// -----------------------------------------------
// wh::playermodule::I_FogOfWarListener -- fog-of-war reveal listener.
// KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_FogOfWarListener@playermodule@wh@@ (TD 0x184C92938).  Implementor analyzed:
// guimodule::C_UIMapCloudAtlas (single base @+0x00, vtable 0x183B8EED0 = 4 slots).
// The fog-of-war producer side lives in playermodule (C_FogOfWar<3,4> / I_FogOfWar /
// C_NullFogOfWar TDs); the listener registration site was not traced this wave.
//
// Slot roles are read from the C_UIMapCloudAtlas overrides only -- all names INFERRED:
//   [0] 0x182AFD97C  scalar deleting dtor
//   [1] 0x180B57E5C  (int a2, const CryStringT& levelName, int a4, int a5) -> find the
//       per-level S_Cloud and forward (sub_180B5803C) = fog revealed / cloud update
//   [2] 0x182AFFC60  (?, a3) on the CURRENT level (name via sub_181460EA8)
//   [3] 0x180C3D6E4  (?, const CryStringT& levelName) -> release the level's S_Cloud
//       (sub_180D0EB90) = level fog reset/unload

namespace wh::playermodule {

class I_FogOfWarListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_FogOfWarListener;
    virtual ~I_FogOfWarListener() = default;                                                // [0]
    // fog revealed on a level; forwards into the level's cloud grid [names INFERRED]
    virtual void OnFogOfWarChanged(int a2, const CryStringT<char>& levelName, int a4, int a5) = 0;  // [1]
    virtual void OnCurrentLevelFogChanged(void* a2, void* a3) = 0;                          // [2] role UNVERIFIED
    virtual void OnLevelFogReset(void* a2, const CryStringT<char>& levelName) = 0;          // [3]
};
static_assert(sizeof(I_FogOfWarListener) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::playermodule
