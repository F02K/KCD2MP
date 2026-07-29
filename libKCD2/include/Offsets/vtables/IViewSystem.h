#pragma once
#include <cstdint>
#include "../../CryEngine/CryCommon/Cry_Math.h"   // Quat

// -----------------------------------------------
// IViewSystem -- CryEngine view-system interface, KCD2 binary vtable order (impl CViewSystem).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  CViewSystem bases: IViewSystem @+0x00 (vtable 0x18405D6C8,
// 24 slots -- modeled here), IMovieUser @+0x08 (0x18405D7E8: [1] SetActiveCamera 0x1807EEE70,
// [2] BeginCutScene 0x183724500, [3] EndCutScene 0x18372788C), ILevelSystemListener @+0x10
// (0x18405D790: [2] OnLoadingStart 0x180DA8168, [9] OnUnloadComplete 0x180FC312C; rest nullsubs).
// Evidence dumps in KCD2/analysis/vtables_reslot/ (also a full 19-slot CView map in the .md).
//
// [vs KCD1] KCD1's header was a placeholder (only IsPlayingCutScene slot 19 known). KCD2 map is
// full ground truth; the one shared anchor (IsPlayingCutScene = slot 19, cutscene counter field)
// did NOT move. Slots flagged LIKELY: [8], [14], [15].

namespace Offsets {

struct IView;
struct IViewSystemListener;

struct IViewSystem {
    virtual ~IViewSystem() {}                                                // [0]  0x00  deleting dtor (0x183722E7C)
    virtual IView*   CreateView() = 0;                                       // [1]  0x08  0x180B4BE24 -- alloc 0x438 CView, id map insert
    virtual void     RemoveViewById(uint32_t viewId) = 0;                    // [2]  0x10  0x183731C78 -- lookup, Release, erase
    virtual void     RemoveView(IView* pView) = 0;                           // [3]  0x18  0x183731C80 -- GetViewId then common impl
    virtual void     SetActiveViewById(uint32_t viewId) = 0;                 // [4]  0x20  0x183733C38 -- old->SetActive(0), new->SetActive(1)
    virtual void     SetActiveView(IView* pView) = 0;                        // [5]  0x28  0x180B4C200 -- null -> -1
    virtual IView*   GetView(uint32_t viewId) = 0;                           // [6]  0x30  0x1807F0A90 -- id map lookup
    virtual IView*   GetActiveView() = 0;                                    // [7]  0x38  0x1807F0A84 -- GetView(m_activeViewId)
    virtual bool     IsClientActorViewActive() = 0;                          // [8]  0x40  0x1807EF0A4 -- active->GetLinkedId == clientActorId [LIKELY]
    virtual uint32_t GetViewId(IView* pView) = 0;                            // [9]  0x48  0x1807EEE34 -- reverse map walk
    virtual uint32_t GetActiveViewId() = 0;                                  // [10] 0x50  0x1837286E0 -- cutscene view id if in cutscene
    virtual IView*   GetViewByEntityId(uint32_t entityId, bool force) = 0;   // [11] 0x58  0x18372A624 -- linked-id scan; miss+force -> CreateView+LinkTo
    virtual bool     AddListener(IViewSystemListener* p) = 0;                // [12] 0x60  0x18100F038
    virtual bool     RemoveListener(IViewSystemListener* p) = 0;             // [13] 0x68  0x18100DBF4
    virtual void     Serialize(void* ser) = 0;                               // [14] 0x70  0x1837336C8 -- reading -> per-view ResetShaking [name LIKELY]
    virtual void     PostSerialize() = 0;                                    // [15] 0x78  0x18372F1BC -- per-view CreateAudioListener [LIKELY]
    virtual float    GetDefaultZNear() = 0;                                  // [16] 0x80  0x181AA5650 -- float +144
    virtual void     SetBlendParams(float a, float b, bool c) = 0;           // [17] 0x88  0x181AAFA40 -- +148/+152/+156
    virtual void     SetOverrideCameraRotation(bool bOverride, Quat rot) = 0;// [18] 0x90  0x181AAFA80 -- +117/+120
    virtual bool     IsPlayingCutScene() const = 0;                          // [19] 0x98  0x180B4C1BC -- cutsceneCount(+112) != 0  [KCD1 slot 19 too]
    virtual void     UpdateSoundListeners() = 0;                             // [20] 0xA0  0x183735B00 -- +165 gate; active view gets GetViewCamera
    virtual void     SetDeferredViewSystemUpdate(bool b) = 0;                // [21] 0xA8  0x180BC69B0 -- byte +164
    virtual bool     UseDeferredViewSystemUpdate() = 0;                      // [22] 0xB0  0x1809B7760 -- byte +164
    virtual void     SetControlAudioListeners(bool b) = 0;                   // [23] 0xB8  0x183733D78 -- +165; per-view SetActive(arg)
};

}  // namespace Offsets
