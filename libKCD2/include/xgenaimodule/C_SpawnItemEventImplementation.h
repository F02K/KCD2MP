#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SpawnItemEventImplementation : I_AnimEventImplementation --
// spawn-item anim event; the most-overriding leaf (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08 (create-site proven: alloc 8 in ctor sub_18199BDE8 / factory
// sub_18342C6FC).
// -----------------------------------------------
// RTTI TD rva 0x4FF3030; COL rva 0x45C3B70; vtable 0x184011018, 13 slots.
// Overrides: [1]sub_1808FA610 (&unk_18533ADA0) [2]sub_1808FA2C4
// [3]sub_18138E644 (REAL 2nd pass) [4]sub_180EE5C10 [5]sub_181AA7170 (*out=3)
// [6]sub_1808FA4D0 [7]sub_181087C98 [8]sub_18342FE30 [10]sub_181A6CBB8
// [12]sub_18343472C.  Inherited: [0][9][11].

namespace wh::xgenaimodule {

class C_SpawnItemEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SpawnItemEventImplementation;
    void* GetName() override;                            // [1]  sub_1808FA610
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1808FA2C4
    void  ApplySecondary(int param, void* ctx) override; // [3]  sub_18138E644 (real)
    void* BuildStringA(void* out) override;              // [4]  sub_180EE5C10
    int*  GetCategory(int* out) override;                // [5]  sub_181AA7170 -> *out=3
    void  SerializeHook6() override;                     // [6]  sub_1808FA4D0
    void  SerializeHook7() override;                     // [7]  sub_181087C98
    void  SerializeHook8() override;                     // [8]  sub_18342FE30
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CBB8
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343472C
};
static_assert(sizeof(C_SpawnItemEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
