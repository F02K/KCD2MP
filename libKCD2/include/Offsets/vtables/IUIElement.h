#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// IUIElement -- KCD2 binary vtable order (REORDERED vs KCD1)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashUI.h
// (UNIQUE_IFACE struct IUIElement).
//
// Impl: CFlashUIElement (.?AVCFlashUIElement@@), size 0x250 (592; GetMemoryUsage
// AddObject + pool-alloc sites), ctor sub_18088DF88(elem, pFlashUI, pBaseElem, instanceId),
// newed by CFlashUI::LoadElementsFromFile and by GetInstance on a miss.
// Primary vtable @ 0x184049BD0, 113 slots (KCD1: 101), this_off 0.
// RTTI: CFlashUIElement : IUIElement (+0), IFSCommandHandler (+0x08),
//   IExternalInterfaceHandler (+0x10, NEW vs KCD1), IVirtualKeyboardEvents (+0x18).
//
// [FUNDAMENTAL vs KCD1]
// - Slot order is RESHUFFLED (e.g. GetInstances moved [5]->[7]); every slot below was
//   matched by BEHAVIOR (member offsets, GFx child-vfunc offsets, arg types), not position.
// - IUIElementIterator is GONE (no KCD2 RTTI). GetInstances [7] now fills a caller-
//   provided {begin,end,cap} vector snapshot (hidden sret) of the instances vector;
//   a ForEachInstance visitor slot [8] (std::function<bool(IUIElement*)>) was added.
//   Do NOT port the KCD1 IUIElementIterator.h.
// - GetFlashPlayer [42] and GetInstance [4] now return std::shared_ptr (hidden sret) --
//   CFlashPlayer/instances are shared_ptr-managed in KCD2 (see IFlashPlayer.h).
// - 8 new WORLD-HOST slots appended at the tail [105..112] (in-world/diegetic UI:
//   attach element to an entity's geometry, flash<->world coord mapping) + inserted
//   [8] (ForEachInstance), [32] (cursor-counter refresh), [43], [64] (cry_updateBoundaries).
// - Members recovered: +0x20 refcount, +0x28 m_pFlashUI, +0x30 m_sName, +0x38 m_sGroupName,
//   +0x40 m_sFlashFile, +0x48 m_fAlpha, +0x4C m_iLayer, +0x50 m_bValid(root), +0x51 m_bHiding,
//   +0x52/+0x53 unload-request flags, +0x58 m_pFlashPlayer (shared_ptr), +0x78 m_bVisible,
//   +0x80 m_flags (qword), +0x89/+0x8A dirty/lazy bytes, +0x8C SUIConstraints (40B),
//   +0xD0 m_pBaseElement, +0xD8 m_iInstanceID, +0xE0 SRWLOCK, +0xE8..+0xF8 instances vector,
//   +0x100/+0x118 var/array desc arrays (stride 0x28), +0x130/+0x148 movieclip/tmpl descs
//   (stride 0x88), +0x160/+0x178 event/function descs (SUIEventDesc 0x68; arrays proven by
//   "New dynamic created var/array/MC/MC Template" + "Dynamic Created Fct" alloc strings),
//   +0x198 world-host ptr, +0x1A0 sub-mtl name, +0x1A8 world-cursor flag, +0x1D0 listeners
//   vector, +0x1F8/+0x208 created-varobj maps, +0x218 ext textures (lock +0x230), +0x240
//   varobj-op lock, +0x68 bootstrapper.
//
// OVERLOAD PAIRS (verified from decompiled arg use; MSVC emits adjacent same-name
// overloads reversed -- 1st-declared lands at the HIGHER slot, so each group below is
// declared HIGHER-SLOT-FIRST):
// - The six Get*Desc pairs: (const char*) = LOWER slot, (int) = HIGHER slot
//   => declare (int) FIRST.  !! OPPOSITE of IFlashUI::GetUIElement, and NOTE: the KCD1
//   IUIElement.h declared these char*-first, which mis-dispatches -- do not copy it.
// - CallFunction / GetMovieClip / CreateMovieClip / SetVariable / GetVariable /
//   CreateVariable / SetArray / GetArray / CreateArray / LoadTexIntoMc / UnloadTexFromMc:
//   desc-version = LOWER slot, name-version = HIGHER slot => declare name-version FIRST.
// - RemoveMovieClip TRIPLE: binary (IFlashVariableObject*)[74] < (desc)[75] < (name)[76]
//   => declared in full reverse: (name), (desc), (varobj). [group-reversal per MSVC rule;
//   pair case runtime-proven, triple order flagged for /FA verification before first use]

