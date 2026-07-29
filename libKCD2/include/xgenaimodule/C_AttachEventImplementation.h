#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AttachEventImplementation : I_AnimEventImplementation --
// attach-object anim event (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (alloc 8
// in ctor sub_18199C038 / factory sub_18342C2DC).
// -----------------------------------------------
// RTTI TD rva 0x4FF2D90; COL rva 0x45C39E0; vtable 0x184011178, 13 slots.
// Overrides: [1]sub_180EE5AB8 [2]sub_18179B588 [3]nullsub_1 [10]sub_181A6D654
// [12]sub_18343462C.  Rest inherited.

namespace wh::xgenaimodule {

class C_AttachEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AttachEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE5AB8
    bool  Apply(int param, void* ctx) override;          // [2]  sub_18179B588
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180D1E8B4 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6D654
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343462C
};
static_assert(sizeof(C_AttachEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
