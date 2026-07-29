#pragma once

// -----------------------------------------------
// wh::xgenaimodule::XGenPublic::I_StealthWatcher -- public stealth-context query
// interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI TD rva 0x4F5B300 (.?AVI_StealthWatcher@XGenPublic@xgenaimodule@wh@@); own
// vtable 0x183FECEB8, 3 slots: [0] sub_1832EE128, [1] _purecall, [2] _purecall.

namespace wh::xgenaimodule::XGenPublic {

class I_StealthWatcher {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_StealthWatcher;
    virtual ~I_StealthWatcher();   // [0] sub_1832EE128 [U -- dtor placement unverified]
    virtual void _vf1() = 0;       // [1] _purecall here [U role]
    virtual void _vf2() = 0;       // [2] _purecall here; impl = the trespass/stealth-context check [U role]
};
static_assert(sizeof(I_StealthWatcher) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule::XGenPublic
