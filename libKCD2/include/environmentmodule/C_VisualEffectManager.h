#pragma once
#include <list>

// -----------------------------------------------
// wh::environmentmodule::C_VisualEffectManager -- live visual-effect registry
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// Own vtable 0x183AB8EF8; ctor sub_180E3C34C. Owned by C_EnvironmentModule @+0x90. The
// +0x08 member is a self-linked sentinel node + zero count (sub_181AB55C0) -- the exact
// MSVC std::list {head, size} shape [container type INFERRED from that shape].

namespace wh::environmentmodule {

class I_VisualEffect;

class C_VisualEffectManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VisualEffectManager;
    virtual ~C_VisualEffectManager();   // [0]; add/remove slots not enumerated

    std::list<I_VisualEffect*> m_effects;   // +0x08  live effects [std::list INFERRED]
};
static_assert(sizeof(C_VisualEffectManager) == 0x18, "C_VisualEffectManager must be 0x18 (alloc 24)");

}  // namespace wh::environmentmodule
