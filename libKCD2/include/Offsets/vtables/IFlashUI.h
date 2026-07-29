#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/smartptr.h"   // _smart_ptr<T> (CryEngine intrusive smart pointer; AddRef=vf[1]/Release=vf[2])

// -----------------------------------------------
// IFlashUI -- KCD2 binary vtable order (REORDERED vs KCD1)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashUI.h
// (struct IFlashUI : public ICryUnknown). gEnv+0x140 holds the IFlashUI* (the
// CFlashUI singleton -- see crysystem/SSystemGlobalEnvironment.h; KCD1 was +0x130).
//
// Impl: CFlashUI (.?AVCFlashUI@@), primary vtable @ 0x18404A7D8, 49 slots (KCD1: 43).
// ctor sub_1816F27F0 (obj 472 bytes / 0x1D8); factory create sub_1816F264C
// (CSingletonFactory<CFlashUI>, singleton getter sub_180798810).
// RTTI: CFlashUI : IFlashUI(+0) : ICryUnknown, IHardwareMouseEventListener (+0x08),
//   IInputEventListener (+0x10), IGameFrameworkListener (+0x18),
//   ILevelSystemListener (+0x20), ISystemEventListener (+0x28),
//   ILoadtimeCallback (+0x30).
//
// [FUNDAMENTAL vs KCD1] KCD2 REORDERS the vtable -- the KCD1 assumption "IFlashUI is
// not interfuscated, slot == declaration order" is FALSE for KCD2. Every slot below
// was matched BEHAVIORALLY (member offsets, alloc sizes, child-vtable dispatch, arg
// types), NOT by position. Notable moves vs KCD1:
//   * The ICryUnknown quartet is reordered: [0]=QueryInterface (GUID compare),
//     [1]=QueryComposite (ret 0), [2]=GetFactory (ret &singleton), [3]=~dtor
//     (KCD1 was [0]dtor,[1]GetFactory,[2]QI,[3]QC).
//   * GetUIActionEvents moved from KCD1 [6] to the LAST slot [48].
//   * KCD2 inserts new element-enumeration/instance-string slots ([16][17][19][20])
//     between the GetUIElement family and GetUIAction family (see IUIElement.h for
//     the IUIElementIterator replacement on the element side).
// KCD2 strips FlashUI debug-log strings, so anchors are structural: [30]
// CreateEventSystem pool-allocs a 0x58-byte CFlashUIEventSystem; [11]
// LoadElementsFromFile pool-allocs a 0x250-byte CFlashUIElement; [10] Shutdown zeroes
// gEnv->pFlashUI (qword_18492D938); [40] GetMemoryStatistics Adds "FlashUI"/"Elements"
// /"Actions"/"EventSystems".
//
// OVERLOAD PAIRS (both const-char* HIGHER here; declare the const-char* overload FIRST
// so MSVC's adjacent-overload reversal lands each on its binary slot -- the proven
// GetUIElement crash-class fix):
//   GetUIElement(int) [13] / GetUIElement(const char*) [14]
//   GetUIAction(int)  [21] / GetUIAction(const char*)  [22]

namespace Offsets {

struct ICryFactory;
struct IUIElement;
struct IUIAction;
struct IUIActionManager;
struct IUIModule;
struct IUIEventSystem;
struct IUIEventSystemIterator;
struct IVirtualKeyboardEvents;
struct ITexture;
struct ICrySizer;

struct IFlashUI {
    // --- ICryUnknown quartet (REORDERED vs KCD1) ---
    virtual void* QueryInterface(const void* iid) const = 0;                     // [0]  0x00  0x180798F54  GUID compare vs ICryUnknown/IFlashUI iids, returns this on match (const CryInterfaceID&)
    virtual void* QueryComposite(const char* name) const = 0;                    // [1]  0x08  0x18066CD10  returns 0
    virtual ICryFactory* GetFactory() const = 0;                                 // [2]  0x10  0x181AAEFE0  VERIFIED: returns &CSingletonFactory<CFlashUI> (qword_185350150)
    virtual void Dtor(char flags) = 0;                                           // [3]  0x18  0x183595374  scalar deleting dtor (sub_1835939E8 + free)

