#pragma once
#include <cstdint>
#include <cstddef>
#include "I_Dialog.h"
#include "I_TestDialog.h"
#include "I_DialogCommandReceiver.h"
#include "I_DialogInteractionCallback.h"
#include "C_PlayerDialogController.h"
#include "S_DialogParams.h"
#include "../entitymodule/I_ActorSpeakCallback.h"
#include "../xgenaimodule/I_IntermissionInvoker.h"

struct ICVar;

// -----------------------------------------------
// wh::dialogmodule::C_DialogInstance -- THE live conversation object (KCD2 1.5.6, kd7u).
// sizeof 0x448 (std::_Ref_count_obj2 alloc 0x458; object at ctrlblock+0x10).
// -----------------------------------------------
// RTTI TD 0x184B4B8E8; 6-way multiple inheritance (CHDattr = 1); ctor sub_1808DCC08;
// Init/Setup sub_180D577A8; dtor-body sub_1808D9FF8; primary deleting-dtor
// sub_181534F6C. Subobject vtables:
//   +0x00 C_DialogInstance + entitymodule::I_ActorSpeakCallback  0x183E63A40 (4)
//   +0x08 I_DialogInteractionCallback                            0x183E63AF0 (1)
//   +0x10 I_DialogCommandReceiver                                0x183E63B00 (11)
//   +0x18 I_Dialog                                               0x183E63B60 (37)
//   +0x20 xgenaimodule::I_IntermissionInvoker                    0x183E63C90 (2)
//   +0x28 I_TestDialog                                           0x183E63A68 (16)
// Interior regions between the pinned fields are unwalked (zero-init runtime state).
// CHEAT: +0x370/+0x378 cache the wh_dlg_AutoSkip / wh_dlg_SkipCooldown ICVars (dialog
// auto-skip); programmatic skip via the data::S_FastForwardRequest* message family.

namespace wh::dialogmodule {

class C_DialogInstance
    : public entitymodule::I_ActorSpeakCallback,     // +0x00 (primary)
      public I_DialogInteractionCallback,            // +0x08
      public I_DialogCommandReceiver,                // +0x10
      public I_Dialog,                               // +0x18
      public xgenaimodule::I_IntermissionInvoker,    // +0x20
      public I_TestDialog                            // +0x28
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogInstance;
    uint8_t  _unk30[0x28];            // +0x030..+0x057  0x28B obj (ctor sub_1808DCBEC / dtor sub_1808DB258)
    S_DialogParams m_params;          // +0x058  embedded copy; copy-ctor sub_18093CBD4 stores S_DialogParams vftable (0xE8)
    uint8_t  _unk140[0x18];           // +0x140  std::vector<16B elem> (dtor sub_181E62E50; elem dtor sub_18086F8D0 on elem+8)
    void*    m_pScene;                // +0x158  ->232B non-poly scene runtime; Init sub_180D577A8 allocs 0xE8, builds
                                      //         sub_180D58108 (std::vector<32B> actors @+0, std::vector<8B> @+72, flags dword @+188)
    uint8_t  _unk160[0x118];          // +0x160..+0x277  [not walked]
    int32_t  m_phase;                 // +0x278  Init sets 23 on abort
    uint8_t  _pad27C[4];              // +0x27C
    void*    m_pParticipant;          // +0x280  ctor arg a6 (= *(dialogMgr+0x280)); non-null + I_Dialog vf gate m_pPlayerCtl create in Init sub_180D577A8
    uint8_t  _unk288[0x14];           // +0x288..+0x29B  [not walked]
    uint16_t m_stateWord;             // +0x29C  reset to 256 by Haggle-OnEnd + interaction cb
    uint8_t  _unk29E[0x4A];           // +0x29E..+0x2E7  [not walked]
    C_PlayerDialogController* m_pPlayerCtl;   // +0x2E8  owned; created in Init if player
                                              //         participates; freed FIRST in dtor
    int32_t  m_d2F0;                  // +0x2F0  ctor 19
    uint8_t  _unk2F4[0x64];           // +0x2F4..+0x357  [not walked]
    ICVar*   m_pCVarVisibleSequence;  // +0x358  wh_dlg_VisibleSequence
    ICVar*   m_pCVarNoCameras;        // +0x360  wh_dlg_NoCameras
    ICVar*   m_pCVarLODRatio;         // +0x368  wh_dlg_LODRatio
    ICVar*   m_pCVarAutoSkip;         // +0x370  wh_dlg_AutoSkip      <-- CHEAT
    ICVar*   m_pCVarSkipCooldown;     // +0x378  wh_dlg_SkipCooldown  <-- CHEAT
    uint8_t  _unk380[0xC8];           // +0x380..+0x447  [not walked]
};
static_assert(sizeof(C_DialogInstance) == 0x448, "C_DialogInstance must be 0x448 (ref-count obj alloc 0x458)");
static_assert(offsetof(C_DialogInstance, m_pScene) == 0x158);
static_assert(offsetof(C_DialogInstance, m_phase) == 0x278);
static_assert(offsetof(C_DialogInstance, m_stateWord) == 0x29C);
static_assert(offsetof(C_DialogInstance, m_pPlayerCtl) == 0x2E8);
static_assert(offsetof(C_DialogInstance, m_pCVarAutoSkip) == 0x370);

}  // namespace wh::dialogmodule
