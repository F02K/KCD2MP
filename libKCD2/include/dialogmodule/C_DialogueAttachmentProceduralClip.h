#pragma once
#include <cstddef>
#include "S_DialogueAttachmentProceduralParams.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueAttachmentProceduralClip -- Mannequin procedural clip
// (KCD2 1.5.6, kd7u).  sizeof 0x38 (alloc 56, shared_ptr factory sub_182806DA8).
// -----------------------------------------------
// RTTI TD 0x184B4EE40; primary vtable 0x183E67180 (13 slots); deleting-dtor [0]
// sub_180E0CC50. Bases per RTTI: TProceduralClip<S_DialogueAttachmentProceduralParams> +
// IProceduralClip (mdisp 0). The layout EXACTLY matches the SDK ICryMannequin.h shape
// (IProceduralClip: vptr + m_entity/m_charInstance/m_scope/m_action ptrs;
// TProceduralClip adds PARAMS m_params @+0x28) -- but the SDK header does not declare
// IProceduralParams in this env, so the chain is mirrored standalone (do NOT also
// derive the SDK TProceduralClip in a TU using this).
// Slots (names inferred; CryMannequin proc-clip order may be interfuscator-shuffled --
// mirrored for layout only): [1] sub_180947C48 Enter/Initialise (binds a1[1..3] +
// TProceduralClip base init sub_1804B02B0); [2] sub_1828079BC applies params (copies
// b8/b9/d12 into the embedded copy, then calls vtable[12]); [11] sub_1808D2480
// GetName/typename (returns ""); [12] sub_1803B6E80 nullsub (Update/Exit empty).

struct ICharacterInstance;
class IScope;
class IAction;

namespace wh::dialogmodule {

class C_DialogueAttachmentProceduralClip {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueAttachmentProceduralClip;
    virtual ~C_DialogueAttachmentProceduralClip();   // [0] sub_180E0CC50
    virtual void _vf1();    // [1] Enter/Initialise (sub_180947C48)
    virtual void _vf2();    // [2] apply params (sub_1828079BC)
    virtual void _vf3();    // [3] [role UNVERIFIED]
    virtual void _vf4();    // [4] [role UNVERIFIED]
    virtual void _vf5();    // [5] [role UNVERIFIED]
    virtual void _vf6();    // [6] [role UNVERIFIED]
    virtual void _vf7();    // [7] [role UNVERIFIED]
    virtual void _vf8();    // [8] [role UNVERIFIED]
    virtual void _vf9();    // [9] [role UNVERIFIED]
    virtual void _vf10();   // [10] [role UNVERIFIED]
    virtual void _vf11();   // [11] GetName/typename (sub_1808D2480, returns "")
    virtual void _vf12();   // [12] nullsub (Update/Exit empty)

    IEntity*            m_pEntity;        // +0x08  IProceduralClip::m_entity (Enter sub_180947C48 stores a2)
    ICharacterInstance* m_pCharInstance;  // +0x10  IProceduralClip::m_charInstance (Enter stores a3)
    IScope*             m_pScope;         // +0x18  IProceduralClip::m_scope (Enter stores a4)
    _smart_ptr<IAction> m_pAction;        // +0x20  IActionPtr; base init sub_1804B02B0 intrusive-AddRefs (refcnt @pointee+0x58) / releases old
    S_DialogueAttachmentProceduralParams m_params;   // +0x28  embedded params copy
};
static_assert(sizeof(C_DialogueAttachmentProceduralClip) == 0x38,
              "C_DialogueAttachmentProceduralClip must be 0x38 (alloc 56)");
static_assert(offsetof(C_DialogueAttachmentProceduralClip, m_params) == 0x28);

}  // namespace wh::dialogmodule
