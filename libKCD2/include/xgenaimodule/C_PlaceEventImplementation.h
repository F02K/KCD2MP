#pragma once
#include <cstdint>
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PlaceEventImplementation : I_AnimEventImplementation --
// place-item anim event; the second 16-byte leaf (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x10 (create-site proven: qword_18549D378(16) in ctor sub_1819A1C68 /
// factory sub_18342C678).  +0x08 NOT initialized by the ctor -- purpose
// UNVERIFIED.
// -----------------------------------------------
// RTTI TD rva 0x4FF3190; COL rva 0x45C3C38; vtable 0x1840132D8, 13 slots.
// Family-B dtor: [0]sub_18342FB88.  Overrides: [1]sub_1803A2F00 [2]sub_181086090
// [3]nullsub_1 [4]sub_180C85278 [5]sub_181AAB760 [6]sub_1803A2D88 [7]sub_1810869F4
// [8]nullsub_1 [9]sub_18343149C [10]sub_181A6CE04 [12]sub_18343470C.  Inherited: [11].

namespace wh::xgenaimodule {

class C_PlaceEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlaceEventImplementation;
    ~C_PlaceEventImplementation() override;              // [0]  family-B dtor sub_18342FB88
    void* GetName() override;                            // [1]  sub_1803A2F00
    bool  Apply(int param, void* ctx) override;          // [2]  sub_181086090
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    void* BuildStringA(void* out) override;              // [4]  sub_180C85278
    int*  GetCategory(int* out) override;                // [5]  sub_181AAB760
    void  SerializeHook6() override;                     // [6]  sub_1803A2D88
    void  SerializeHook7() override;                     // [7]  sub_1810869F4
    void  SerializeHook8() override;                     // [8]  nullsub_1
    void* GetDefaultWuid(void* out) override;            // [9]  sub_18343149C
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CE04
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343470C

    uint64_t field_8;   // +0x08  ctor leaves UNinitialized; purpose UNVERIFIED [U]
};
static_assert(sizeof(C_PlaceEventImplementation) == 0x10, "must be 0x10 (alloc 16)");

}  // namespace wh::xgenaimodule
