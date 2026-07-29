#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DespawnItemEventImplementation : I_AnimEventImplementation
// -- despawn-item anim event (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (alloc 8
// in ctor sub_18199BFA4 / factory sub_18342C360).
// -----------------------------------------------
// RTTI TD rva 0x4FF2D40; COL rva 0x45C3B20; vtable 0x184011258, 13 slots.
// Overrides: [1]sub_1808FA748 [2]sub_1808FA160 [3]nullsub_1 [5]sub_181AA7170
// (*out=3) [6]sub_1808FA618 [7]sub_181087D98 [8]nullsub_1 [10]sub_181A6D29C
// [12]sub_18343464C.  Inherited: [0][4][9][11].

namespace wh::xgenaimodule {

class C_DespawnItemEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DespawnItemEventImplementation;
    void* GetName() override;                            // [1]  sub_1808FA748
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1808FA160
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    int*  GetCategory(int* out) override;                // [5]  sub_181AA7170 -> *out=3
    void  SerializeHook6() override;                     // [6]  sub_1808FA618
    void  SerializeHook7() override;                     // [7]  sub_181087D98
    void  SerializeHook8() override;                     // [8]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6D29C
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343464C
};
static_assert(sizeof(C_DespawnItemEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
