#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_TriggerPointEventImplementation : I_AnimEventImplementation
// -- "triggerpoint" anim event (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08
// (create-site proven: qword_18549D378(8) in ctor sub_18199BCC0 / factory
// sub_18342C804; vptr only).
// -----------------------------------------------
// RTTI TD rva 0x4FF3260; COL rva 0x45C3B98; vtable 0x184010E58, 13 slots.
// Overrides: [1]sub_180EE56EC ("triggerpoint") [2]sub_18343153C [3]nullsub_1
// [10]sub_181A6CB48 [12]sub_18343476C.  Rest inherited.

namespace wh::xgenaimodule {

class C_TriggerPointEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TriggerPointEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE56EC -> "triggerpoint"
    bool  Apply(int param, void* ctx) override;          // [2]  sub_18343153C
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CB48
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343476C
};
static_assert(sizeof(C_TriggerPointEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
