#pragma once
#include <cstdint>
#include "guimodule/C_UICharacterBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCharacter -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x468.
// -----------------------------------------------
// RTTI .?AVC_UIApseCharacter@guimodule@wh@@ (TD 0x184C92000). ctor sub_18146CDF8.
// Base: C_UICharacterBase @+0x00 (= C_UIBase + IActionListener@+0x10 +
// IHardwareMouseEventListener@+0x18). vtables: primary 0x183ED8FE0 (39 slots) /
// action 0x183ED9120 / hw-mouse 0x183ED8FC8.
// Primary overrides: [1] Init 0x180ED1E28, [2] Deinit 0x182AFE2C0, [4] Update
// 0x180567D70; fills every _purecall slot of the base (own bodies [10..38]:
// 0x180C42C64, 0x181951C7C, 0x1808F2AC4, 0x1808F2AF8, 0x1816E9F70, 0x181F4A960,
// 0x180C41DCC, 0x181F4A850, 0x180C41E00, 0x180C4A620, 0x180C4A4DC, 0x180C41FB0,
// 0x180C42030, 0x181951CA0, 0x1816EA108, 0x1808EFA98, 0x1808F070C, 0x1808F052C,
// 0x180C41EF0, 0x181047178, 0x1808EFA30, 0x182B00E68, 0x182B02090, 0x180C41EE0,
// 0x181817698, 0x1808F0490, 0x182AFFA80, 0x182B00544, 0x180561794).
//
// The Apse character-screen panel. Embedded BY VALUE in C_UIApse @+0x2158 (size pinned
// by next embed C_UIApseCamera @+0x25C0). Its ctor also heap-creates the slots panel
// (sub_18146D050 -> C_UIApseCharacterSlots, stored at base +0xF0).

namespace wh::guimodule {

class C_UIApse;

class C_UIApseCharacter : public C_UICharacterBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCharacter;
    // Implementations of the base's pure slots (per-slot role UNVERIFIED).
    void _vf10() override;   // [10]
    void _vf15() override;   // [15]
    void _vf21() override;   // [21]
    void _vf22() override;   // [22]
    void _vf26() override;   // [26]
    void _vf29() override;   // [29]
    void _vf30() override;   // [30]
    void _vf31() override;   // [31]
    void _vf33() override;   // [33]
    void _vf34() override;   // [34]
    void _vf35() override;   // [35]
    void _vf36() override;   // [36]
    void _vf37() override;   // [37]

    float    m_pitchAngle3F8;    // +0x3F8  camera/model pitch angle; slot31 0x182B00E68 accumulates a2*200 then fminf(fmaxf(x,-20),70)
    uint8_t  _pad3FC[4];         // +0x3FC
    wh::entitymodule::C_Actor* m_pActor400;  // +0x400  displayed actor; slot31 reads it, slot24/25 deref *(+0x668)=IAnimatedCharacter & *(+0x190)=RagdollMgr; reset slot28
    wh::entitymodule::C_Actor* m_pActor408;  // +0x408  displayed actor #2; slot24 truthiness-check, slot25 0x1808EFA98 derefs *(+0x668); reset slot28
    wh::entitymodule::C_Actor* m_pActor410;  // +0x410  displayed actor #3; slot29 0x181047178 returns it as the active render actor; slot24 check; reset slot28
    std::map<wh::entitymodule::C_Actor*, std::vector<void*>> m_actorSlots418;  // +0x418 {_Myhead@0x418,_Mysize@0x420}; 0x40 node: key(C_Actor*)@+0x20, vector@+0x28; insert slot27 0x1808F052C, iterate slot19/26, dtor sub_180BBEF20
    bool     m_flag428;          // +0x428  ctor 1 [role UNVERIFIED]
    uint8_t  _pad429[7];         // +0x429
    C_UIApse* m_pApse;           // +0x430  owner hub (ctor a2)
    wh::entitymodule::C_Actor* m_pActiveActor438;  // +0x438  selected actor = m_actorSlots418 key; slot34 0x181817698 looks it up, getter slot35 0x1808F0490, cleared slot33 0x180C41EE0
    uint64_t m_unk440;           // +0x440  parallel triple w/ +0x448/+0x450, cleared alongside the +0x400/+0x408/+0x410 actor triple @slot28 sub_180C41EF0 [pointee/role UNVERIFIED]
    uint64_t m_unk448;           // +0x448  see m_unk440
    uint64_t m_unk450;           // +0x450  see m_unk440
    uint64_t m_unk458;           // +0x458  8-byte handle/id; ctor & Clear (slot20 sub_180C4A4DC) set it to qword_185326308, a runtime-initialized 'invalid' sentinel global [WUID/id candidate, type UNVERIFIED]
    bool     m_flag460;          // +0x460  ctor 0 [role UNVERIFIED]
    uint8_t  _pad461[7];         // +0x461
};
static_assert(sizeof(C_UIApseCharacter) == 0x468, "C_UIApseCharacter must be 0x468");
static_assert(offsetof(C_UIApseCharacter, m_pApse) == 0x430, "owner at 0x430");

}  // namespace wh::guimodule
