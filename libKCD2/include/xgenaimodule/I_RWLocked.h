#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_RWLocked -- read/write-lock mixin embedded at +0x20 of every
// C_LinkableObject-derived object (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4EFD858. Never most-derived (no COL of its own); its 3-slot subobject
// vtable appears at host+0x20 in every carrier (C_LinkableObject 0x183A39800,
// C_IntelligentObject 0x183A6C210, C_MessageCapableObject 0x183A6C310, I_NPC
// 0x183A83680, C_NPC 0x183FF1F18) with ALL THREE slots = nullsub_1 (0x1803B6E80) at
// every level. Slot 0 is NOT a virtual dtor: a dtor slot would carry a deleting-dtor
// thunk in the most-derived carriers' subobject vtables, but even C_NPC keeps nullsub
// there. Slot roles (presumably no-op lock/unlock hooks) UNRESOLVED.

namespace wh::xgenaimodule {

class I_RWLocked {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RWLocked;
    virtual void _vf0();   // [0] nullsub_1 at every level [U role]
    virtual void _vf1();   // [1] nullsub_1 [U role]
    virtual void _vf2();   // [2] nullsub_1 [U role]
};
static_assert(sizeof(I_RWLocked) == 0x08, "I_RWLocked is vptr-only");

}  // namespace wh::xgenaimodule
