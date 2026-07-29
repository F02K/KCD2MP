#pragma once

// -----------------------------------------------
// wh::environmentmodule::I_VisualEffect -- visual-effect interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183A70DA0. Base of C_VisualEffect and the Light/Material effect leaves;
// live instances are tracked by C_VisualEffectManager. Slot set not enumerated.

namespace wh::environmentmodule {

class I_VisualEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_VisualEffect;
    virtual ~I_VisualEffect();   // [0]; further slots not enumerated
};
static_assert(sizeof(I_VisualEffect) == 0x08, "I_VisualEffect is a vptr-only interface");

}  // namespace wh::environmentmodule
