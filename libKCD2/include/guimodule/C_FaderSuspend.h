#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include "guimodule/I_FaderListener.h"

// -----------------------------------------------
// wh::guimodule::C_FaderSuspend -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60
// (ALLOC-PROVEN: creator sub_180C09154 = C_FaderController vtable[0] Show factory
// allocates 96 bytes; also the fade-op object held by C_UIGameOver::m_pFaderHandle).
// -----------------------------------------------
// RTTI TD 0x184CB0B30; COL 0x4139140; vtable 0x183A85A28. Base: I_FaderListener@+0x00.
// RAII fade-suspend guard: ctors sub_180C08FF8 / sub_180C09154 resolve the fader
// (sub_1806D6B2C) and register self via fader vf[24]/vf[32].

namespace wh::guimodule {

class C_FaderSuspend : public I_FaderListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FaderSuspend;
    // I_FaderListener impls (vtable 0x183A85A28; roles UNVERIFIED).
    void OnFadeIn() override;    // [1] 0x180C08FC4
    void OnFadeOut() override;   // [2] 0x180C08F10
    void OnFade() override;      // [3] 0x180C08EA4

    std::weak_ptr<I_FaderController> m_fader;  // +0x08  weak ref to owning fader controller (raw ptr +0x08, ctrl-block +0x10); lock() via sub_1806D6B2C
    CryStringT<char>  m_name;       // +0x18  [role UNVERIFIED]
    std::function<void()> m_onReleased;  // +0x20 (0x40)  callback; on suspend release it is invoked then destroyed (sub_1808BB150), copy-assigned by slots 2/3 (sub_1808E71DC)
};
static_assert(sizeof(C_FaderSuspend) == 0x60, "C_FaderSuspend 0x60 (alloc-proven: creator sub_180C09154 allocates 96 bytes)");

}  // namespace wh::guimodule