    // --- IFlashUI ---
    virtual void Init() = 0;                                                     // [4]  0x20  0x1819A6FA8  VERIFIED: pool-allocs the CFlashUIActionEvents member (ctor sub_180604578) into this+0x38
    virtual bool PostInit() = 0;                                                 // [5]  0x28  0x1817EBD98  VERIFIED: ANDs child Init() over elements/listeners, returns bool
    virtual void Update(float fDeltatime) = 0;                                   // [6]  0x30  0x18085D614  VERIFIED: per-frame update loop over the element/listener set (child vf+0x28)
    virtual void Reload() = 0;                                                   // [7]  0x38  0x1835BD0CC  tears down elements/actions (sub_1835BD288/sub_1835BE608) + re-dispatches system event 61 [conf MED]
    virtual void _vf8() = 0;                                                     // [8]  0x40  0x1835BE4A8  UNVERIFIED: partial unload helper (sub_1835BE608 + sub_1835BD7C0); KCD2-added, do not call
    virtual void _vf9() = 0;                                                     // [9]  0x48  0x183598AB0  UNVERIFIED: thunk -> sub_1835982F0 (releases the actions vector this+0x80); KCD2-added, do not call
    virtual void Shutdown() = 0;                                                 // [10] 0x50  0x1835C0FB0  VERIFIED: full teardown; zeroes gEnv->pFlashUI (qword_18492D938 = 0)

    virtual bool LoadElementsFromFile(const char* fileName) = 0;                 // [11] 0x58  0x180563AE0  VERIFIED: parses element XML ("name" attr), pool-allocs 0x250-byte CFlashUIElement (ctor sub_18088DF88), SetName + inserts into elements map this+0x60
    virtual bool LoadActionFromFile(const char* sFileName, int type) = 0;        // [12] 0x60  0x1835B1D4C  VERIFIED: dispatch by IUIAction::EUIActionType (0=FlowGraph sub_18173E670, 1=LuaScript sub_1835B22D0)

    // access for IUIElements (elements map/vector at this+0x60). OVERLOAD PAIR:
    // const char* declared FIRST -> lands at the HIGHER binary slot [14].
    virtual _smart_ptr<IUIElement>& GetUIElement(_smart_ptr<IUIElement>& out, const char* name) const = 0;  // [14] 0x70  0x1807F4DF0  name-keyed lookup on elements map this+0x60 (sub_1807F4E48)
    virtual _smart_ptr<IUIElement>& GetUIElement(_smart_ptr<IUIElement>& out, int index) const = 0;         // [13] 0x68  0x18056406C  bounds-checked index into elements array this+0x60 (stride 0x10)
    virtual int GetUIElementCount() const = 0;                                   // [15] 0x78  0x180564170  VERIFIED: count of elements array this+0x60

    virtual void* ForEachElement(void* pFunctor) = 0;                            // [16] 0x80  0x18085E790  KCD2-added: visitor over elements this+0x60, invokes std::function<bool(IUIElement*)> until it returns false [conf MED, name coined]
    virtual void _vf17() = 0;                                                    // [17] 0x88  0x18085E734  UNVERIFIED: builds a predicate and calls [16]; KCD2-added element-find helper
    virtual IUIElement* GetUIElementByInstanceStr(const char* UIInstanceStr) = 0;// [18] 0x90  0x18073D924  VERIFIED: parses "name@index", resolves element by name then instance by index (sub_18073D940/sub_18073D9E4)
    virtual void _vf19() = 0;                                                    // [19] 0x98  0x18073D940  UNVERIFIED: instance-string resolver used by [18] (returns {element, instance})
    virtual void _vf20() = 0;                                                    // [20] 0xA0  0x18073D9E4  UNVERIFIED: "name@index" parser used by [19]

    // access for IUIActions (actions map/vector at this+0x80). OVERLOAD PAIR:
    // const char* declared FIRST -> HIGHER binary slot [22].
    virtual IUIAction* GetUIAction(const char* name) const = 0;                  // [22] 0xB0  0x1823DBD90  VERIFIED: name-keyed lookup on actions map this+0x88, returns action from this+0x80 array
    virtual IUIAction* GetUIAction(int index) const = 0;                         // [21] 0xA8  0x18060FFCC  VERIFIED: bounds-checked index into actions array this+0x80
    virtual int GetUIActionCount() const = 0;                                    // [23] 0xB8  0x181AAF040  VERIFIED: count of actions array this+0x80

    virtual IUIActionManager* GetUIActionManager() const = 0;                    // [24] 0xC0  0x181AAF050  VERIFIED: returns m_pFlashUIActionManager (this+0xD8)
    virtual void UpdateFG() = 0;                                                 // [25] 0xC8  0x18060FF00  VERIFIED: reentrancy-guarded (this+0x1D0) flowgraph update over actions/manager
    virtual void RegisterModule(IUIModule* pModule, const char* name) = 0;       // [26] 0xD0  0x18073D31C  VERIFIED: dedup-append into the module list this+0xE0
    virtual void UnregisterModule(IUIModule* pModule) = 0;                       // [27] 0xD8  0x18073F2C0  VERIFIED: remove from this+0xE0 (deferred if iterating)
    virtual void SetHudElementsVisible(bool bVisible) = 0;                       // [28] 0xE0  0x1835C021C  VERIFIED: gated by CVar "hud_hide", writes hud-visible byte this+0x17C
    virtual void EnableEventStack(bool bEnable) = 0;                             // [29] 0xE8  0x1803B6E80  nullsub in KCD2 (empty) [conf MED by position]

