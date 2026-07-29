#pragma once

// -----------------------------------------------
// wh::environmentmodule::IWeatherSystem -- weather-system interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183ED1208: [0] dtor + 20 pure virtuals (all _purecall 0x181D93E5D in the
// interface vtable). Implemented by C_WeatherSystem (vtable 0x183AB8D20). wh:: interfaces
// are NOT interfuscator-shuffled -- slot positions are trustworthy.
// Resolved slots (proven against the C_WeatherSystem implementation + Lua binders):
//   [9]  GetRainIntensity() -> float             impl 0x180535EC0 (Lua "GetRainIntensity")
//   [11] ForceImmediateWeatherUpdate()           impl 0x180552724 (Lua binder sub_180C820A0)
//   [18] AddSpatialWeatherModifier(pos, radius)  impl 0x180A4A028 -- inserts a node into the
//        C_WeatherSystem +0x18F0 spatial-modifier container (pos @node+24/+40, radius @+56);
//        localized weather lever [return type UNVERIFIED, modeled void]
// Remaining slots are unresolved wind/cloud/preset accessors -- decompile the matching
// C_WeatherSystem vtable entry before use.

namespace wh::environmentmodule {

class IWeatherSystem {
public:
    virtual ~IWeatherSystem();                                            // [0]
    virtual void  _vf1() = 0;                                             // [1]  unresolved
    virtual void  _vf2() = 0;                                             // [2]  unresolved
    virtual void  _vf3() = 0;                                             // [3]  unresolved
    virtual void  _vf4() = 0;                                             // [4]  unresolved
    virtual void  _vf5() = 0;                                             // [5]  unresolved
    virtual void  _vf6() = 0;                                             // [6]  unresolved
    virtual void  _vf7() = 0;                                             // [7]  unresolved
    virtual void  _vf8() = 0;                                             // [8]  unresolved
    virtual float GetRainIntensity() = 0;                                 // [9]  0x180535EC0
    virtual void  _vf10() = 0;                                            // [10] unresolved
    virtual void  ForceImmediateWeatherUpdate() = 0;                      // [11] 0x180552724
    virtual void  _vf12() = 0;                                            // [12] unresolved
    virtual void  _vf13() = 0;                                            // [13] unresolved
    virtual void  _vf14() = 0;                                            // [14] unresolved
    virtual void  _vf15() = 0;                                            // [15] unresolved
    virtual void  _vf16() = 0;                                            // [16] unresolved
    virtual void  _vf17() = 0;                                            // [17] unresolved
    virtual void  AddSpatialWeatherModifier(const Vec3& pos, float radius) = 0;   // [18] 0x180A4A028
    virtual void  _vf19() = 0;                                            // [19] unresolved
    virtual void  _vf20() = 0;                                            // [20] unresolved
};
static_assert(sizeof(IWeatherSystem) == 0x08, "IWeatherSystem is a vptr-only interface");

}  // namespace wh::environmentmodule
