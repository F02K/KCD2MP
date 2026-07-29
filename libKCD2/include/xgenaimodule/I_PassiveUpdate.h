#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_PassiveUpdate -- passive-update tick interface (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4EFB640; standalone vtable rva 0x3FD6FF0, 4 slots: [0]
// CONCRETE scalar deleting dtor sub_183201C0C, [1][2][3] _purecall (one of
// them is the passive-update tick -- which one [U]).  Confirmed derived (via
// vptr-restoring dtors): C_TimeWaiting (TD 0x4FF5B90) and a multi-MB grid-like
// class (dtors sub_183201540/sub_1832015B0).

namespace wh::xgenaimodule {

class I_PassiveUpdate {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_PassiveUpdate;
    virtual ~I_PassiveUpdate();  // [0] concrete deleting dtor sub_183201C0C
    virtual void _vf1() = 0;     // [1] [U role]
    virtual void _vf2() = 0;     // [2] [U role]
    virtual void _vf3() = 0;     // [3] [U role]
};
static_assert(sizeof(I_PassiveUpdate) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