struct SUIArguments;      // FlashUI value types -- GLOBAL namespace (defined in guimodule/SUITypes.h)
struct TUIData;
struct SUIParameterDesc;  // GLOBAL namespace (defined in guimodule/SUIParameterDesc.h)
struct SUIEventDesc;      // GLOBAL namespace (defined in guimodule/SUIEventDesc.h)
struct SUIMovieClipDesc;  // GLOBAL namespace (SDK type; not yet REd)

namespace Offsets {

struct IFlashPlayer;
struct IFlashVariableObject;
struct IUIElementEventListener;
struct IDynTextureSource;
struct ITexture;
struct XmlNodeRef;

struct IUIElement {
    // SDK nested type (POD replicated verbatim; +0x8C block, 40 bytes with tail flags):
    struct SUIConstraints {
        enum EPositionType  { ePT_Fixed, ePT_Fullscreen, ePT_Dynamic, ePT_FixedDynTexSize };
        enum EPositionAlign { ePA_Lower, ePA_Mid, ePA_Upper };
        EPositionType  eType;      // +0x00 (elem +0x8C; GetDynTexSize tests ==3)
        int            iLeft;      // +0x04
        int            iTop;       // +0x08
        int            iWidth;     // +0x0C (GetDynTexSize out)
        int            iHeight;    // +0x10
        EPositionAlign eHAlign;    // +0x14
        EPositionAlign eVAlign;    // +0x18
        bool           bScale;     // +0x1C
        bool           bMax;       // +0x24 in KCD2 block (padding-shifted)
    };

    virtual void Dtor(char flags) = 0;                                              // [0]   0x000  0x1816E94B0  scalar deleting dtor (~ sub_18088DAF0)
    virtual void AddRef() = 0;                                                      // [1]   0x008  0x1803C2A60  InterlockedIncrement refcount this+0x20
    virtual void Release() = 0;                                                     // [2]   0x010  0x18085EF08  InterlockedDecrement +0x20, deletes at 0

    // instances (IUIElementIterator is GONE -- see banner)
    virtual unsigned int GetInstanceID() const = 0;                                 // [3]   0x018  0x181A88CE0  returns dword this+0xD8
    virtual _smart_ptr<IUIElement> GetInstance(unsigned int instanceID) = 0;        // [4]   0x020  0x180555EE4  VERIFIED: searches root instances vector +0xE8 by id +0xD8; on miss pool-allocs 0x250 CFlashUIElement (ctor sub_18088DF88(elem, this+0x28 pFlashUI, this, id)), appends, notifies listeners OnInstanceCreated (+0x28); id 0 returns self (intrusive refcount +0x20)
    virtual bool DestroyInstance(unsigned int instanceID) = 0;                      // [5]   0x028  0x180564A68  VERIFIED: removes matching instance from root vector +0xE8 (rejects id 0), notifies listeners OnInstanceDestroyed (+0x30)
    virtual bool DestroyThis() = 0;                                                 // [6]   0x030  0x180564E54  DestroyInstance(this->m_iInstanceID)
    virtual std::vector<IUIElement*> GetInstances() = 0;                            // [7]   0x038  0x18085EDFC  VERIFIED: returns a {begin,end,cap} vector of root + instances (each AddRef'd) built under shared lock +0xE0 (REPLACES IUIElementIteratorPtr); caller must Release [2] every entry
    virtual void ForEachInstance(void* pFunctor) = 0;                               // [8]   0x040  0x180564CF4  KCD2-added: visits root+instances with std::function<bool(IUIElement*)> (functor cell at arg+0x38) until it returns false [name coined]

