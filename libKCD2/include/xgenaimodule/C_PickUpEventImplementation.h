#pragma once
#include <cstdint>
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PickUpEventImplementation : I_AnimEventImplementation --
// pick-up anim event; one of the two 16-byte leaves (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x10 (create-site proven: qword_18549D378(16) in ctor
// sub_1819A1CFC / factory sub_18342C5F4).  +0x08 is NOT initialized by the ctor
// and never observed read/written by the audited virtuals -- purpose UNVERIFIED.
// -----------------------------------------------
// RTTI TD rva 0x4FF3210; COL rva 0x45C3C10; vtable 0x184013268, 13 slots.
// Family-B dtor: [0]sub_18342FB88.  Overrides: [1]sub_1803A2D80 (&qword_18533AD90)
// [2]sub_1808CE498 [3]nullsub_1 [4]sub_180C8525C [5]sub_181AAB760 [6]sub_1803A2C18
// [7]sub_181087874 [8]nullsub_1 [9]sub_183431420 [10]sub_181A6CE20
// [12]sub_1834346EC.  Inherited: [11].

namespace wh::xgenaimodule {

class C_PickUpEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickUpEventImplementation;
    ~C_PickUpEventImplementation() override;             // [0]  family-B dtor sub_18342FB88
    void* GetName() override;                            // [1]  sub_1803A2D80
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1808CE498
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    void* BuildStringA(void* out) override;              // [4]  sub_180C8525C
    int*  GetCategory(int* out) override;                // [5]  sub_181AAB760
    void  SerializeHook6() override;                     // [6]  sub_1803A2C18
    void  SerializeHook7() override;                     // [7]  sub_181087874
    void  SerializeHook8() override;                     // [8]  nullsub_1
    void* GetDefaultWuid(void* out) override;            // [9]  sub_183431420
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CE20
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_1834346EC

    uint64_t field_8;   // +0x08  ctor leaves UNinitialized; purpose UNVERIFIED [U]
};
static_assert(sizeof(C_PickUpEventImplementation) == 0x10, "must be 0x10 (alloc 16)");

}  // namespace wh::xgenaimodule
