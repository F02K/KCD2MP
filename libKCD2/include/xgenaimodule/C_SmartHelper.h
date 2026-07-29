#pragma once
#include <cstdint>
#include "I_PositionAndOrientation.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartHelper : I_PositionAndOrientation -- a smart-object
// helper point (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x140 (create-site
// proven: sub_1803E4364 @0x1803E43ED: sub_1804F75C0(320) -> ctor
// sub_1803E5184(this, a2=WUID, a3, a4=vec/quat, a5=typeId)).  dtor
// sub_183441EA0.
// -----------------------------------------------
// RTTI TD rva 0x4FF3A28 (2 bases).  Vtable rva 0x4014090 (3 slots:
// [0]0x1A72EB0 [1]0x1A7FB20 [2]0x3442294).  Interior container regions typed
// as blobs [U interior].

namespace wh::xgenaimodule {

class C_SmartHelper : public I_PositionAndOrientation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartHelper;
    void PaoVf0() override;   // [0] 0x1A72EB0
    void PaoVf1() override;   // [1] 0x1A7FB20
    void PaoVf2() override;   // [2] 0x3442294

    uint32_t        m_typeId;        // +0x08  ctor a5
    uint8_t         _pad0C[4];       // +0x0C
    framework::WUID m_wuid;          // +0x10  copied from ctor a2 (sub_1804FD208)
    uint64_t        m_pos18;         // +0x18  qword from ctor [U meaning]
    uint32_t        m_20;            // +0x20  from ctor a3 [U role]
    uint8_t         m_vecQuat24[16]; // +0x24..+0x34  OWORD copied from ctor a4 (vec/quat) [U type]
    uint8_t         _pad34[4];       // +0x34
    uint8_t         m_vec38[0x18];   // +0x38..+0x50  vector [U elem type]
    int32_t         m_handle50;      // +0x50  ctor: -1
    int32_t         m_handle54;      // +0x54  ctor: -1
    float           m_scale58;       // +0x58  ctor: 1.0f [U role]
    uint8_t         _pad5C[4];       // +0x5C
    void*           m_mapHead60;     // +0x60  std::unordered_map (+0x58..+0x98) sentinel list-head node* (32B self-linked)
    size_t          m_mapSize68;     // +0x68  std::unordered_map element count (_List._Mysize)
    uint8_t         m_hash70[0x28];  // +0x70..+0x98  hash-map (mask 7 @+0x88, count 8 @+0x90) [U interior]
    uint8_t         m_vec98[0x18];   // +0x98..+0xB0  vector [U elem type]
    uint8_t         m_subB0[0x40];   // +0xB0..+0xF0  sub-object (sub_1803E52BC) [U interior]
    uint8_t         m_zeroF0[0x40];  // +0xF0..+0x130  zeroed containers [U types]
    uint8_t         _unk130[8];      // +0x130  owned-gate: dtor frees heap buf @+0x120 if nonzero (qword_18549D358, size->dword_1851DFB1C) [U scalar]
    uint8_t         m_byte138;       // +0x138  ctor: 0 [U role]
    uint8_t         _pad139[7];      // +0x139
};
static_assert(sizeof(C_SmartHelper) == 0x140, "C_SmartHelper must be 0x140 (alloc 320 at sub_1803E4364)");

}  // namespace wh::xgenaimodule