    // common
    virtual void SetName(const char* name) = 0;                                     // [9]   0x048  0x180563CA4  VERIFIED: CryString assign this+0x30; called by CFlashUI::LoadElementsFromFile with xml "name" attr (vf+0x48)
    virtual const char* GetName() const = 0;                                        // [10]  0x050  0x18072DD20  returns this+0x30
    virtual void SetGroupName(const char* groupName) = 0;                           // [11]  0x058  0x180563C98  VERIFIED: assign this+0x38; LoadElementsFromFile calls with file-level name (vf+0x58)
    virtual const char* GetGroupName() const = 0;                                   // [12]  0x060  0x180602360  returns this+0x38 (ICF-folded with CFlashUI::GetUIActionEvents)
    virtual void SetFlashFile(const char* flashFile) = 0;                           // [13]  0x068  0x1835C01CC  VERIFIED: strcmp against +0x40; on change assigns + ReloadBootStrapper (calls [22])
    virtual const char* GetFlashFile() const = 0;                                   // [14]  0x070  0x181A8B5A0  returns this+0x40

    virtual bool Init(bool bLoadAsset) = 0;                                         // [15]  0x078  0x18088EF1C  VERIFIED: loads flash asset ("CE_NoAutoUpdate" flag), creates player into +0x58, SetViewport, "cry_onSetup" invoke, notifies listeners OnInit(+0x10); ReloadBootStrapper re-Inits via vf+0x78
    virtual void Unload(bool bAllInstances) = 0;                                    // [16]  0x080  0x18088E93C  VERIFIED: bAll -> DestroyInstance sweep; releases player +0x58, notifies OnUnload (+0x18)
    virtual void Reload(bool bAllInstances) = 0;                                    // [17]  0x088  0x1835BD1DC  VERIFIED: bAll -> per-instance vf+0x88 recursion; else save visible, Unload, Init, restore
    virtual bool IsInit() const = 0;                                                // [18]  0x090  0x180773960  returns (this+0x58 != 0); ReloadBootStrapper snapshots via vf+0x90
    virtual void RequestUnload(bool bAllInstances) = 0;                             // [19]  0x098  0x181AAF1B0  this+0x52=1, +0x53=bAllInstances (processed by Update [23])
    virtual bool IsValid() const = 0;                                               // [20]  0x0A0  0x180890160  returns root element byte +0x50 (walks +0xD0 chain; +0x50 set from Serialize success)

    virtual void UnloadBootStrapper() = 0;                                          // [21]  0x0A8  0x1835C1C20  thunk -> sub_1815A32E0: Unload(true) + releases bootstrapper +0x68; ReloadBootStrapper calls per-instance via vf+0xA8
    virtual void ReloadBootStrapper() = 0;                                          // [22]  0x0B0  0x1835BD35C  VERIFIED ANCHOR: snapshots IsInit(vf+0x90)/IsVisible(vf+0xE8) per instance, UnloadBootStrapper(vf+0xA8), re-Init(vf+0x78) + SetVisible(vf+0xE0)
    virtual void Update(float fDeltaTime) = 0;                                      // [23]  0x0B8  0x18085EF30  VERIFIED: lazy-flag gate (+0x80 & 0x1800 / dirty +0x89), player->Advance (player vf+0x60), processes RequestUnload +0x52/+0x53, "cry_onLanguageChanged"
    virtual void Render() = 0;                                                      // [24]  0x0C0  0x181006468  player->Render (player vf+0x68)
    virtual void RenderLockless() = 0;                                              // [25]  0x0C8  0x181006468  same impl as [24] -- COMDAT folded (as KCD1)

