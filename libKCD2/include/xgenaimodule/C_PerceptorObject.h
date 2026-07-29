#pragma once
#include <cstdint>
#include <vector>
#include "C_PerceptionObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptorObject : C_PerceptionObject -- a perceiving entity
// record (the NPC-side counterpart of C_PerceptibleObject).  KCD2 WHGame.dll
// 1.5.6, kd7u.  sizeof 0xC0 (create-site proven: qword_18549D378(192) in
// sub_1803EBEB8 @0x1803ebf02; registered into the manager container via
// sub_1820852B0 @0x1803ebf4e).  SINGLE inheritance (CHD numBases 2 = {self,
// C_PerceptionObject@0}, ONE COL) -- the four adjacent .rdata vtables
// (...332a8/...332c0/...332e8/...33320) are four independent classes, not MI
// sub-vtables.
// -----------------------------------------------
// RTTI TD rva 0x4F4C5A0; COL 0x1840DFCA8; vtable 0x183A332C0, 4 slots (adds
// [2],[3]).  ctor sub_1803EC580; dtor body sub_1803EBE30 (destructs the +0x68/
// +0x80/+0x98 vectors via sub_180A608D4, frees the +0x18 vector).

namespace wh::xgenaimodule {

class C_PerceptorObject : public C_PerceptionObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptorObject;
    ~C_PerceptorObject() override;   // [0] sub_1803EBDEC (thunk -> body sub_1803EBE30)
    void _vf1() override;            // [1] sub_181A72470 -- const `return 1` predicate (IsPerceptor/IsActive [U name])
    virtual void Serialize(void* ar);      // [2] sub_1832AFD1C -- write: m_intVec count+data, m_registryEntry name, m_flag60, m_flagB8
    virtual void Deserialize(void* ar);    // [3] sub_1832AF550 -- read: symmetric; m_registryEntry resolved via qword_185481FC8 @0x1832af597

    std::vector<void*>   m_vec18;          // +0x18  8B elems (ctor @0x1803ec5ab; dtor frees via sub_181AB5490) [U elem type]
    int32_t              m_unk30;          // +0x30  ctor: -1
    int32_t              m_unk34;          // +0x34  ctor: 0
    int16_t              m_unk38;          // +0x38  ctor: 0
    uint8_t              _pad3A[6];        // +0x3A  align to +0x40
    std::vector<int32_t> m_intVec;         // +0x40  serialized as count + 4B elems (sub_1832AFD1C @0x1832afd4c)
    void*                m_registryEntry;  // +0x58  registry-resolved ptr (qword_185481FC8 lookup on load; write emits entry+16 name)
    uint8_t              m_flag60;         // +0x60  serialized byte (@0x1832afda7)
    uint8_t              _pad61[7];        // +0x61  align to +0x68
    std::vector<void*>   m_vec68;          // +0x68  dtor sub_180A608D4(a1+13) [U elem type]
    std::vector<void*>   m_vec80;          // +0x80  dtor sub_180A608D4(a1+16) [U elem type]
    std::vector<void*>   m_vec98;          // +0x98  dtor sub_180A608D4(a1+19) [U elem type]
    uint64_t             m_unkB0;          // +0xB0  ctor: 0
    uint8_t              m_flagB8;         // +0xB8  serialized byte (@0x1832afdb7)
    uint8_t              _padB9[7];        // +0xB9  align to 0xC0
};
static_assert(sizeof(C_PerceptorObject) == 0xC0, "C_PerceptorObject must be 0xC0 (alloc 192)");

}  // namespace wh::xgenaimodule
