#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::game::I_SkipEventProvider -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 2 slots.
// -----------------------------------------------
// RTTI .?AVI_SkipEventProvider@game@wh@@ (TD 0x184CB05A0). Cutscene skip-input hook:
// base of C_TrackViewBase (@+0x60, subobject vtable 0x183B2E1D8: [0] dtor-thunk
// 0x18213A390, [1] 0x182B36D6C). Method role/signature UNVERIFIED (coined).

namespace wh::game {

class I_SkipEventProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SkipEventProvider;
    virtual ~I_SkipEventProvider();   // [0]
    virtual void _vf1() = 0;          // [1] 0x182B36D6C (C_TrackViewBase impl) [role UNVERIFIED — OnSkipRequested?]
};
static_assert(sizeof(I_SkipEventProvider) == 8);

}  // namespace wh::game
