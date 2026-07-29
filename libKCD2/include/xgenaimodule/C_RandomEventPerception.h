#pragma once
#include "I_RandomEventPerception.h"

// -----------------------------------------------
// wh::xgenaimodule::C_RandomEventPerception : I_RandomEventPerception -- concrete
// random-event perception hook (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT proven
// -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4EC04E0; vtable 0x18475CC50 (.rdata tail -- late/registration
// vtable region), 2 slots: [0] sub_1832BC38C, [1] sub_1832BC2AC.

namespace wh::xgenaimodule {

class C_RandomEventPerception : public I_RandomEventPerception {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RandomEventPerception;
    void _vf0() override;   // [0] sub_1832BC38C [U role; dtor placement unverified]
    void _vf1() override;   // [1] sub_1832BC2AC [U role]
};
// members + sizeof unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
