#pragma once
#include "../framework/S_ResourceHandle.h"
#include "../Offsets/vtables/IRenderNodeInstanceStreamingCallback.h"

// -----------------------------------------------
// wh::environmentmodule::C_StreamingHelper -- render-node streaming callback helper
// (KCD2 1.5.6, kd7u).  sizeof 0x28 (alloc 40).
// -----------------------------------------------
// Own vtable 0x183AB8EE0; ctor sub_180E3C2F8. IRenderNodeInstanceStreamingCallback impl that
// registers itself with the 3D engine (qword_18492D808 vfunc +2056). Owned by
// C_EnvironmentModule @+0xA0. Carries two unbound 0x10 resource handles (&unk_185665D78).

namespace wh::environmentmodule {

class C_StreamingHelper : public Offsets::IRenderNodeInstanceStreamingCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StreamingHelper;
    framework::S_ResourceHandle m_handle08;   // +0x08  (&unk_185665D78, -1/-1) [target UNVERIFIED]
    framework::S_ResourceHandle m_handle18;   // +0x18  (&unk_185665D78, -1/-1) [target UNVERIFIED]
};
static_assert(sizeof(C_StreamingHelper) == 0x28, "C_StreamingHelper must be 0x28 (alloc 40)");

}  // namespace wh::environmentmodule