    // visibility
    virtual void RequestHide() = 0;                                                 // [26]  0x0D0  0x1835BE3C4  VERIFIED: invokes "cry_requestHide", sets m_bHiding +0x51
    virtual bool IsHiding() const = 0;                                              // [27]  0x0D8  0x181AAF0F0  returns byte +0x51
    virtual void SetVisible(bool bVisible) = 0;                                     // [28]  0x0E0  0x180568510  VERIFIED: +0x78, invokes "cry_onShow"/"cry_onHide", notifies OnSetVisible (+0x20)
    virtual bool IsVisible() const = 0;                                             // [29]  0x0E8  0x181A7D900  returns byte +0x78
    virtual void SetFlag(int flag, bool bSet) = 0;                                  // [30]  0x0F0  0x1818DD734  EFlashUIFlags; rejects flag & 0x1000 (const flags); writes qword +0x80
    virtual bool HasFlag(int flag) const = 0;                                       // [31]  0x0F8  0x18056AD6C  VERIFIED ANCHOR: flag & +0x80 (root-delegates 0xF0000 mask); CFlashUI::UseSharedRT calls vf+0xF8 with 0x400
    virtual void UpdateCursorState() = 0;                                           // [32]  0x100  0x18056ACBC  KCD2-added: syncs hardware-mouse counter (gEnv->pHardwareMouse Inc/DecrementCounter +0x58/+0x60) + player mouse flag from flags/visibility [name coined]
    virtual float GetAlpha() const = 0;                                             // [33]  0x108  0x181A739D0  returns float +0x48
    virtual void SetAlpha(float fAlpha) = 0;                                        // [34]  0x110  0x1835BFCA8  clamps 0..1 -> +0x48, player->SetBackgroundAlpha (player vf+0x10)
    virtual int GetLayer() const = 0;                                               // [35]  0x118  0x181A718A0  returns dword +0x4C
    virtual void SetLayer(int iLayer) = 0;                                          // [36]  0x120  0x1835C02C0  writes dword +0x4C
    virtual void SetConstraints(const SUIConstraints& newConstraints) = 0;          // [37]  0x128  0x1835BFF50  copies 40B block to +0x8C, then UpdateViewPort [62]
    virtual const SUIConstraints& GetConstraints() const = 0;                       // [38]  0x130  0x18056AFC0  returns &this+0x8C

    // lazy update
    virtual void ForceLazyUpdate() = 0;                                             // [39]  0x138  0x181AAEEE0  word +0x89 = 1 (dirty + lazy)
    virtual void LazyRendered() = 0;                                                // [40]  0x140  0x181AAF110  byte +0x8A = 0
    virtual bool NeedLazyRender() const = 0;                                        // [41]  0x148  0x1835B2720  !(flags & 0x1800) || byte +0x8A

    virtual std::shared_ptr<IFlashPlayer> GetFlashPlayer() = 0;                     // [42]  0x150  0x1803FA4AC  VERIFIED: lazy-inits (creates the player if absent) then returns the shared_ptr copied from m_pFlashPlayer +0x58 (CFlashPlayer is shared_ptr-managed, see IFlashPlayer.h; KCD1 returned a raw ptr)
    virtual bool _vf43() = 0;                                                       // [43]  0x158  0x180773960  UNVERIFIED name: identical body to IsInit [18] (this+0x58 != 0); likely HasFlashPlayer/IsLoaded

