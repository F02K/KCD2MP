#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SpawnedItemsCleanupEventImplemenation :
// I_AnimEventImplementation -- cleanup of spawned items; class name carries the
// BINARY'S OWN TYPO "Implemenation" (verbatim from the TypeDescriptor).  KCD2
// WHGame.dll 1.5.6, kd7u.  sizeof 0x08 (alloc 8 in ctor sub_18199BD54 / factory
// sub_18342C780).
// -----------------------------------------------
// RTTI TD rva 0x4FF3080; COL rva 0x45C3B48; vtable 0x184010FA8, 13 slots.
// Overrides: [1]sub_180EE5864 [2]sub_1819ED0A8 [3]nullsub_1 [5]sub_181AA7170
// (*out=3) [6]sub_1808FACD8 [7]sub_18342FD94 [8]nullsub_1 [10]sub_181A6CB9C
// [12]sub_18343474C.  Inherited: [0][4][9][11].

namespace wh::xgenaimodule {

class C_SpawnedItemsCleanupEventImplemenation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SpawnedItemsCleanupEventImplemenation;
    void* GetName() override;                            // [1]  sub_180EE5864
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1819ED0A8
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    int*  GetCategory(int* out) override;                // [5]  sub_181AA7170 -> *out=3
    void  SerializeHook6() override;                     // [6]  sub_1808FACD8
    void  SerializeHook7() override;                     // [7]  sub_18342FD94
    void  SerializeHook8() override;                     // [8]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CB9C
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343474C
};
static_assert(sizeof(C_SpawnedItemsCleanupEventImplemenation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
