#pragma once
#include <cstdint>
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIObject -- root of the AI object-model spine (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x20 (create-site proven: sub_1804F75C0(32) -> ctor, both
// formation paths).
// -----------------------------------------------
// RTTI TD rva 0x4EFD890; vtable 0x1840140B0, 9 slots. The spine (one grown primary
// vtable, all bases mdisp 0):
//   C_AIObject(9) -> C_LinkableObject(19) -> C_MessageCapableObject(20)
//   -> C_IntelligentObject(27) -> I_NPC(64) -> C_NPC(64)
// Real ctor sub_18047BDFC(this, const WUID* wuid, I_AIPuppet* puppet): registers this
// in C_AIObjectManager (q_185481FC8) hashmap @mgr+0x60 keyed by m_linkableId (insert
// node+0x18 = this, then two notify calls). Dtor sub_18047B6A4 (via slot-0 deleting
// dtor sub_18344211C) unregisters. Find-by-id: sub_18047C1F8 (FNV-1a-64, offset basis
// 0xCBF29CE484222325). Slots [2..5] forward into the held puppet's vtable (puppet
// slots [5]/[14]/[17]/[19]) -- +0x18 is the PUPPET, not the raw host entity
// (G2_supplement §7.1). Layout proven by the ctor (G2_supplement §3.2).

namespace wh::xgenaimodule {

class I_AIPuppet;

class C_AIObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIObject;
    virtual ~C_AIObject();               // [0] deleting dtor sub_18344211C -> dtor sub_18047B6A4 (unregisters from C_AIObjectManager)
    virtual uint32_t GetTypeMask();      // [1] sub_181AABDC0 -> dword_18533AE80; per-class type bit consumed by the checked-cast sub_1805D2764 [V]
    virtual void _vf2();                 // [2] sub_180647E98 -> m_puppet->[5] UpdateCachedTransform [V fwd]
    virtual void _vf3();                 // [3] sub_1808777F0 -> m_puppet->[14] [V fwd, U role]
    virtual void _vf4();                 // [4] sub_18081361C -> m_puppet->[17] [V fwd, U role]
    virtual void _vf5();                 // [5] sub_1834437BC -> m_puppet->[19] [V fwd, U role]
    virtual void* _vf6();                // [6] sub_18066CD10 -- return 0 (default) [V]
    virtual void* GetStaticTypeToken();  // [7] sub_183442EE0 -> &qword_1856098F8 (lazy static, atexit-registered) [V]
    virtual void* _vf8();                // [8] sub_181AA2090 -> returns global qword_185496678 [V, U role]

    framework::WUID m_linkableId;        // +0x08  the C_AIObjectManager hashmap key [V]
    int32_t         m_unk10;             // +0x10  ctor: -1 [U role]
    int32_t         m_unk14;             // +0x14  ctor: -1 [U role]
    I_AIPuppet*     m_puppet;            // +0x18  the puppet (NOT the raw host entity) [V]
};
static_assert(sizeof(C_AIObject) == 0x20, "C_AIObject must be 0x20");

}  // namespace wh::xgenaimodule
