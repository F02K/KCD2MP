#pragma once
#include "C_PhaseItemEventImplementationBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SetItemPhaseEventImplementation :
// C_PhaseItemEventImplementationBase -- "setitemphase" anim event (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (create-site proven: sub_1804F75C0(8) in
// ctor sub_1820C0BF0; factory sub_1820C0C30).
// -----------------------------------------------
// RTTI TD rva 0x4FF30D0; COL rva 0x45C3BC0; vtable 0x184010EC8, 13 slots.
// Overrides: [1]sub_1820C22C0 ("setitemphase") [2]sub_1820C24A0 [3]nullsub_1
// [4]sub_180C85240 (base-shared) [10]sub_1820C3EA0 [12]sub_1820C3EC0.
// Inherited: [0][5][6][7][8][9][11].

namespace wh::xgenaimodule {

class C_SetItemPhaseEventImplementation : public C_PhaseItemEventImplementationBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SetItemPhaseEventImplementation;
    void* GetName() override;                            // [1]  sub_1820C22C0 -> "setitemphase"
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1820C24A0
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x180EE5510 undecoded
    rttr::type get_type() const override;                    // [10] sub_1820C3EA0
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_1820C3EC0
};
static_assert(sizeof(C_SetItemPhaseEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