    // descs. Each (int)/(const char*) pair: char* = LOWER slot, int = HIGHER slot
    // => (int) DECLARED FIRST (see banner; opposite of IFlashUI::GetUIElement).
    virtual const SUIParameterDesc* GetVariableDesc(int index) const = 0;           // [45]  0x168  0x1835B0670  bounds-checked index into +0x100
    virtual const SUIParameterDesc* GetVariableDesc(const char* varName) const = 0; // [44]  0x160  0x1835B0688  name lookup on +0x100 (sub_18088D5F8)
    virtual int GetVariableCount() const = 0;                                       // [46]  0x170  0x181AAF060  count of +0x100
    virtual const SUIParameterDesc* GetArrayDesc(int index) const = 0;              // [48]  0x180  0x18359FAD0  index into +0x118
    virtual const SUIParameterDesc* GetArrayDesc(const char* arrayName) const = 0;  // [47]  0x178  0x18359FAE8  name lookup on +0x118
    virtual int GetArrayCount() const = 0;                                          // [49]  0x188  0x181AAEEF0  count of +0x118
    virtual const SUIMovieClipDesc* GetMovieClipDesc(int index) const = 0;          // [51]  0x198  0x18153E2FC  index into +0x130
    virtual const SUIMovieClipDesc* GetMovieClipDesc(const char* movieClipName) const = 0; // [50]  0x190  0x1835AF5BC  name lookup on +0x130 (sub_18088D834) -- also used by CallFunction/SetVariable name-versions to resolve pTmplName
    virtual int GetMovieClipCount() const = 0;                                      // [52]  0x1A0  0x18153E580  count of +0x130
    virtual const SUIMovieClipDesc* GetMovieClipTmplDesc(int index) const = 0;      // [54]  0x1B0  0x1835AF5C8  index into +0x148
    virtual const SUIMovieClipDesc* GetMovieClipTmplDesc(const char* movieClipTmplName) const = 0; // [53]  0x1A8  0x1835AF5E0  name lookup on +0x148
    virtual int GetMovieClipTmplCount() const = 0;                                  // [55]  0x1B8  0x18176A240  count of +0x148
    virtual const SUIEventDesc* GetEventDesc(int index) const = 0;                  // [57]  0x1C8  0x1816CB258  index into +0x160
    virtual const SUIEventDesc* GetEventDesc(const char* eventName) const = 0;      // [56]  0x1C0  0x1835AD6E8  name lookup on +0x160 (sub_18088B3EC)
    virtual int GetEventCount() const = 0;                                          // [58]  0x1D0  0x181AAEFC0  count of +0x160
    virtual const SUIEventDesc* GetFunctionDesc(int index) const = 0;               // [60]  0x1E0  0x1816056F4  index into +0x178
    virtual const SUIEventDesc* GetFunctionDesc(const char* functionName) const = 0;// [59]  0x1D8  0x1807F6F20  name lookup on +0x178
    virtual int GetFunctionCount() const = 0;                                       // [61]  0x1E8  0x181AAEFF0  count of +0x178

    virtual void UpdateViewPort() = 0;                                              // [62]  0x1F0  0x18088EB90  VERIFIED: constraints +0x8C -> player->SetViewport (player vf+0x20), invokes "cry_onResize"
    virtual void GetViewPort(int& x, int& y, int& width, int& height, float& aspectRatio) = 0; // [63]  0x1F8  0x1835B0694  player->GetViewport (player vf+0x28)
    virtual void UpdateBoundaries() = 0;                                            // [64]  0x200  0x180568C0C  KCD2-added: recomputes movie boundaries, invokes "cry_updateBoundaries" (caches +0x1BC..+0x1C8) [name coined]

    virtual bool Serialize(XmlNodeRef& xmlNode, bool bIsLoading) = 0;               // [65]  0x208  0x180563CB0  VERIFIED ANCHOR: CFlashUI::LoadElementsFromFile calls vf+0x208 (node, true); NOTE KCD1 had CallFunction here -- slot meaning CHANGED

    virtual void AddEventListener(IUIElementEventListener* pListener, const char* name) = 0; // [66]  0x210  0x180566138  dedup-append into +0x1D0 (walked by Init/Unload/SetVisible/instance events)
    virtual void RemoveEventListener(IUIElementEventListener* pListener) = 0;       // [67]  0x218  0x1805660E4  remove from +0x1D0 (deferred if iterating)

