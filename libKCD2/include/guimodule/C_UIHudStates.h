#pragma once
#include <cstdint>
#include <map>
#include "guimodule/C_UIFlashBase.h"
#include "entitymodule/I_EquipmentManagerListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudStates -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_UIHudStates@guimodule@wh@@ (TD 0x184CD0948).
// Bases: C_UIFlashBase @+0x00 (COL 0x41360F8), IUIElementEventListener @+0x10
// (COL 0x4136120), entitymodule::I_EquipmentManagerListener @+0x58 (COL 0x4136148).
// vtables: primary 0x183C2AD50 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C2AD08 (8), equipment 0x183C2ACE0 (4: [0] dtor thunk 0x18213A4DC,
// [1] 0x18172ED4C, [2] 0x1816AFB50, [3] nullsub). ctor sub_1818503F4; rttr
// as_std_shared_ptr creator sub_181850368 (alloc 0x80).
//
// Player-state HUD block (stats bars / equipped-item states on the "hud" movie).
// Init [1] sub_180C3CD30: base Init + registers {this, sub_182B8D504} with the
// ctx+0x18 mode manager (vf[+0x118] list). OnModuleMessage [3] sub_18085DE2C: msg 52
// re-reads the mode (sub_1819BD44C) + the player-object state (*(ctx+8)->vf[+0x200]()
// -> +0x668 obj vf[+0x148] -> sub_1819BD490/sub_180EC9684). OnGameStart [5]
// sub_180EC9208: hooks {this, sub_180EC9874} @game+0xB38, three delegates on the
// equipment hub (*(ctx+0x160)+0xA8 vf[+0x1A8] -> vf[+0x20] obj: vf[+0x18]/[+0x28]/
// [+0x38] with sub_180EC9730/sub_180EC9920/sub_180EC9728), registers the
// I_EquipmentManagerListener subobject via sub_1823CF1A0(sub_1808F7D8C(game),
// this+0x58) and sets m_bListening. OnGameEnd [6] sub_180EC7E8C mirrors.
// Equipment callbacks [1]/[2] resolve the item by WUID (*(ctx+0xE0)+0xC8 map +0x168),
// filter item flag 0x1A and track per-item records in m_equippedItems.

namespace wh::guimodule {

class C_UIHudStates : public C_UIFlashBase, public wh::entitymodule::I_EquipmentManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudStates;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180C3CD30
    void OnModuleMessage(void* msg) override;   // [3] 0x18085DE2C
    void OnGameStart(void* pGame) override;     // [5] 0x180EC9208
    void OnGameEnd(void* pGame) override;       // [6] 0x180EC7E8C
    // I_EquipmentManagerListener overrides.
    void OnItemEquipped(void* a2, uint64_t itemWuid) override;    // [E1] 0x18172ED4C
    void OnItemUnequipped(void* a2, uint64_t itemWuid) override;  // [E2] 0x1816AFB50

    std::map<uint64_t, void*> m_equippedItems;  // +0x60  WUID -> per-item state record (insert sub_1814A2890/sub_1808F63A4; value type UNVERIFIED)
    bool     m_bListening;                      // +0x70  set 1 by OnGameStart (@0x180ec9312)
    uint8_t  _pad71[3];                         // +0x71
    int32_t  m_74;                              // +0x74  ctor 0 [role UNVERIFIED]
    int32_t  m_78;                              // +0x78  ctor 0 [role UNVERIFIED]
    uint8_t  _pad7C[4];                         // +0x7C
};
static_assert(sizeof(C_UIHudStates) == 0x80, "C_UIHudStates must be 0x80");
static_assert(offsetof(C_UIHudStates, m_equippedItems) == 0x60, "equipped-item map at 0x60");

}  // namespace wh::guimodule
