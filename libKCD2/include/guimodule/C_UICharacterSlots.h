#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/I_UIDragAndDropTarget.h"

// -----------------------------------------------
// wh::guimodule::C_UICharacterSlots -- KCD2 WHGame.dll 1.5.6 (kd7u).  ABSTRACT, base extent 0x48.
// -----------------------------------------------
// RTTI .?AVC_UICharacterSlots@guimodule@wh@@ (TD 0x184C91E80). NO own COL -- never
// instantiated standalone; sole concrete subclass is C_UIApseCharacterSlots, whose
// primary vtable 0x183B8F3A0 (14 slots) realizes this shape: C_UIBase 10 + own
// [10..13]. Bases: C_UIBase @+0x00 + I_UIDragAndDropTarget @+0x10.
//
// Abstract equipment-slot panel. Own slot addrs in the subclass vtable: [10] 0x180C41F40,
// [11] 0x180C4A540, [12] 0x181F4B660, [13] 0x1805682AC -- base-vs-derived split
// UNVERIFIED (no base COL to diff), so all four are declared pure here and implemented
// by the subclass. Members +0x18..+0x40 written on the sub_18146D050 construction path
// (+0x18 is the inherited I_UIDragAndDropTarget::m_dragCtx08, not an own member --
// verify pass); which ctor (base vs derived) writes them is UNVERIFIED.

namespace wh::guimodule {

class C_UICharacterBase;
class C_UIApseCharacterContent;

class C_UICharacterSlots : public C_UIBase, public I_UIDragAndDropTarget {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICharacterSlots;
    virtual void _vf10() = 0;   // [10] role UNVERIFIED
    virtual void _vf11() = 0;   // [11] role UNVERIFIED
    virtual void _vf12() = 0;   // [12] role UNVERIFIED
    virtual void _vf13() = 0;   // [13] role UNVERIFIED

    // (+0x18 = inherited I_UIDragAndDropTarget::m_dragCtx08 -- set from arg in DnD vf[1]
    // sub_1811A234C, cleared in vf[2] sub_1811A2A40)
    C_UIApseCharacterContent* m_pContent;  // +0x20  -> owner's content pane (owner+0x1C8)
    C_UICharacterBase*        m_pOwner;    // +0x28  owning character panel
    void*                     m_unk30;     // +0x30  non-owning context ptr: read as m_unk30+0x668 w/ vtable calls in DnD-target vf[1] sub_1811A234C; identity-compared in vf[12] sub_181F4B660 (sets m_flag38), reset in vf[11] sub_180C4A540
    bool                      m_flag38;    // +0x38  ctor 0 [role UNVERIFIED]
    uint8_t                   _pad39[7];   // +0x39
    void*                     m_unk40;     // +0x40  ctor 0 [role UNVERIFIED]
};
static_assert(sizeof(C_UICharacterSlots) == 0x48, "C_UICharacterSlots base extent must be 0x48");

}  // namespace wh::guimodule
