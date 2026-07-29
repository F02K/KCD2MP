#pragma once
#include <cstdint>
#include <memory>
#include "C_SharedResource.h"
#include "E_NodeRuntimeState.h"
#include "S_NodeExecuteContext.h"
#include "rttr/variant.h"

class XmlNodeRef;

// -----------------------------------------------
// wh::conceptmodule::C_Node -- Skald concept-graph node base (KCD2 WHGame.dll Steam
// 1.5.6, e4cp).  sizeof 0x40, vtable 0x183A46838 (41 slots).  Ctor 0x1806B2744.
// -----------------------------------------------
// Every Skald node class derives from this (195 classes register a
// _smart_ptr<T> -> _smart_ptr<C_Node> rttr converter): C_SharedResource -> C_Node ->
// {C_If, C_TemplatedNode, C_AutoTriggerable<C_Node> -> C_Effect, ...}.
// Two-phase lifecycle:
//  * [4] Activate() runs ONCE (Flag_Activated): rttr-enumerates this type's properties,
//    binds every port-ref-typed member (type set qword_1855D3CA0) to this node and
//    populates/repairs m_ports.
//  * [13] Execute(ctx) fires per trigger pulse -- graph edges are std::bind
//    subscriptions of the slot-13 PMF (thunk 0x18213877C) stored on trigger ports --
//    gates on m_runtimeState, then delegates to [33] OnExecute(ctx), the per-type work.
// rttr type "wh::conceptmodule::Node" (reg sub_180C0DE9C): properties Name / RuntimeState
// (read-only) / Ports / PortsView (getter 0x18270A4D8 -> std::map<std::string,
// _smart_ptr<I_Port>>); methods EnableDebug / Save / Load; nested type Node::Hint.
// Member offsets are proven twice: ctor writes AND the member-object-pointer constants
// baked into the property builders (sub_180C0E4F0/460/610 -> 0x10/0x18/0x28).

namespace wh::conceptmodule {

class I_Port;
namespace definition { class I_PortDefinition; }

class C_Node : public C_SharedResource {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Node;

