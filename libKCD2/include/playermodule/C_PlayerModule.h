#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"
#include "framework/C_BaseModule.h"
#include "Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener

// -----------------------------------------------
// wh::playermodule::C_PlayerModule -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA0.
// -----------------------------------------------
// RTTI .?AVC_PlayerModule@playermodule@wh@@  primary vtable 0x183F74448 (11 slots)
// ctor sub_180BE9208  scalar-deleting dtor sub_182EA7BF8 (operator delete this,0xA0)
// Stored at S_GameContext+0x128 (VERIFIED, module registration sub_180C9DA44).
//
// Bases (from ctor vtable writes): C_BaseModule/I_ModuleMessageListener @+0x00,
// ISystemEventListener @+0x10. All members from +0x18 are owned sub-managers built
// by the ctor; only the two named ones below have verified identities.
//
// [FUNDAMENTAL vs KCD1] The autosave-request mechanic changed. In KCD1 the module
// stored bool m_autoSaveRequested@+0x108 and the per-frame tick polled/cleared it and
// dispatched C_ModuleMessageSaveGameRequest(saveType=1). In KCD2, Update (slot4,
// sub_180533478) polls no such byte and no method reads +0x98 as a flag; save
// requests are driven from other subsystems and the E_SaveType values shifted (KCD1
// autosave=1; KCD2 callers use 2/6/8). So a bool written here will NOT trigger a save;
// +0x98 below is exposed only as an UNVERIFIED placeholder, not the KCD1 flag.
//
// [MODERATE vs KCD1] m_pFastTravel moved +0xB0 -> +0x28 (still an owned C_FastTravel*).

namespace wh::playermodule {

class C_FastTravel;
class C_ForgeWardrobeManager;   // NEW in KCD2
class C_MinigameManager;
class C_TutorialManager;
class C_RandomEventManager;
class C_FOWManager;
class C_Keybinds;
class C_ActionHintManager;
class C_ActionSets;
class I_ActionSets;
class C_PlayerManager;
class C_ShelverManager;
class C_ControllerFeedbackManager;

class C_PlayerModule
    : public wh::framework::C_BaseModule
    , public Offsets::ISystemEventListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerModule;

    // C_BaseModule contributes slots [0..6] only (7-slot base vtable 0x183A83EA8);
    // the 4 extra slots in the 11-slot C_PlayerModule vtable are its OWN:
    virtual void _vf7();                                     // [7]  0x1809DD2D0  role unresolved
    // RTTR trio [8..10]: get_type 0x182EB7BB0, get_ptr 0x1805F5DA0 (`return this`),
    // get_derived_info 0x182EB7954. Registered BASELESS.
    RTTR_ENABLE()

    // C_BaseModule base occupies +0x00 (vtable) / +0x08 (m_state) / +0x0C (pad);
    // ISystemEventListener base occupies +0x10 (vtable). Members follow at +0x18.
    C_MinigameManager*      m_pMinigameManager; // +0x18  (0xB0,  ctor sub_180EF7184) sessions + minigame scriptbinds
    C_TutorialManager*      m_pTutorialManager; // +0x20  (0x50,  ctor sub_18179FD3C stores C_TutorialManager vtable)
    C_FastTravel*           m_pFastTravel;      // +0x28  (0x130, ctor sub_180BE9BA0)            VERIFIED
    C_RandomEventManager*   m_pRandomEventManager; // +0x30  (0x118, ctor sub_180BE83E8 stores C_RandomEventManager vtable)
    C_FOWManager*           m_pFOWManager;      // +0x38  (0xA8,  ctor sub_180BE9FB4 stores C_FOWManager vtable; embeds C_NullFogOfWar)
    C_Keybinds*             m_pKeybinds;        // +0x40  (0x1F0, ctor sub_18124417C stores C_Keybinds vtable)
    void*                   m_sub48;            // +0x48  sub-manager (0x20,  intrusive list node)
    void*                   m_sub50;            // +0x50  sub-manager (0x50,  C_Signal-like)
    C_ActionHintManager*    m_pActionHintManager; // +0x58  (0x140, ctor sub_181748CFC stores C_ActionHintManager vtable)
    I_ActionSets*           m_pActionSets;      // +0x60  = C_ActionSets+8, the I_ActionSets subobject
                                                //        (obj 0x90, ctor sub_180BE9E68; VERIFIED disasm 0x180BE93DA)
    void*                   m_sub68;            // +0x68  sub-manager (0x20,  C_Signal-like)
    void*                   m_sub70;            // +0x70  sub-manager (0x10,  single-node list)
    C_PlayerManager*        m_pPlayerManager;   // +0x78  (0xD8,  ctor sub_180BE9868 stores C_PlayerManager vtable; registers name "C_PlayerManager")
    C_ShelverManager*       m_pShelverManager;  // +0x80  (0x88,  ctor sub_180794D8C stores C_ShelverManager vtable)
    C_ControllerFeedbackManager* m_pControllerFeedbackManager; // +0x88  (0x58,  ctor sub_180C3D1CC stores C_ControllerFeedbackManager vtable)
    C_ForgeWardrobeManager* m_pForgeWardrobe;   // +0x90  (0x20)  NEW in KCD2                    VERIFIED
    uint64_t                m_98;               // +0x98  zero-init, no verified reader; NOT the KCD1 autosave flag (UNVERIFIED)
};
static_assert(sizeof(C_PlayerModule) == 0xA0, "C_PlayerModule must be 0xA0");

}  // namespace wh::playermodule