    // functions. name-version = HIGHER slot => declared FIRST.
    virtual bool CallFunction(const char* fctName, const SUIArguments& args, TUIData* res, const char* pTmplName) = 0; // [69]  0x228  0x1807F6D94  VERIFIED: resolves fct by name + tmpl via GetMovieClipDesc(name), tail-calls [68]
    virtual bool CallFunction(const SUIEventDesc* pFctDesc, const SUIArguments& args, TUIData* res, const SUIMovieClipDesc* pTmplDesc) = 0; // [68]  0x220  0x1807F6C64  VERIFIED: GFx Invoke (varobj vf+0x70), converts args/result, RemoveFlashVarObj cleanup

    virtual IFlashVariableObject* GetMovieClip(const char* movieClipName, const char* pTmplName) = 0;                          // [71]  0x238  0x1807F4C18  name-resolves then [70]
    virtual IFlashVariableObject* GetMovieClip(const SUIMovieClipDesc* pMovieClipDesc, const SUIMovieClipDesc* pTmplDesc) = 0; // [70]  0x230  0x1807F4C60  resolves flash var obj, type-checked (varobj vf+0x20)
    virtual IFlashVariableObject* CreateMovieClip(const SUIMovieClipDesc*& pNewInstanceDesc, const char* movieClipTemplate, const char* mcParentName, const char* mcInstanceName) = 0;   // [73]  0x248  0x18359B7A0  name-resolves then [72]
    virtual IFlashVariableObject* CreateMovieClip(const SUIMovieClipDesc*& pNewInstanceDesc, const SUIMovieClipDesc* pMovieClipTemplateDesc, const SUIMovieClipDesc* pParentMC, const char* mcInstanceName) = 0; // [72]  0x240  0x18359B804  GFx attachMovie (varobj vf+0x148), auto-names "%s_%i", registers desc in +0x1F8
    // RemoveMovieClip TRIPLE: binary order (varobj)[74] < (desc)[75] < (name)[76];
    // declared fully reversed (see banner).
    virtual void RemoveMovieClip(const char* movieClipName) = 0;                    // [76]  0x260  0x1835BE030  name-resolves then [75]
    virtual void RemoveMovieClip(const SUIParameterDesc* pMovieClipDesc) = 0;       // [75]  0x258  0x1835BE058  invokes "removeMovieClip", releases GFx obj, unregisters desc
    virtual void RemoveMovieClip(IFlashVariableObject* pVarObject) = 0;             // [74]  0x250  0x1835BDFC8  reverse-lookup in +0x1F8 map then [75]

    // variables (name-version = HIGHER slot => declared FIRST). value = TUIData*.
    virtual bool SetVariable(const char* varName, const void* value, const char* pTmplName) = 0;                              // [78]  0x270  0x1835C0BF8  name-resolves then desc-version
    virtual bool SetVariable(const SUIParameterDesc* pVarDesc, const void* value, const SUIMovieClipDesc* pTmplDesc) = 0;      // [77]  0x268  0x1835C0C5C  thunk -> sub_1835C0DBC (GFx SetMember)
    virtual bool GetVariable(const char* varName, TUIData* valueOut, const char* pTmplName) = 0;                                 // [80]  0x280  0x1835B0490  name-resolves then [79]
    virtual bool GetVariable(const SUIParameterDesc* pVarDesc, TUIData* valueOut, const SUIMovieClipDesc* pTmplDesc) = 0;         // [79]  0x278  0x1835B04F4  GFx GetMember (varobj vf+0x50), converts to TUIData
    virtual bool CreateVariable(const SUIParameterDesc*& pNewDesc, const char* varName, const void* value, const char* pTmplName) = 0;       // [82]  0x290  0x18359AD4C  name-tmpl wrapper (ICF-folded with CreateArray [88]; decompile degenerate -- verify before first call)
    virtual bool CreateVariable(const SUIParameterDesc*& pNewDesc, const char* varName, const void* value, const SUIMovieClipDesc* pTmplDesc) = 0; // [81]  0x288  0x18359BEE8  creates desc (+0x100), sets *pNewDesc, then SetVariable

