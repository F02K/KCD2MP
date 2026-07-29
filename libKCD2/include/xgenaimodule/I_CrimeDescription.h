#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_CrimeDescription -- base of the crime-description records
// produced by the crime collector and consumed by C_CrimeResolver (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (modeled polymorphic).
// -----------------------------------------------
// RTTI TD rva 0x4F46F28 (.?AUI_CrimeDescription@... -- struct in source).  No
// COL/vtable of its own (never most-derived).  Slot count (5) read from the four
// S_*Description implementor vtables (all 5-slot, shared bodies [1] sub_18039956C
// / [2] sub_1808D2480 across siblings => slots belong to this interface).  The
// factory sub_1832A4400 returns these via std::shared_ptr<I_CrimeDescription>.
// Interface/implementor slot split modeled all-on-interface [U]; slot roles
// UNRESOLVED.

namespace wh::xgenaimodule {

class I_CrimeDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CrimeDescription;
    virtual ~I_CrimeDescription();   // [0] per-class deleting dtor [U -- dtor placement unverified]
    virtual void _vf1() = 0;         // [1] [U role]
    virtual void _vf2() = 0;         // [2] [U role]
    virtual void _vf3() = 0;         // [3] [U role]
    virtual void _vf4() = 0;         // [4] [U role]
};
static_assert(sizeof(I_CrimeDescription) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
