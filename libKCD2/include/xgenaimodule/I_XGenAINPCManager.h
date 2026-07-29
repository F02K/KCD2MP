#pragma once

// -----------------------------------------------
// wh::I_XGenAINPCManager -- cross-module NPC-manager interface mixin (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08.  NOTE: namespace wh, NOT xgenaimodule (mangled
// .?AVI_XGenAINPCManager@wh@@) -- the cross-module face of C_NPCManager (its +0x20
// MI base), registered with the wh registry via (*q_18492D8D8)->vf[0x3C0/8].
// -----------------------------------------------
// RTTI TD rva 0x4F6E400; standalone vtable 0x183FF1FB8, 3 slots:
// [0] dtor sub_183320E78, [1]/[2] _purecall. C_NPCManager subobject vtable @+0x20 =
// 0x183FF1948 ([0] sub_18213D6AC, [1] sub_180AEBC3C, [2] sub_180AEEA88).
// Slot names use the _xnpcm_ prefix to avoid colliding with C_NPCManager's own
// primary-slot _vfN names (a same-named declaration there would silently become an
// override instead of a new primary slot).

namespace wh {

class I_XGenAINPCManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_XGenAINPCManager;
    virtual ~I_XGenAINPCManager();      // [0] sub_183320E78
    virtual void _xnpcm_vf1() = 0;      // [1] [U role]
    virtual void _xnpcm_vf2() = 0;      // [2] [U role]
};
static_assert(sizeof(I_XGenAINPCManager) == 0x08, "I_XGenAINPCManager is vptr-only");

}  // namespace wh
