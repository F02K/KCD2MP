#pragma once
#include <cstdint>
#include <memory>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UIApseCharacterContent.h"
#include "guimodule/C_FaderController.h"
#include "framework/C_ReadinessObserver.h"
#include "Offsets/vtables/IActionListener.h"
struct ISystemEventListener;                      // needed by the vendored SDK header below
#include "CryEngine/CryCommon/IHardwareMouse.h"   // IHardwareMouseEventListener

// -----------------------------------------------
// wh::guimodule::C_UICharacterBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  ABSTRACT, sizeof 0x3F8.
// -----------------------------------------------
// RTTI .?AVC_UICharacterBase@guimodule@wh@@ (TD 0x184C91E48). ctor sub_180BC71D4.
// Bases: C_UIBase @+0x00 + IActionListener @+0x10 + IHardwareMouseEventListener @+0x18.
// Own vtables 0x183ED8BF8 (primary, 39 slots) / 0x183ED8D38 (action) / 0x183ED8D58
// (hw-mouse). Size proven by the sole subclass C_UIApseCharacter's first own write
// @+0x3F8.
//
// Big abstract "character panel" API: C_UIBase 10 slots + own [10..38] (29 virtuals,
// 13 of them _purecall in the base vtable -- filled by C_UIApseCharacter). Own-slot
// roles all UNVERIFIED (coined _vfNN; base impl addrs cited below; [11..13] share the
// getter body 0x180838AE0). Primary overrides: [1] Init 0x180ED217C, [2] Deinit
// 0x182AFE69C, [4] Update 0x180567D70.

