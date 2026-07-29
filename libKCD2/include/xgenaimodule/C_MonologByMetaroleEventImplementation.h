#pragma once
#include "C_MonologEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MonologByMetaroleEventImplementation :
// C_MonologEventImplementation -- monolog anim event addressed by metarole (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (alloc 8 in ctor sub_18197A428 / factory
// sub_18342C4EC).
// -----------------------------------------------
// RTTI TD rva 0x4FF2E80; COL rva 0x45C3A30; vtable 0x184011100, 14 slots.
// Overrides: [1]sub_180EE5954 [2]sub_1808AC89C (SHARED with MonologByAlias)
// [3]nullsub_1 [4]sub_180EE5BF4 [10]sub_181A6CEC8 [12]sub_1834346AC
// [13]sub_181779750 -> "metarole('%s'),canBeDelayed(%s),overrideContextSuppress
// (%s),defaultAnimState(%i),forceSubtitles(%s)".  Inherited: [0][5..9][11].

namespace wh::xgenaimodule {

class C_MonologByMetaroleEventImplementation : public C_MonologEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MonologByMetaroleEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE5954
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1808AC89C (shared body)
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    void* BuildStringA(void* out) override;              // [4]  sub_180EE5BF4
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x18178203C undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CEC8
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_1834346AC
    const char* GetFormatString() override;              // [13] sub_181779750 (metarole format string)
};
static_assert(sizeof(C_MonologByMetaroleEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
