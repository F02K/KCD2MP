#pragma once
#include <cstdint>
#include <vector>
#include "C_AIObject.h"
#include "I_RWLocked.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LinkableObject : C_AIObject + I_RWLocked(@+0x20) -- linkable
// spine level (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60 (derived: own 7-qword
// region +0x28..+0x5F is followed immediately by C_IntelligentObject's members at
// +0x60, and C_MessageCapableObject adds no data).
// -----------------------------------------------
// RTTI TD rva 0x4EFD908; primary vtable 0x183A39820, 19 slots (adds [9..18]);
// I_RWLocked subobject vtable @+0x20 = 0x183A39800 (3x nullsub). Real ctor
// sub_18047BD70(this, const WUID*, I_AIPuppet*): base ctor pass-through (disasm:
// r8 untouched), writes both vptrs, zeroes +0x28..+0x58, registers in q_1854961B0
// (count++ @mgr+0x60). Side branch: C_DynamicLinkableObject ctor sub_18047BCFC
// (registers in q_1854961A0). Slot 11 is THE GetOwnership slot -- certified across
// all 16 audited host vtables (G2_supplement §6.2); this default impl returns the
// process-wide static C_EmptyOwnership.
// +0x28 region resolved (G3): the edge walker sub_18047AFA4 iterates
// [this+0x28, this+0x30) as a contiguous array of S_LinkEdge* -> std::vector;
// the second vector @+0x40 and the owned pointer @+0x58 are zeroed by the ctor
// but their element/pointee types were not observed [U].

namespace wh::xgenaimodule {

class I_Ownership;
class S_LinkEdge;

class C_LinkableObject : public C_AIObject, public I_RWLocked {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkableObject;
    ~C_LinkableObject() override;          // [0]  deleting dtor sub_1834421C4
    uint32_t GetTypeMask() override;       // [1]  sub_1805D29F0
    virtual void _vf9();                   // [9]  nullsub_1
    virtual void _vf10();                  // [10] sub_180636330 [U role]
    virtual I_Ownership* GetOwnership();   // [11] sub_18047AFF4 -> lazy static C_EmptyOwnership @off_18492F5F8 (never NULL) [V]
    virtual void _vf12();                  // [12] sub_180838AE0 (shared stub) [U role]
    virtual void* _vf13();                 // [13] sub_18066CD10 -- return 0
    virtual bool _vf14();                  // [14] sub_18041A6A0 -- return 1
    virtual bool _vf15();                  // [15] sub_18041A6A0 -- return 1
    virtual void _vf16();                  // [16] sub_18320A8D4 [U role]
    virtual void* _vf17();                 // [17] sub_1805F5DA0 -- RTTR get_ptr
    virtual void _vf18();                  // [18] sub_18320A894 [U role]

    std::vector<S_LinkEdge*> m_links;      // +0x28  outgoing link edges (walked by sub_18047AFA4 as [begin,end)) [V]
    std::vector<void*> m_vec40;            // +0x40  second edge/holder vector; zeroed by ctor [U elem type]
    void* m_ptr58;                         // +0x58  owned pointer; zeroed by ctor [U role]
};
static_assert(sizeof(C_LinkableObject) == 0x60, "C_LinkableObject must be 0x60 (C_AIObject 0x20 + I_RWLocked @0x20 + 7 qwords)");

}  // namespace wh::xgenaimodule
