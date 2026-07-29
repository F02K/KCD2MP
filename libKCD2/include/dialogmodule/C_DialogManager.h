#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "C_DialogueDataManager.h"
#include "../guimodule/I_InteractiveScenePlayer.h"

struct ICVar;

// -----------------------------------------------
// wh::dialogmodule::C_DialogManager -- the dialog-system workhorse (KCD2 1.5.6, kd7u).
// sizeof 0x1F0 (alloc 496 in C_DialogModule::Init; stored at module +0x10).
// -----------------------------------------------
// RTTI TD 0x184B4DF18. Base: guimodule::I_InteractiveScenePlayer (mdisp 0, shared
// primary vtable 0x183E64C90, 9 slots -- exactly the interface's dtor+8, NO rttr trio,
// NO added slots). ctor sub_180E2FEFC; dtor sub_182803130. Slot notes:
//   [1] 0x18134F7BC  per-frame request pump (drains the +0xF8..+0x110 ring buffer)
//   [2] 0x182804650  list op on +0xF0
//   [3] 0x18041A6A0  return 1 (IsReady/CanUpdate)
//   [4] 0x180622D04  update tick (pumps subsystem + dialog runtime qword_185494490)
//   [5]/[6]/[8] roles UNVERIFIED; [7] identity thunk
// CVar registration sub_180E3DE3C wires ~50 wh_dlg_* CVars; only the fields flagged
// below bind to manager storage -- the rest are free CVars (see dossier section 6a).
// CHEAT levers: wh_dlg_AutoSkip, wh_dlg_SkipCooldown, wh_dlg_ForcedDecision (force a
// branch), wh_dlg_Enable (+0x78), request geometry (+0x1B0..+0x1BC), and the Lua
// "DialogModule" table (m_pScriptBind, +0x128).

namespace wh::dialogmodule {

class C_ScriptBindDialog;
class C_DialogCameraManager;

class C_DialogManager : public guimodule::I_InteractiveScenePlayer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogManager;
    // 6 message/mailbox subscription slots (stride 0x10): 4x desc &unk_185666CF8
    // (+0x08..+0x47), 2x desc &unk_185666F50 (+0x48..+0x67). Registered via
    // sub_1803B9FA4/sub_180BC5BD4, unsubscribed in the dtor [element type UNVERIFIED].
    struct S_MsgSubscription {
        const void* m_pDesc;    // +0x00  wh/Cry DynArray head (COW SmallDynStorage: {int count@-8, int cap@-4}; empty=shared static &unk_185666CF8 / &unk_185666F50; CF8-group elems 0x10-stride, grow sub_180396724)
        int16_t     m_a;        // ctor -1
        int16_t     m_b;        // ctor -1
        uint8_t     _pad[4];
    };

