#pragma once
#include <cstdint>
#include "C_MessageCapableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_IntelligentObject : C_MessageCapableObject -- intelligent spine
// level (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xA8 (own members +0x60..+0xA7 proven
// by ctor; G2_supplement §3.5).
// -----------------------------------------------
// RTTI TD rva 0x4EFD9C8; primary vtable 0x183A6C230, 27 slots (implements [19], adds
// [20..26]); I_RWLocked subobject vtable @+0x20 = 0x183A6C210. Real ctor
// sub_180A2BC4C(this, const WUID* wuid, I_AIPuppet* puppet): registers in the
// per-level intelligent-object registry q_1854930D0 (hashmap @mgr+0x60; owning
// manager class identity [U]). OnMessage ([19], sub_18070DBD0) gates on
// m_messageGate->vf[3]() -- if the gate is absent or refuses, the message is marked
// state 7 and the dispatcher returns 3 (rejected).

namespace wh::xgenaimodule {

class C_MessageInbox;

class C_IntelligentObject : public C_MessageCapableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_IntelligentObject;
    ~C_IntelligentObject() override;                       // [0]  deleting dtor sub_1832086F0
    uint32_t GetTypeMask() override;                       // [1]  sub_181AA3690
    int64_t OnMessage(void* msg) override;                 // [19] sub_18070DBD0 -- gate check via m_messageGate->vf[3]() [V]
    virtual void _vf20();                                  // [20] sub_18071174C [U role]
    virtual void SetMessageInbox(C_MessageInbox* inbox);   // [21] sub_180A2AD4C; the NPC factory attaches the inbox through this slot (vf +0xA8) [V-usage, U name]
    virtual void _vf22();                                  // [22] sub_180A2A858 [U role]
    virtual void _vf23();                                  // [23] sub_1807FAC88 [U role]
    virtual void _vf24();                                  // [24] nullsub_1
    virtual void _vf25();                                  // [25] sub_183209730 [U role]
    virtual void _vf26();                                  // [26] sub_180A2C3E0 [U role]

    void*           m_staticDefault;   // +0x60  DynArray<T> head (CryEngine SmallDynStorage, single ptr; empty = &unk_185665430; blk hdr {cap@-8,cnt@-4}); elem 0x10 {obj* @0, callback/vtbl* @8}; teardown sub_1804CDAE8 / walk sub_180875DF8 [elem T U]
    int16_t         m_word68;          // +0x68  ctor: -1 [U role]
    int16_t         m_word6A;          // +0x6A  ctor: -1 [U role]
    uint8_t         _pad6C[4];         // +0x6C
    void*           m_messageGate;     // +0x70  message-gate object*, set post-ctor; concrete type [U] (brain candidate, G4 wave)
    void*           m_typeToken;       // +0x78  intrusive _smart_ptr<T> (assign sub_1804FB678: AddRef vf[3]/Release vf[4]; dtor sub_1804FB980 Release vf[4]); pointee = RTTR type descriptor (puppet host type via sub_1805D7094, else registry qword_18492D828 vf[13](0)) [pointee class U]
    void*           m_entries;         // +0x80  flat-vec data {T* data; size_t count; size_t cap}; elem 0x10 {CryStringT<char> @0, void* @8 (obj*, U role)}; dtor sub_180A2C368 decrefs each name @[ptr-12]
    uint64_t        m_entryCount;      // +0x88  element count (arg a1[1] to sub_180A2C368/sub_180A2C3A8)
    uint64_t        m_entryCap;        // +0x90  capacity (elements)
    framework::WUID m_wuid;            // +0x98  own WUID copy at this level
    bool            m_flagA0;          // +0xA0  ctor: 0 [U role]
    bool            m_flagA1;          // +0xA1  ctor: 0 [U role]
    uint8_t         _padA2[6];         // +0xA2
};
static_assert(sizeof(C_IntelligentObject) == 0xA8, "C_IntelligentObject must be 0xA8");

}  // namespace wh::xgenaimodule
