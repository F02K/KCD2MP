#pragma once
#include "C_MonologEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MonologByAliasEventImplementation :
// C_MonologEventImplementation -- monolog anim event addressed by alias (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (alloc 8 in ctor sub_18197A4BC / factory
// sub_18342C468).
// -----------------------------------------------
// RTTI TD rva 0x4FF2FA0; COL rva 0x45C3AF8; vtable 0x184011088, 14 slots.
// Overrides: [1]sub_180EE59CC [2]sub_1808AC89C (SHARED with MonologByMetarole)
// [3]nullsub_1 [4]sub_180EE5BD8 [10]sub_181A6CEE4 [12]sub_18343468C
// [13]sub_181AAB770 -> "alias('%s'),canBeDelayed(%s),overrideContextSuppress(%s),
// defaultAnimState(%i),forceSubtitles(%s)".  Inherited: [0][5..9][11].

namespace wh::xgenaimodule {

class C_MonologByAliasEventImplementation : public C_MonologEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MonologByAliasEventImplementation;
    void* GetName() override;                            // [1]  sub_180EE59CC
    bool  Apply(int param, void* ctx) override;          // [2]  sub_1808AC89C (shared body)
    void  ApplySecondary(int param, void* ctx) override; // [3]  nullsub_1
    void* BuildStringA(void* out) override;              // [4]  sub_180EE5BD8
    // trio overrides [10]/[12] ([11] get_ptr inherited); RTTR_ENABLE(<base>) deferred -- base lambda 0x18178203C undecoded
    rttr::type get_type() const override;                    // [10] sub_181A6CEE4
    rttr::detail::derived_info get_derived_info() override;  // [12] sub_18343468C
    const char* GetFormatString() override;              // [13] sub_181AAB770 (alias format string)
};
static_assert(sizeof(C_MonologByAliasEventImplementation) == 0x08, "vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
