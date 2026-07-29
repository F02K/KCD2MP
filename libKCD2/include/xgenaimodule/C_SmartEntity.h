#pragma once
#include <cstdint>
#include "C_IntelligentObject.h"
#include "I_SmartEntitySubbrainListener.h"
#include "I_SmartEntity.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEntity : C_IntelligentObject (primary chain, carries
// I_RWLocked @+0x20) + I_SmartEntitySubbrainListener (@+0xA8) +
// XGenPublic::I_SmartEntity (@+0xB0) -- root of the smart-entity family (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x188 (PROVEN as subobject: I_Area mdisp =
// 392 in C_SmartArea; C_InteractiveSmartEntity/C_SmartArea extend at +0x188).
// ctor sub_180A2B2C8 (base-chain ctor sub_180A2BC4C).
// -----------------------------------------------
// RTTI TD rva 0x4F51DF0.  Vtables: primary rva 0x4013E50 (40 slots -- extends
// the 27-slot C_IntelligentObject table with NEW [27..39]); +0x20 I_RWLocked
// rva 0x4013FB8 (3 nullsub); +0xA8 listener rva 0x4013F98 ([0]0xCD4CD0
// [1]0x12D1778 [2]0x12D174C); +0xB0 public iface rva 0x4013FD8 (6 slots).
// Primary overrides vs C_IntelligentObject: [0]dtor 0x3442260,
// [1]GetTypeMask 0x1AABDF0, [16]0x3443E74, [18]0x3443DE4, [20]0x711720,
// [25]0x34431D8.  NEW slots: [27]0x1A72EA0 [28]0xA2AD98 [29]0xA2C184
// [30]0xA2DF94 [31]0x1AABE40 [32]0x1AABE30 [33]0x1AABE20 [34]0x1AABE00
// [35]0xA2ADF8 [36]0xB29810 [37]0x7FACE0 [38]0x3442FF0 [39]0x66CD10.
// Data-member types beyond the container skeletons partly UNVERIFIED (G6F).

namespace wh::xgenaimodule {

class C_SmartEntity
    : public C_IntelligentObject,
      public I_SmartEntitySubbrainListener,
      public XGenPublic::I_SmartEntity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEntity;
    ~C_SmartEntity() override;        // [0]  deleting dtor 0x3442260
    uint32_t GetTypeMask() override;  // [1]  0x1AABDF0
    void _vf16() override;            // [16] 0x3443E74
    void _vf18() override;            // [18] 0x3443DE4
    void _vf20() override;            // [20] 0x711720
    void _vf25() override;            // [25] 0x34431D8
    virtual void _vf27();             // [27] NEW 0x1A72EA0 [U role]
    virtual void _vf28();             // [28] NEW 0xA2AD98 [U role]
    virtual void _vf29();             // [29] NEW 0xA2C184 [U role]
    virtual void _vf30();             // [30] NEW 0xA2DF94 [U role]
    virtual void _vf31();             // [31] NEW 0x1AABE40 [U role]
    virtual void _vf32();             // [32] NEW 0x1AABE30 [U role]
    virtual void _vf33();             // [33] NEW 0x1AABE20 [U role]
    virtual void _vf34();             // [34] NEW 0x1AABE00 [U role]
    virtual void _vf35();             // [35] NEW 0xA2ADF8 [U role]
    virtual void _vf36();             // [36] NEW 0xB29810 [U role]
    virtual void _vf37();             // [37] NEW 0x7FACE0 [U role]
    virtual void _vf38();             // [38] NEW 0x3442FF0 [U role]
    virtual void _vf39();             // [39] NEW 0x66CD10 (return 0) [U role]
    // I_SmartEntitySubbrainListener impls (defined once here, inherited family-wide)
    void SeslVf0() override;          // [0] 0xCD4CD0
    void SeslVf1() override;          // [1] 0x12D1778
    void SeslVf2() override;          // [2] 0x12D174C
    // XGenPublic::I_SmartEntity impls
    void XseVf0() override; void XseVf1() override; void XseVf2() override;
    void XseVf3() override; void XseVf4() override; void XseVf5() override;

    wh::shared::C_Signal<> m_signalB8;   // +0xB8  (0x10) C_Signal; idle delegate-table sentinel &unk_1856099A0; teardown sub_180A2DC18 [sig U]
    wh::shared::C_Signal<> m_signalC8;   // +0xC8  (0x10) C_Signal; idle delegate-table sentinel &unk_18566C430; teardown sub_180A2DBC0 [sig U]
    wh::shared::C_Signal<> m_signalD8;   // +0xD8  (0x10) C_Signal; idle delegate-table sentinel &unk_18566C430; teardown sub_180A2DBC0 [sig U]
    uint8_t  m_containersE8[0x70]; // +0xE8..+0x158  zeroed vector/map skeletons [U types]
    framework::WUID m_wuid158;  // +0x158  copy of ctor WUID arg (*a2); base C_IntelligentObject stores the same value at +0x98
    uint8_t  _unk160[0x20];    // +0x160..+0x180  zeroed (+0x174 dword, +0x17C byte) [U roles]
    uint8_t  m_flags180;       // +0x180  3 bool bits (&= 0xF8)
    uint8_t  _pad181[7];       // +0x181
};
static_assert(sizeof(C_SmartEntity) == 0x188, "C_SmartEntity must be 0x188 (I_Area mdisp 392 in C_SmartArea)");

}  // namespace wh::xgenaimodule
