#pragma once

// -----------------------------------------------
// wh::environmentmodule::C_TimeOfDayProfile -- named TOD profile handle (KCD2 1.5.6, kd7u).
// sizeof 0x10.
// -----------------------------------------------
// Own vtable 0x183B7E820. Ctor sub_1813FF944 copies the profile name (CryString copy
// sub_1804FD208); dtor sub_182AE31D0 frees only the string. The actual time-of-day curve
// payload lives in the weather-profile database row, not here.

namespace wh::environmentmodule {

class C_TimeOfDayProfile {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TimeOfDayProfile;
    virtual ~C_TimeOfDayProfile();   // [0]; further slots not enumerated

    CryStringT<char> m_name;         // +0x08  profile name
};
static_assert(sizeof(C_TimeOfDayProfile) == 0x10, "C_TimeOfDayProfile must be 0x10");

}  // namespace wh::environmentmodule
