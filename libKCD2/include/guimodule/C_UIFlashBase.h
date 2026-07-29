#pragma once
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include <boost/container/vector.hpp>
#include "guimodule/C_UIBase.h"
#include "Offsets/vtables/IUIElementEventListener.h"   // Offsets::IUIElementEventListener / IUIElement / SUIEventDesc / SUIArguments

// -----------------------------------------------
// wh::guimodule::C_UIFlashBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58.
// -----------------------------------------------
// RTTI .?AVC_UIFlashBase@guimodule@wh@@ (TD 0x184C92B80).
// Bases: C_UIBase @+0x00, IUIElementEventListener @+0x10 (COLs 0x413BA80 / 0x413BAA8).
// vtables: primary 0x183A81128 (14 slots), listener 0x183A810E0 (8 slots, dtor thunk
// at [7] sub_18213A234). ctor sub_180BC86C4.
//
// Base of every Flash-backed UI screen. Binds to one IFlashUI element by name and owns
// the two-way call bridge:
//
//   INBOUND  (Flash -> C++): the listener slot [0] (OnUIEvent, sub_180557C2C) checks
//   pSender == m_pElement, looks the event name up in the SORTED flat map m_dispatchers
//   (lower_bound sub_180557D10 over 16-byte {name, I_Dispatcher*} pairs), copies the
//   SUIArguments (0x30, sub_1810C294C) and QUEUES a std::function<void()> closure
//   ({map-pair*, args copy, tail arg} -> _Func_impl 0x48, built by sub_181F50180/
//   sub_181F512D0) into m_queuedEvents. The game-thread Update override (slot [4],
//   sub_180533878) moves the queue out and invokes each closure, which virtual-calls
//   pair->second->Dispatch(...) -- the S_Dispatcher below unpacks each SUIData into the
//   typed C++ argument list and calls the registered member handler.
//
//   OUTBOUND (C++ -> Flash): non-virtual per-signature CallFunction helpers (e.g.
//   sub_1808BE874 = (name, bool), sub_1805576BC = (name, CryStringT, int)) which invoke
//   vf[12] first (lazy-bind hook, nullsub default), box the typed args into SUIArguments
//   and call m_pElement->CallFunction (KCD2 IUIElement vf +0x228) -- there is NO
//   KCD1-style SUIEventSenderDispatcher any more.
//
// Element binding (Init override, slot [1], sub_180565D38): stores the module ptr, then
// g_pFlashUI (qword_18492D938, cached IFlashUI*) ->GetUIElement(GetElementName())
// [vf +0x70], stores it at +0x48 and AddEventListener(this+0x10, "UIEClass") [vf +0x210];
// the module-owned C_UICommonEventHandler is registered per element under
// "CommonEventHandler" (rebind helper sub_180565FD0 reads it from m_pModule+0x28).
// Deinit (slot [2], sub_182B12388) = m_pElement->RemoveEventListener(this+0x10) [vf +0x218].
//
// [FUNDAMENTAL vs KCD1] Replaces the whole KCD1 C_UIEBase + SUIEventReceiverDispatcher /
// SUIEventSenderDispatcher / IUIEventSystem descriptor machinery. Inbound calls are now
// name-string keyed (no event ids) and deferred through a std::function queue; handler
// binding is a plain {object, function} pair per name.

namespace wh::guimodule {

class C_UIFlashBase : public C_UIBase, public Offsets::IUIElementEventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashBase;
    // Type-erased dispatcher interface (RTTI .?AUI_Dispatcher@C_UIFlashBase@guimodule@wh@@,
    // vtable 0x183ED9460: [0] deleting dtor sub_182AFDA70, [1] pure Dispatch).
    struct I_Dispatcher {
        virtual ~I_Dispatcher();                                                          // [0]
        // a2 is the tail value captured from the listener call (unused by every observed
        // instantiation); args is the flash-side argument pack.
        virtual void Dispatch(void* pUserData, const SUIArguments& args) const = 0;  // [1]
    };

    // Typed dispatcher (22 instantiations, e.g. .?AU?$S_Dispatcher@XV?$CryStringT@D@@H@...).
    // sizeof 0x18 (alloc 24, e.g. S_Dispatcher<void,bool> at registration helper
    // sub_180ED1478). Dispatch (e.g. void(bool)=0x1819DB364, void()=0x18155CDE8,
    // void(CryStringT,int)=0x18188AEE4) validates args.GetArgCount() == sizeof...(Args),
    // converts each SUIData with the per-type extractors (string sub_180558308,
    // int sub_180558C28, bool sub_18055964C, ...) and calls m_pfnHandler(m_pObject, ...).
    // Ret is void in all 22 observed instantiations.
    template<typename Ret, typename... Args>
    struct S_Dispatcher : I_Dispatcher {
        void Dispatch(void* pUserData, const SUIArguments& args) const override;

