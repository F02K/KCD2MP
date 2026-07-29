#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_CrimeResolver -- crime-resolution interface (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (modeled polymorphic; carries the
// implementor's primary vptr @+0).
// -----------------------------------------------
// RTTI TD rva 0x4F48458.  No COL/vtable of its own (never most-derived).  Slot
// count (11) read from the sole implementor's primary vtable, C_CrimeResolver
// (0x183ABE9B8).  NOTE: the crime-resolution entry point Resolve (sub_1832A0B08)
// is NON-virtual -- it is NOT one of these slots.  Interface/implementor slot
// split modeled all-on-interface [U].

namespace wh::xgenaimodule {

class I_CrimeResolver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CrimeResolver;
    virtual ~I_CrimeResolver();   // [0] impl deleting dtor sub_1832A0A48
    virtual void _vf1() = 0;      // [1]  impl sub_180416300 [U role]
    virtual void _vf2() = 0;      // [2]  impl sub_18093A6D8 [U role]
    virtual bool _vf3() = 0;      // [3]  impl sub_18082DBF0 -- IsResolving/IsEmpty-style check [U role+signature]
    virtual void* _vf4() = 0;     // [4]  impl sub_181A7FAD0 (the shared ownership-getter body) [U role]
    virtual void _vf5() = 0;      // [5]  impl sub_18194D94C [U role]
    virtual void _vf6() = 0;      // [6]  impl sub_182081B00 [U role]
    virtual void _vf7() = 0;      // [7]  impl sub_1805BA114 [U role]
    virtual void _vf8() = 0;      // [8]  impl sub_1832A2360 -- subscribe (crime-resolution subscription) [U role]
    virtual void _vf9() = 0;      // [9]  impl sub_1832A24BC [U role]
    virtual void _vf10() = 0;     // [10] impl sub_1832A26B8 [U role]
};
static_assert(sizeof(I_CrimeResolver) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