    virtual ~C_Node();                                     // [0]  0x1806B3EF4; body 0x1806B4318 unsubscribes + frees m_ports, frees m_name
    RTTR_ENABLE(C_SharedResource)                          // [5..7] trio overrides, get_type 0x180D2164C ([4] Activate inherited: shared 0x180E544B4)
    virtual XmlNodeRef Save();                             // [8]  base 0x1823CA690 returns null ref (ICF-folded with [9]); rttr method "Save"
    virtual _smart_ptr<C_Node> FindChildByName(CryStringT<char> const& name);  // [9]  base sret-null (leaves have no children); edge resolver ResolvePortPath 0x1806988CC hops through it [name LIKELY]
    virtual int32_t GetExecutionOrder() const;             // [10] base 0 (0x18066CD10, 19760 xrefs); C_AutoTriggerable/C_Effect 1 -- ascending sort key of the trigger-fire batch (read @0x18061dd7e): effects fire last
    virtual bool Load(XmlNodeRef node);                    // [11] base 0x18270A610 destroys the by-value arg, returns true; rttr method "Load"; FunctionBase family overrides validate here ("method:'%s' does not exist", 0x1826B9E10)
    virtual rttr::variant GetPortValue(_smart_ptr<I_Port> const& port);  // [12] base 0x180453D30 invalid variant; pull-model: C_DataPort::GetValueImpl 0x18069254C asks the owner node (vcall+0x60); C_MakeArray 0x1806A98B8
    virtual void Execute(S_NodeExecuteContext const& ctx); // [13] 0x180692060 trigger entry: Awake -> OnExecute; Hibernating -> only if the firing port's definition (I_Port+0x18) vslot-16 predicate allows [LIKELY IsTriggerableWhenHibernated]
    virtual void unk14();                                  // [14] 0x18270A924 posts _smart_ptr<this> to a queue with tag 5 (PostSelfMessage candidate) [U sig]
    virtual void DispatchLifecycleEvent(int32_t event, int32_t expectedPhase);  // [15] 0x1806B0AFC: skip if suppressed [36] while Hibernating; deliver [34] if [35] phase matches
    virtual void unk16();                                  // [16] nullsub [U]
    virtual void Shutdown();                               // [17] 0x18069557C: [37] + port unsubscribe walk (sub_181E34DC0, sub_181E34C50) [name LIKELY]
    virtual void Rebind(void* arg);                        // [18] 0x181E34CA0: [37] + port walk + per-target lambda (vt 0x183E38910) [name LIKELY, sig U]
    virtual void unk19();                                  // [19] 0x180695328 Detach candidate: smart self -> sub_181E318A0 -> [38] [U]
    virtual void Hibernate(std::vector<_smart_ptr<C_Node>>& changed);  // [20] 0x180ACFEE4: -> Hibernating, appends self to `changed` if it flipped
    virtual void WakeUp(std::vector<_smart_ptr<C_Node>>& changed);     // [21] 0x180695248: -> Awake, appends self to `changed` if it flipped
    virtual bool unk22();                                  // [22] return false (0x180838AE0) [U]
    virtual bool unk23();                                  // [23] return false (0x180838AE0) [U]
    virtual bool IsAutoTriggerable() const;                // [24] base false; C_AutoTriggerable+ true (0x18041A6A0)
    virtual void SetAutoTriggerEnabled(bool enabled);      // [25] base nullsub; C_AutoTriggerable+ sets/clears Flag_AutoTriggerEnabled (0x1823CC2DC)
    virtual void unk26();                                  // [26] 0x180F57008 takes a std::function by value, base just destroys it [U sig]
    virtual void EnumerateNodeVariants();                  // [27] base 0x18061DF80 no-op; Skald editor palette enumerator -- C_Function 0x18068FCA4 emits one variant per reflected global method with metadata[0]==1; C_If 0x18268E70C emits {"If", 2, "wh::conceptmodule"} [sig U: (sink, bool allVariants)]
    virtual void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink);  // [28] base 0x1804F5004: one definition per reflected C_PortRef-typed property (filter type qword_1855D3CB0); dynamic-pin nodes synthesize (FunctionBase 0x1804F5504 from the method signature, Switch/Select/StateBase per group)
    virtual bool unk29();                                  // [29] base false; C_StateVariable -> true (0x18041A6A0) [U -- "has persistent value" candidate]
    virtual void unk30();                                  // [30] base nullsub; C_StateVariable 0x1808B0F8C applies a new value with change detection -> fires OnStateChanged [SetValue candidate, sig U]
    virtual uint8_t unk31();                               // [31] 0x181A74A40 returns 2 (small enum getter) [U]
    virtual bool unk32();                                  // [32] return false [U]
    virtual void OnExecute(S_NodeExecuteContext const& ctx);  // [33] base nullsub; the node-type work, rttr-registered under the graph name "Execute" (C_Effect 0x1806B13C4, C_If 0x18169333C)
    virtual void OnLifecycleEvent(int32_t event);          // [34] base nullsub; C_AutoTriggerable 0x1806B0C04, C_Effect 0x181E2ED20 [event enum unlocated]
    virtual int32_t MapEventToPhase(int32_t event);        // [35] base 0; C_AutoTriggerable+ 0x1806B0B80: ev {0,1}->1, 5->2, else 0
    virtual bool IsEventSuppressedWhileHibernating(int32_t event);  // [36] 0x1806B0B5C: false for ev in {2,5,7}, true otherwise
    virtual void Reset();                                  // [37] base nullsub; re-init hook invoked by [17]/[18]: C_AutoTriggerable+ 0x180694E8C (re)registers the auto-trigger, C_StateVariable 0x1806B0DC4 re-applies DefaultValue [name LIKELY]
    virtual void UnregisterAutoTrigger();                  // [38] base nullsub; C_AutoTriggerable 0x180F6ED40, C_Effect 0x180F6EDA8
    virtual void ForceDeactivate();                        // [39] base nullsub; C_Effect 0x180ACF0AC emits the falling edge if latched
    virtual void PropagateToPorts(bool propagate);         // [40] base nullsub; C_Effect 0x1813E804C walks m_ports when true [name LIKELY]

    _smart_ptr<I_Port> FindPortByName(CryStringT<char> const& name);    // 0x180699818
    _smart_ptr<I_Port> GetOrCreatePort(CryStringT<char> const& name);   // 0x180699630: existing, else definition direction+type -> concrete C_*Port + AddPort
    bool AddPort(_smart_ptr<I_Port> const& port);                       // 0x180697390: dup check, bind (0x180697E78 plants owner ref + definition weak_ptr), insert (0x181E33520)
    std::map<std::string, _smart_ptr<I_Port>> GetPortsView();           // 0x18270A4D8 -- rttr getter "PortsView"

    CryStringT<char>   m_name;           // +0x10  rttr "Name"
    E_NodeRuntimeState m_runtimeState;   // +0x18  rttr "RuntimeState" (read-only); ctor Hibernating
    int32_t            m_unk1C;          // +0x1C  ctor -1 [role UNVERIFIED -- index/handle sentinel]
    bool               m_unk20;          // +0x20  ctor false [role UNVERIFIED]
    uint8_t            _pad21[7];        // +0x21
    std::vector<_smart_ptr<I_Port>> m_ports;  // +0x28  rttr "Ports"; populated/repaired by Activate; elements Released on destruction (sub_1806B43A4)
};
static_assert(sizeof(C_Node) == 0x40, "C_Node must be 0x40 (derived members start at +0x40)");

}  // namespace wh::conceptmodule
