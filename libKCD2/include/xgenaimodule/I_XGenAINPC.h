#pragma once

// -----------------------------------------------
// wh::I_XGenAINPC -- cross-module NPC interface mixin embedded at +0xA8 of every NPC
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.  NOTE: namespace wh, NOT xgenaimodule
// (mangled .?AVI_XGenAINPC@wh@@) -- declared by xgenaimodule's I_NPC for consumers in
// other modules.
// -----------------------------------------------
// RTTI TD rva 0x4F6E430; standalone vtable 0x183A830E0, 2 slots:
// [0] dtor sub_183320E4C, [1] _purecall. Subobject vtables: I_NPC+0xA8 -> 0x183A83668
// ([0] sub_18213D6B8, [1] _purecall), C_NPC+0xA8 -> 0x183FF1B00 ([0] sub_18213D688,
// [1] sub_1804DFADC = the only implementation).

namespace wh {

class I_XGenAINPC {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_XGenAINPC;
    virtual ~I_XGenAINPC();      // [0] sub_183320E4C
    virtual void _vf1() = 0;     // [1] _purecall; implemented only by C_NPC (sub_1804DFADC) [U role]
};
static_assert(sizeof(I_XGenAINPC) == 0x08, "I_XGenAINPC is vptr-only");

}  // namespace wh
