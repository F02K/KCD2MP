#pragma once
#include "../framework/I_Patch.h"

// -----------------------------------------------
// wh::xgenaimodule::I_XGenAIModulePatch : framework::I_Patch -- XGenAI module-patch
// contract (KCD2 1.5.6, kd7u).  Abstract intermediate, adds slots [3..5].
// -----------------------------------------------
// RTTI TD rva 0x4FE8C20 (no standalone COL -- never most-derived). Slot split proven
// 3 (I_Patch) + 3 (this) + 0 (concrete) by the C_Patch1_3 vtables in xgenaimodule and
// entitymodule (G2_dossier.md §12). In C_Patch1_3: [3] returns a static patch
// descriptor (&unk_18558D440), [4]/[5] share one body. Slot names/semantics UNRESOLVED.

namespace wh::xgenaimodule {

class I_XGenAIModulePatch : public framework::I_Patch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_XGenAIModulePatch;
    virtual void _vf3() = 0;   // [3]  C_Patch1_3: sub_1820BDDB0 -> &unk_18558D440 (static patch descriptor) [role UNVERIFIED]
    virtual void _vf4() = 0;   // [4]  C_Patch1_3: sub_1820BDD30 [role UNVERIFIED]
    virtual void _vf5() = 0;   // [5]  C_Patch1_3: sub_1820BDD30 (same body as [4]) [role UNVERIFIED]
};
static_assert(sizeof(I_XGenAIModulePatch) == 0x08, "I_XGenAIModulePatch is vptr-only");

}  // namespace wh::xgenaimodule
