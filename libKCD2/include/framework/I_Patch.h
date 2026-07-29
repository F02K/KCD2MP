#pragma once

// -----------------------------------------------
// wh::framework::I_Patch -- savegame/DB migration patch root (KCD2 1.5.6, kd7u).
// Pure interface, 3 slots.
// -----------------------------------------------
// RTTI TD rva 0x4C734A8; standalone vtable 0x183EC9B80, exactly 3 slots:
// [0] dtor sub_181F23620 (shared trivial), [1] _purecall, [2] _purecall.
// Root of every module's patch chain: I_XGenAIModulePatch/I_EntityModulePatch/
// I_GUIModulePatch/I_RPGPatch each add 3 module-contract slots on top (proven by the
// identically-shaped 6-slot C_Patch1_3 vtables in xgenaimodule AND entitymodule --
// G2_dossier.md §12). Slot semantics UNRESOLVED.

namespace wh::framework {

class I_Patch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Patch;
    virtual ~I_Patch();        // [0]  sub_181F23620 (shared trivial dtor)
    virtual void _vf1() = 0;   // [1]  [role UNVERIFIED]
    virtual void _vf2() = 0;   // [2]  [role UNVERIFIED]
};
static_assert(sizeof(I_Patch) == 0x08, "I_Patch is vptr-only");

}  // namespace wh::framework