    S_MsgSubscription m_subscriptions[6];   // +0x08..+0x67
    data::C_DialogueDataManager* m_pDataManager;   // +0x68  owned 0x158 obj (factory
                                            //         sub_180E2FDE4; shared_ptr claim UNVERIFIED)
    void*    m_pObj70;                      // +0x70  owned 0x18 struct {ICVar* wh_dlg_VoiceRoot @+8; ICVar* wh_dlg_DBARoot @+16} (ctor sub_180E2FD44 REGISTER_STRING; dtor sub_18284E774 unregisters, then frees)
    int32_t  m_enable;                      // +0x78  ctor 1  (CVar wh_dlg_Enable)      <-- CHEAT
    int32_t  m_testMode;                    // +0x7C  ctor 0  (CVar wh_dlg_TestMode)    <-- CHEAT
    void*    m_treeHead80;                  // +0x80  MSVC std::_Tree head sentinel (map/set); sub_1803F7FAC allocs 40B node {_Left/_Parent/_Right=self, _Color=1,_Isnil=1 => word 0x0101}; 8B value_type UNRESOLVED
    size_t   m_treeSize88;                  // +0x88  std::_Tree node count (ctor 0; dtor sub_1803E6A3C recursively erases: _Isnil@byte+25, _Left=*i, _Right=i[2], frees 40B nodes)
    void*    m_pActiveSession;              // +0x90  borrowed ptr to the active dialog/scene session (slot[5] 0x1828043EC validates via sub_18134FA2C & reads +0xE8; sub_18134F8E0 sets flag@+933) -- NOT owned (no dtor release)
    void*    m_ref98;                       // +0x98  OWNED refcounted obj (_smart_ptr semantics: dtor 0x18280330F Releases via strong@+8/weak@+12, vtbl[0] destroy + vtbl[1] free); pointee class UNRESOLVED
    uint64_t _qA0;                          // +0xA0  zero-init [role UNVERIFIED]
    void*    m_refA8;                       // +0xA8  OWNED refcounted obj (_smart_ptr semantics: dtor 0x1828032CB Releases via strong@+8/weak@+12); pointee class UNRESOLVED
    uint64_t _qB0;                          // +0xB0  zero-init [role UNVERIFIED]
    uint64_t _qB8;                          // +0xB8  zero-init [role UNVERIFIED]
    std::vector<void*> m_activeA;           // +0xC0  broadcast target (sub_181141BF0;
                                            //         elements are 0x10 records [type UNVERIFIED])
    std::vector<void*> m_activeB;           // +0xD8  2nd broadcast target (0x10 records)
    void*    m_pRequestQueue;               // +0xF0  WH intrusive-list sentinel head (16B node via sub_182432D14, node[0]=&this+0xF0 self-ref, node[1]=0); dtor sub_182803080 frees the 16B node -- a list head, NOT a map (same pattern as C_PositioningManager m_listHead48)
    uint64_t m_ringF8[4];                   // +0xF8..+0x117  request ring-buffer bookkeeping
                                            //         (buffer ptr / mask / head / count) used by slot [1]
    uint64_t _q118;                         // +0x118  ctor 0
    uint64_t _q120;                         // +0x120  ctor 0
    C_ScriptBindDialog* m_pScriptBind;      // +0x128  owned 0x78 obj (ctor sub_180E30ACC)
    uint8_t  m_obj130[0x48];                // +0x130..+0x177  sub-object (sub_180E30120
                                            //         hash/map init) [layout UNVERIFIED]
    uint64_t m_list178[2];                  // +0x178  std::list head (sub_180E3014C, alloc-256
                                            //         sentinel) [type UNVERIFIED]
    C_DialogCameraManager* m_pCameraMgr;    // +0x188  owned 0xD0 obj (ctor sub_1816F365C;
                                            //         set in CVar-reg sub_180E3DE3C)
    uint64_t m_cont190[3];                  // +0x190..+0x1A7  container (unknown_libname_15;
                                            //         std map/list) [type UNVERIFIED]
    uint64_t _q1A8;                         // +0x1A8  ctor 0
    float    m_requestMaxDistance;          // +0x1B0  CVar wh_dlg_RequestMaxDistance    <-- CHEAT
    int32_t  m_requestMaxAxisAngle;         // +0x1B4  ctor 45 (wh_dlg_RequestMaxAxisAngle)
    int32_t  m_requestMaxTotalAngle;        // +0x1B8  ctor 90 (wh_dlg_RequestMaxTotalAngle)
    float    m_requestTimeout;              // +0x1BC  CVar wh_dlg_RequestTimeout
    int32_t  m_d1C0;                        // +0x1C0  CVar-bound (sub_1828007A4 target) [name UNVERIFIED]
    int32_t  m_recordDialog;                // +0x1C4  ctor 0 (wh_dlg_RecordDialog)
    int32_t  m_dialogDebug;                 // +0x1C8  ctor 0 (wh_dlg_DialogDebug)
    int32_t  m_actorSoundDebug;             // +0x1CC  ctor 0 (wh_dlg_ActorSoundDebug)
    uint64_t _q1D0;                         // +0x1D0  [not walked]
    ICVar*   m_pTestPreselectedSeq;         // +0x1D8  wh_dlg_TestPreselectedSequences (string)
    uint64_t m_cont1E0[2];                  // +0x1E0  std container (sub_1809ED074, alloc-64
                                            //         sentinel) [type UNVERIFIED]
};
static_assert(sizeof(C_DialogManager) == 0x1F0, "C_DialogManager must be 0x1F0 (alloc 496)");
static_assert(offsetof(C_DialogManager, m_pDataManager) == 0x68);
static_assert(offsetof(C_DialogManager, m_enable) == 0x78);
static_assert(offsetof(C_DialogManager, m_activeA) == 0xC0);
static_assert(offsetof(C_DialogManager, m_pScriptBind) == 0x128);
static_assert(offsetof(C_DialogManager, m_pCameraMgr) == 0x188);
static_assert(offsetof(C_DialogManager, m_requestMaxDistance) == 0x1B0);

}  // namespace wh::dialogmodule