    // event systems
    virtual IUIEventSystem* CreateEventSystem(const char* name, int eType) = 0;  // [30] 0xF0  0x180605404  VERIFIED: GetEventSystem [31] first, else pool-allocs 0x58-byte CFlashUIEventSystem; eType = IUIEventSystem::EEventSystemType
    virtual IUIEventSystem* GetEventSystem(const char* name, int eType) = 0;     // [31] 0xF8  0x1806054C0  VERIFIED: name lookup in the per-type event-system map, returns existing or 0
    virtual _smart_ptr<IUIEventSystemIterator> CreateEventSystemIterator(int eType) = 0; // [32] 0x100 0x180605530  VERIFIED: news a CUIEventSystemIterator (vftable stored); returns IUIEventSystemIteratorPtr by value

    // input events
    virtual void DispatchControllerEvent(int event, unsigned int state, float arg) = 0; // [33] 0x108 0x18359C728  VERIFIED shape: UI-blocked gate (sub_1835B1A48), broadcasts a controller event to elements
    virtual void SendFlashMouseEvent(int evt, int iX, int iY, int iButton, float fWheel, bool bFromController) = 0; // [34] 0x110 0x18085BB2C  VERIFIED: 6-arg mouse event broadcast to elements
    virtual bool DisplayVirtualKeyboard(unsigned int flags, const wchar_t* title, const wchar_t* initialInput, int maxInputLength, IVirtualKeyboardEvents* pInCallback) = 0; // [35] 0x118 0x18359C7B8  routes to the platform input handler (pSystem+0x1B0 chain) [conf MED by position]
    virtual bool IsVirtualKeyboardRunning() = 0;                                 // [36] 0x120 0x1835B1A48  returns pSystem input-handler +0x288 flag (also the UI-blocked gate) [conf LOW, name by position]
    virtual bool CancelVirtualKeyboard() = 0;                                    // [37] 0x128 0x18359825C  pSystem input-handler +0x296 [conf LOW, name by position]

    virtual void CheckPreloadedTexture(ITexture* pTexture) const = 0;            // [38] 0x130 0x1803B6E80  nullsub in KCD2 (empty) [conf LOW by position]
    virtual bool UseSharedRT(const char* instanceStr, bool defVal) const = 0;    // [39] 0x138 0x18176C6BC  VERIFIED shape: resolves element by instance str, returns its flag &1024
    virtual void GetMemoryStatistics(ICrySizer* s) const = 0;                    // [40] 0x140 0x1835AE240  VERIFIED: AddObject(this,472) + Add "FlashUI"/"Elements"/"Actions"/"EventSystems"
    virtual void GetScreenSize(int& width, int& height) = 0;                     // [41] 0x148 0x1835B0110  VERIFIED: writes screen w/h from this+0x110 override or pSystem+0x250/+0x248

    // UI Emulator (Sandbox) callbacks -- 6-slot set/clear block (each stores/clears a
    // 0x20-byte functor). Behavior VERIFIED (the set/clear pairing), individual
    // callback identity by KCD1 order.
    virtual void SetEditorScreenSizeCallback(void* cb) = 0;                      // [42] 0x150 0x181AAF1F0  stores functor this+0x110
    virtual void RemoveEditorScreenSizeCallback() = 0;                           // [43] 0x158 0x181AAF150  clears this+0x110
    virtual void SetEditorPlatformCallback(void* cb) = 0;                        // [44] 0x160 0x181AAF1D0  stores functor this+0x150
    virtual void RemoveEditorPlatformCallback() = 0;                             // [45] 0x168 0x181AAF120  clears this+0x150
    virtual void SetEditorUILogEventCallback(void* cb) = 0;                      // [46] 0x170 0x181AAF210  stores functor this+0x130
    virtual void RemoveEditorUILogEventCallback() = 0;                           // [47] 0x178 0x181AAF180  clears this+0x130

    virtual void* GetUIActionEvents() const = 0;                                 // [48] 0x180 0x180602360  VERIFIED: returns the CFlashUIActionEvents member (this+0x38); KCD1 had this at [6]
};

}  // namespace Offsets