namespace wh::guimodule {

class C_UIApseCharacterSlots;
template <typename T> class C_BasicFader;   // 0x30 fader worker (vftable + CryStringT@+0x10); no full RE yet

class C_UICharacterBase : public C_UIBase,
                          public Offsets::IActionListener,        // +0x10
                          public IHardwareMouseEventListener {    // +0x18
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICharacterBase;
    virtual void _vf10() = 0;   // [10] _purecall
    virtual void _vf11();       // [11] 0x180838AE0 (shared getter)
    virtual void _vf12();       // [12] 0x180838AE0 (shared getter)
    virtual void _vf13();       // [13] 0x180838AE0 (shared getter)
    virtual void _vf14();       // [14] 0x180C47894
    virtual void _vf15() = 0;   // [15] _purecall
    virtual void _vf16();       // [16] 0x18146C0CC
    virtual void _vf17();       // [17] nullsub
    virtual void _vf18();       // [18] 0x180C41E00
    virtual void _vf19();       // [19] 0x180C4A1BC
    virtual void _vf20();       // [20] 0x180C4A0EC
    virtual void _vf21() = 0;   // [21] _purecall
    virtual void _vf22() = 0;   // [22] _purecall
    virtual void _vf23();       // [23] 0x181951C90
    virtual void _vf24();       // [24] 0x1816EA0D8
    virtual void _vf25();       // [25] 0x1808EE70C
    virtual void _vf26() = 0;   // [26] _purecall
    virtual void _vf27();       // [27] 0x1808F0810
    virtual void _vf28();       // [28] 0x1819ED1A0
    virtual void _vf29() = 0;   // [29] _purecall
    virtual void _vf30() = 0;   // [30] _purecall
    virtual void _vf31() = 0;   // [31] _purecall
    virtual void _vf32();       // [32] 0x182B02750
    virtual void _vf33() = 0;   // [33] _purecall
    virtual void _vf34() = 0;   // [34] _purecall
    virtual void _vf35() = 0;   // [35] _purecall
    virtual void _vf36() = 0;   // [36] _purecall
    virtual void _vf37() = 0;   // [37] _purecall
    virtual void _vf38();       // [38] 0x18056182C

    // Interface impls live in the base subobject vtables 0x183ED8D38 / 0x183ED8D58
    // (slot bodies NOT dumped this wave -- UNVERIFIED which are defaults).
    void OnAction(const Offsets::SActionId& action, int activationMode, float value) override;      // action vtable 0x183ED8D38
    void OnHardwareMouseEvent(int iX, int iY, EHARDWAREMOUSEEVENT event, int wheelDelta) override;  // hw-mouse vtable 0x183ED8D58

    void*    m_unk20;             // +0x20  ctor 0 [role UNVERIFIED]
    uint8_t  m_stats28[0xC8];     // +0x28  embedded wh::guimodule::C_UICharacterStats (ctor sub_180BC6F70 stores C_UICharacterStats::vftable; dtor sub_182AFCC34(this+0x28)); NO REd header yet -- kept as bytes
    C_UIApseCharacterSlots* m_pSlots;  // +0xF0  shared_ptr-managed slots panel (filled by the derived ctor via sub_18146D050); stored-pointer half only -- control-block slot location UNVERIFIED (dossier conflict: +0xF8 is also ctor-written)
    int32_t  m_unkF8;             // +0xF8  ctor 0; standalone int32 -- m_pSlots@+0xF0 is a RAW OWNING ptr (dtor sub_182AFCB70 deletes it via a1[30]->deleting-dtor, NOT a shared_ptr) so +0xF8 is NOT a control-block half; role UNVERIFIED
    uint8_t  m_timerFC[0x30];     // +0xFC  sub_180A70610(this+0xFC, 1.4f, ...) timing block [layout UNVERIFIED]
    float    m_floats12C[9];      // +0x12C..+0x150  ctor floats (1.0f @+0x138/+0x148) [roles UNVERIFIED]
    uint8_t  m_timer150[0x30];    // +0x150  sub_180A70610(this+0x150, 0.5f, ...) timing block [layout UNVERIFIED]
    uint8_t  m_unk180[0x40];      // +0x180..+0x1C0  ctor-zeroed block (sub_180BC71D4 zeroes +0x180..+0x1C0); sub-field types/roles UNVERIFIED
    bool     m_flag1C0;           // +0x1C0  ctor 1 [role UNVERIFIED]
    uint8_t  _pad1C1[7];          // +0x1C1
    C_UIApseCharacterContent m_content;  // +0x1C8  (0x200) flash element "ApseCharacter"
    uint16_t m_word3C8;           // +0x3C8  ctor 0
    uint8_t  m_byte3CA;           // +0x3CA  ctor 0
    uint8_t  _unk3CB[0xD];        // +0x3CB..+0x3D8  +0x3CB untouched; +0x3CC(qword) & +0x3D4(dword) are ctor-zeroed; sub-field types UNVERIFIED
    void*    m_treeHead3D8;       // +0x3D8  MSVC std::_Tree _Myhead (RB-tree); ctor sub_181AB55C0(56) sentinel self-refs +0/+8/+16, WORD@+24=0x0101 {_Color=1,_Isnil=1}; dtor sub_180B5336C frees nodes then frees the 56B head
    uint64_t m_treeSize3E0;       // +0x3E0  std::_Tree _Mysize (element count; ctor 0); node value size 0x18; set-vs-map & element type UNVERIFIED
    std::unique_ptr<C_BasicFader<C_FaderController>> m_fader3E8;  // +0x3E8  owning (ctor 0); dtor sub_1807D048C -> sub_18194B6E0(p,1) deleting-dtor names C_BasicFader<C_FaderController>
    std::unique_ptr<C_ReadinessObserver> m_readinessObserver3F0;  // +0x3F0  owning (ctor 0); lazily created in Update sub_181F47710 (112B wh::C_ReadinessObserver), move-assigned sub_182AF8EE4, dtor sub_180968324 (virtual deleting-dtor slot 7)
};
static_assert(sizeof(C_UICharacterBase) == 0x3F8, "C_UICharacterBase must be 0x3F8");
static_assert(offsetof(C_UICharacterBase, m_content) == 0x1C8, "content pane at 0x1C8");

}  // namespace wh::guimodule