        void* m_pObject;                       // +0x08  handler "this" (type-erased: each of the 22 S_Dispatcher instantiations binds a different receiver; void* is correct)
        Ret (*m_pfnHandler)(void*, Args...);   // +0x10  handler thunk (non-virtual member fn address)
    };

    // Listener slot [0] override = the inbound queueing entry (sub_180557C2C, doc above).
    // [G4c VERIFIED] KCD2 OnUIEvent is the 4-formal-param Warhorse variant: pResult is a
    // NULLABLE TUIData* out-buffer (see Offsets/vtables/IUIElementEventListener.h).
    // This body captures pResult into the deferred closure (closure+0x40) and passes it
    // to I_Dispatcher::Dispatch as pUserData; C_UICommonEventHandler's slot [0]
    // (sub_180557F3C, "GetButtonId") WRITES a TUIData{4, id} result through it.
    void OnUIEvent(Offsets::IUIElement* pSender, const SUIEventDesc& event,
                   const SUIArguments& args, void* pResult) override;   // [L0] 0x180557C2C

    // Listener slots [1..6] are implemented as empty defaults in the binary vtable
    // 0x183A810E0 (all fold onto nullsub_1); [7] is the per-class deleting-dtor thunk.
    // Declared here (empty inline, matching the nullsubs) so subclasses are concrete
    // and can be embedded by value (C_UIMap does).
    void OnUIEventEx(Offsets::IUIElement*, const char*, const SUIArguments&, void*) override {}  // [L1] nullsub_1
    void OnInit(Offsets::IUIElement*, Offsets::IFlashPlayer*) override {}                                  // [L2] nullsub_1
    void OnUnload(Offsets::IUIElement*) override {}                                                        // [L3] nullsub_1
    void OnSetVisible(Offsets::IUIElement*, bool) override {}                                              // [L4] nullsub_1
    void OnInstanceCreated(Offsets::IUIElement*, Offsets::IUIElement*) override {}                         // [L5] nullsub_1
    void OnInstanceDestroyed(Offsets::IUIElement*, Offsets::IUIElement*) override {}                       // [L6] nullsub_1
    void Dtor(char) override {}                                                                            // [L7] deleting-dtor slot (thunk sub_18213A234)

    virtual void _vf10();                       // [10] 0x180562B3C  m_pElement->vf[+0x358]() when bound; role unknown (UNVERIFIED)
    virtual const char* GetElementName() const; // [11] 0x180564FA0  returns m_szElementName (leaves override with literals, e.g. "Menu")
    virtual void _vf12();                       // [12] nullsub_1    pre-CallFunction hook, invoked by every outbound helper (UNVERIFIED role)
    virtual void _vf13();                       // [13] nullsub_1    role unknown (UNVERIFIED)

    // Registers a typed inbound handler: sorted-insert of {name, new S_Dispatcher<Ret, Args...>
    // {pObject, pfnHandler}} into m_dispatchers (per-signature helper, e.g. sub_180ED1478
    // for void(bool); insert body sub_180565904).
    template<typename Ret, typename... Args>
    void RegisterDispatcher(const CryStringT<char>& name, void* pObject, Ret (*pfnHandler)(void*, Args...));

    // IUIElementEventListener occupies +0x10 (vtable only).
    // Name-sorted flat map of inbound handlers ({ptr,size,cap} shape => boost vector;
    // lookup sub_180557D10, teardown sub_180565CA8 deletes dispatcher + frees name).
    boost::container::vector<std::pair<CryStringT<char>, I_Dispatcher*>> m_dispatchers;  // +0x18
    std::vector<std::function<void()>> m_queuedEvents;   // +0x30  pending inbound calls, drained by Update slot [4] sub_180533878
    Offsets::IUIElement* m_pElement;                     // +0x48  bound flash element (g_pFlashUI->GetUIElement(GetElementName()))
    const char*          m_szElementName;                // +0x50  flash element name; ctor inits to the shared "" literal (byte_183A3D1E0)
};
static_assert(sizeof(C_UIFlashBase) == 0x58, "C_UIFlashBase must be 0x58");
static_assert(sizeof(C_UIFlashBase::S_Dispatcher<void, bool>) == 0x18, "S_Dispatcher must be 0x18");
static_assert(offsetof(C_UIFlashBase, m_dispatchers) == 0x18, "dispatcher map at 0x18 (lookup sub_180557D10)");
static_assert(offsetof(C_UIFlashBase, m_queuedEvents) == 0x30, "event queue at 0x30 (drain sub_180533878)");
static_assert(offsetof(C_UIFlashBase, m_pElement) == 0x48, "element at 0x48");

}  // namespace wh::guimodule
