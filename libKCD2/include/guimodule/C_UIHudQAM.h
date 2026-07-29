#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudQAM -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI .?AVC_UIHudQAM@guimodule@wh@@ (TD 0x184CD0B38).
// Bases: C_UIFlashBase @+0x00 (COL 0x4376810), IUIElementEventListener @+0x10
// (COL 0x4376838) -- no extra interface bases.
// vtables: primary 0x183EEB720 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183EEB6C8 (8). ctor sub_18188A4CC; rttr as_std_shared_ptr creator
// sub_18188A3AC (alloc 0xB0).
//
// Quick-access-menu HUD strip (weapon/food slots on the "hud" movie). Init [1]
// sub_180D42EF8: base Init + a large RegisterDispatcher block (per-signature inbound
// handlers; names not enumerated this wave -- G4). OnModuleMessage [3] sub_18085DD68:
// msgs 63/77 -> sub_181925B4C, msgs 52/65 -> sub_181579C88. Update [4] sub_180533BF4:
// while m_bCountdownActive, m_countdown -= dt; at <= 0 calls sub_1808EA744 (auto-hide).
// The class also binds an actor-action-finished callback (std::_Binder TD
// void(C_UIHudQAM::*)(E_ActorActionFinishReason::Type) at 0x1849B92F0).
// C_Signal argument signatures + slot-id member roles UNVERIFIED.

namespace wh::guimodule {

class C_UIHudQAM : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudQAM;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180D42EF8
    void OnModuleMessage(void* msg) override;   // [3] 0x18085DD68
    void Update() override;                     // [4] 0x180533BF4  (binary passes dt in xmm1; C_UIBase decl has no param -- see NOTE)
    // NOTE: the binary Update body reads a float delta argument; C_UIBase::Update()
    // is declared parameterless from the module broadcast. UNRESOLVED signature
    // mismatch inherited from the G2 spine -- do not call through this slot with args.

    wh::shared::C_Signal<> m_signal58;   // +0x58  (sentinel unk_185665430) [signature UNVERIFIED]
    wh::shared::C_Signal<> m_signal68;   // +0x68  (sentinel unk_185665430) [signature UNVERIFIED]
    uint64_t m_78;                       // +0x78  ctor 0 [role UNVERIFIED]
    uint64_t m_80;                       // +0x80  ctor 0 [role UNVERIFIED]
    CryStringT<char> m_str88;            // +0x88  ctor empty [role UNVERIFIED]
    float    m_countdown;                // +0x90  Update decrement (@0x180533c10)
    int32_t  m_slot94;                   // +0x94  ctor -1 [role UNVERIFIED]
    int32_t  m_slot98;                   // +0x98  ctor -1 [role UNVERIFIED]
    int32_t  m_slot9C;                   // +0x9C  ctor -1 [role UNVERIFIED]
    int32_t  m_slotA0;                   // +0xA0  ctor -1 [role UNVERIFIED]
    int32_t  m_slotA4;                   // +0xA4  ctor -1 [role UNVERIFIED]
    bool     m_flagA8;                   // +0xA8  ctor 0 [role UNVERIFIED]
    bool     m_bCountdownActive;         // +0xA9  Update gate (@0x180533bf4)
    uint8_t  _padAA[6];                  // +0xAA
};
static_assert(sizeof(C_UIHudQAM) == 0xB0, "C_UIHudQAM must be 0xB0");
static_assert(offsetof(C_UIHudQAM, m_countdown) == 0x90, "countdown at 0x90");

}  // namespace wh::guimodule
