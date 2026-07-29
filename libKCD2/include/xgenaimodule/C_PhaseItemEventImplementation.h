#pragma once
#include "C_PhaseItemEventImplementationBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PhaseItemEventImplementation :
// C_PhaseItemEventImplementationBase -- phase-item anim event (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08 (alloc 8 in ctor sub_18199BE7C / factory
// sub_18342C570).
// -----------------------------------------------
// RTTI TD rva 0x4FF2E30; COL rva 0x45C3C88; vtable 0x184010F38, 13 slots.
// Overrides: [1]sub_180EE58DC [2]sub_18150587C [3]nullsub_1 [4]sub_180C85240
// (base-shared) [10]sub_181A6CE3C [12]sub_1834346CC.  Inherited: [0][5..9][11].

namespace wh::xgenaimodule {

class C_PhaseItemEventImplementation : public C_PhaseItemEventImplementationBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PhaseItemEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE58DC
    bool  Apply(int param, void* ctx) override;          // [2]  sub_18150587C
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180EE5510 undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CE3C
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_1834346CC
};
static_assert(sizeof(C_PhaseItemEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
