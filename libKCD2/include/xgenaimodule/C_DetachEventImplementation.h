#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DetachEventImplementation : I_AnimEventImplementation --
// "detachobject" anim event (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (alloc 8
// in ctor sub_18199BF10 / factory sub_18342C3E4).
// -----------------------------------------------
// RTTI TD rva 0x4FF2CA0; COL rva 0x45C3AD0; vtable 0x1840111E8, 13 slots.
// Overrides: [1]sub_180EE5A44 ("detachobject") [2]sub_1818645F0 [3]nullsub_1
// [10]sub_181A6D280 [12]sub_18343466C.  Rest inherited.

namespace wh::xgenaimodule {

class C_DetachEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DetachEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE5A44 -> "detachobject"
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1818645F0
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6D280
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343466C
};
static_assert(sizeof(C_DetachEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