    // arrays (name-version = HIGHER slot => declared FIRST)
    virtual bool SetArray(const char* arrayName, const SUIArguments& values, const char* pTmplName) = 0;                       // [84]  0x2A0  0x1807F6FF0  name-resolves then [83]
    virtual bool SetArray(const SUIParameterDesc* pArrayDesc, const SUIArguments& values, const SUIMovieClipDesc* pTmplDesc) = 0; // [83]  0x298  0x1807F6300  GFx SetArraySize (varobj vf+0x80) + per-element SetElement (vf+0x90)
    virtual bool GetArray(const char* arrayName, SUIArguments& valuesOut, const char* pTmplName) = 0;                          // [86]  0x2B0  0x18359F728  name-resolves then [85]
    virtual bool GetArray(const SUIParameterDesc* pArrayDesc, SUIArguments& valuesOut, const SUIMovieClipDesc* pTmplDesc) = 0; // [85]  0x2A8  0x18359F788  GFx GetArraySize (vf+0x78) + GetElement (vf+0xA0) into SUIArguments
    virtual bool CreateArray(const SUIParameterDesc*& pNewDesc, const char* arrayName, const SUIArguments& values, const char* pTmplName) = 0; // [88]  0x2C0  0x18359AD4C  name-tmpl wrapper (ICF-folded with [82]; verify before first call)
    virtual bool CreateArray(const SUIParameterDesc*& pNewDesc, const char* arrayName, const SUIArguments& values, const SUIMovieClipDesc* pTmplDesc) = 0; // [87]  0x2B8  0x18359AD70  player->CreateArray (player vf+0x190), registers desc, then SetArray

    // textures (name-version = HIGHER slot => declared FIRST)
    virtual void LoadTexIntoMc(const char* movieClip, ITexture* pTexture, const char* pTmplName) = 0;                          // [90]  0x2D0  0x1835B24BC  name-resolves then [89]
    virtual void LoadTexIntoMc(const SUIParameterDesc* pMovieClipDesc, ITexture* pTexture, const SUIMovieClipDesc* pTmplDesc) = 0; // [89]  0x2C8  0x1835B251C  VERIFIED: invokes loadMovie("img://TEXID:%i", tex->GetTextureID vf+0x40)
    virtual void UnloadTexFromMc(const char* movieClip, ITexture* pTexture, const char* pTmplName) = 0;                        // [92]  0x2E0  0x1835C1CA0  name-resolves then [91]
    virtual void UnloadTexFromMc(const SUIParameterDesc* pMovieClipDesc, ITexture* pTexture, const SUIMovieClipDesc* pTmplDesc) = 0; // [91]  0x2D8  0x1835C1D00  VERIFIED: invokes unloadMovie("img://TEXID:%i")

    virtual void ScreenToFlash(const float& px, const float& py, float& rx, float& ry, bool bStageScaleMode) const = 0; // [93]  0x2E8  0x1835BEBE4  player W/H + viewport -> flash coords
    virtual void WorldToFlash(const void* camMat, const void* worldpos, void* flashpos, void* borders, float& scale, bool bStageScaleMode) const = 0; // [94]  0x2F0  0x1835C3930  Matrix34/Vec3/Vec3/Vec2 projection then ScreenToFlash

    // dyn textures
    virtual void AddTexture(IDynTextureSource* pDynTexture) = 0;                    // [95]  0x2F8  0x18088E4D8  insert into +0x218; SetVisible(true) + full-size viewport
    virtual void RemoveTexture(IDynTextureSource* pDynTexture) = 0;                 // [96]  0x300  0x18088E69C  remove from +0x218
    virtual int GetNumExtTextures() const = 0;                                      // [97]  0x308  0x1835AF7BC  count of +0x218 (lock +0x230)
    virtual bool GetDynTexSize(int& width, int& height) const = 0;                  // [98]  0x310  0x1835AD3F8  w/h from constraints +0x98/+0x9C; returns eType==ePT_FixedDynTexSize

