#pragma once

// -----------------------------------------------
// wh::framework::I_BindableObject -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Interface for objects that bind into the WUID registry. Primary base of C_Item (@+0x00).
// Pure interface, no data (the next base sits at +0x08). The KCD2 vtable slots were not fully
// re-verified; only the vtable pointer is load-bearing for the layout.

namespace wh::framework {

class I_BindableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_BindableObject;
    virtual ~I_BindableObject() = default;   // [0]
};

}  // namespace wh::framework