    // input events
    virtual void SendCursorEvent(int evt, int iX, int iY, int iButton, float fWheel) = 0; // [99]  0x318  0x18056AA60  VERIFIED: hw-cursor counter (cvar wh_ui_InputDevice), world-host coord remap (+0x198/+0x1A8, caches last pos +0x1AC/+0x1B0), player->SendCursorEvent (player vf+0xF8) + Advance
    virtual void SendKeyEvent(const void* evt) = 0;                                 // [100] 0x320  0x1835BEF2C  player->SendKeyEvent (player vf+0x100); SFlashKeyEvent&
    virtual void SendCharEvent(const void* charEvent) = 0;                          // [101] 0x328  0x1835BEDFC  player->SendCharEvent (player vf+0x108); SFlashCharEvent&
    virtual void SendControllerEvent(int event, int state, float value) = 0;        // [102] 0x330  0x1835BEE44  VERIFIED: invokes "cry_onController"(event, state, float) -- KCD2 ADDED the float arg

    virtual void GetMemoryUsage(void* s) const = 0;                                 // [103] 0x338  0x1835AE8AC  VERIFIED: Add("UIElement") + AddObject(this, 592) + desc containers + "Instances" recursion; ICrySizer*
    virtual void RemoveFlashVarObj(const SUIParameterDesc* pDesc) = 0;              // [104] 0x340  0x1807F6620  VERIFIED: CallFunction [68] cleanup path; purges created-MC descs by name

    // --- KCD2-added WORLD-HOST tail: in-world (diegetic) UI on an entity's geometry.
    // +0x198 = world-host object (vf+0x100 yields its Matrix34 world TM), +0x1A0 = host
    // sub-material name (raycast filter), +0x1A8 = world-cursor-active flag.
    // All names UNVERIFIED/coined from behavior.
    virtual void SetWorldHost(void* pHost, const char* subMtlName) = 0;             // [105] 0x348  0x18073EF78  +0x198 = pHost, +0x1A0 = subMtlName [UNVERIFIED name]
    virtual void ClearWorldHost() = 0;                                              // [106] 0x350  0x180564E60  +0x198 = 0 [UNVERIFIED name]
    virtual void SetWorldCursorActive(bool bActive) = 0;                            // [107] 0x358  0x180562B58  !bActive -> synth mouse-leave SendCursorEvent(0,-1,-1,0,0); byte +0x1A8 = bActive [UNVERIFIED name]
    virtual Vec3 FlashToWorldPos() = 0;                                             // [108] 0x360  0x1820E3640  flash coords -> WORLD position: raycasts host StatObj sub-objects filtered by +0x1A0 sub-mtl, transforms by host world TM; returns (-1,-1,-1) on miss [UNVERIFIED name]
    virtual void FlashToScreenCoords(int a2, int a3, int a4, void* a5) = 0;         // [109] 0x368  0x1816CF8F4  flash coords -> screen pixel coords ([108] raycast + camera projection via pSystem vf+0x438) [UNVERIFIED name]
    virtual bool WorldHitTest(int iX, int iY) = 0;                                  // [110] 0x370  0x1816BD548  screen (x,y) -> flash coords via mapper sub_180568FD4; false if no world host (+0x198) or coord == -1 [UNVERIFIED name]
    virtual unsigned int _vf111() const = 0;                                        // [111] 0x378  0x1820E34A0  UNVERIFIED: returns dword +0x1CC
    virtual void _vf112(unsigned int v) = 0;                                        // [112] 0x380  0x1820E3D40  UNVERIFIED: writes dword +0x1CC (pair with [111])
};

}  // namespace Offsets
