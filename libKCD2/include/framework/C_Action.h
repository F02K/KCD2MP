#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/smartptr.h"
#include "I_Action.h"
#include "I_ActionImpl.h"
#include "C_Signal.h"

// -----------------------------------------------
// wh::framework::C_Action<TPrivate> -- concrete action base (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x50.
// -----------------------------------------------
// MI:  primary base TPrivate (the action interface: I_Action, or the combat interface chain
// I_CombatActorActionPrivate / ...Block) at +0x00, and I_ActionImpl at +0x10.
//
// ctor sub_18090D31C(this, owner); dtor sub_18090D288 (Releases m_pActionParams, tears down the
// signal, downgrades the vtable). Bare C_Action is abstract (OnStart/OnStop are pure) and is only
// built as the base subobject of a leaf action by its ctor.
//
// C_Action adds NINE OWN primary virtuals AFTER TPrivate's slots (proved by diffing the plain
// C_Action<I_Action> tail [26..34] against the combat C_Action<...Private> tail [56..64] -- the
// same 9 handlers, shifted by the 30 combat-interface slots). Two are pure lifecycle hooks the
// leaf must implement (OnStart/OnStop); the rest default to no-ops / debug reflection stringifiers.
//
// Verified instantiations:
//   C_Action<I_CombatActorActionPrivate>  primary 0x183A60988 (65 slots)  secondary 0x183A60948 (7)
//   C_Action<I_CombatActorActionBlock>     primary 0x183A89F48 (67 slots)  secondary 0x183A8A3F0
//   C_Action<I_Action>                      primary 0x18473F028 (35 slots)  secondary 0x18473EFE8
//
// [FUNDAMENTAL vs KCD1] Shrank 0xD0 -> 0x50. KCD1 had THREE 0x30 C_Signals (+0x18/+0x48/+0x78);
// KCD2 keeps ONE 0x10 C_Signal (+0x18) -- C_Signal de-virtualized 0x30 -> 0x10 AND the two extra
// heads were removed. All trailing members moved up: params +0xB0->+0x30, priority +0xB8->+0x38,
// seq +0xBC->+0x3C, owner +0xC0->+0x40, contextPriority +0xC8->+0x48.

namespace wh::framework {

template<typename TPrivate>
class C_Action : public TPrivate, public I_ActionImpl {
public:
    // ---- C_Action's own primary virtuals (appended after TPrivate's slots) ----
    // slot indices below are for C_Action<I_CombatActorActionPrivate> (56..64).
    virtual void        _vfOwn0() = 0;                   // [56] 0x1803B6E80 (base nullsub; dtor hook target of I_ActionImpl[0])
    virtual bool        OnStart() = 0;                   // [57] _purecall  concrete-action start hook (driven by I_ActionImpl::Start)
    virtual void        OnStop() = 0;                    // [58] _purecall  concrete-action stop  hook (driven by I_ActionImpl::Stop)
    virtual void        OnUpdate() = 0;                  // [59] 0x1803B6E80 nullsub default (driven by I_ActionImpl::Update)
    virtual int64_t     OnTick() = 0;                    // [60] 0x18066CD10 returns 0 default (driven by I_ActionImpl::Tick)
    virtual const char* GetDebugName() const = 0;        // [61] 0x181A72B70 returns the static action-name string
    virtual const char* GetPriorityString() const = 0;   // [62] 0x182756708 sprintf("%d", GetPriority()) -> static buffer
    virtual const char* GetSeqIdString() const = 0;      // [63] 0x1827568F0 sprintf("%d", GetActionSequenceId()) -> static buffer
    virtual void        GetNameString(void* out) const = 0; // [64] 0x1808D2480 builds a string object from the static name

    // ---- data (0x18..0x50) ----
    // +0x00 vtable(primary), +0x08 m_nRefCounter (from _i_multithread_reference_target),
    // +0x10 vtable(I_ActionImpl secondary) -- all supplied by the base subobjects above.
    wh::shared::C_Signal<I_Action&, _smart_ptr<I_Action>&>
                m_onActionChanged;         // +0x18  emitted on Stop (0x10)
    bool        m_isStarted;               // +0x28  set 1 by Start, 0 by Stop
    bool        m_isRunning;               // +0x29  set 1 by Stop (KCD1-correlated name; role UNVERIFIED)
    bool        m_isCompleted;             // +0x2A  set by Stop(reason == 1)
    uint8_t     _pad2B[5];                 // +0x2B
    _smart_ptr<I_Action> m_pActionParams;  // +0x30  ref-counted params/definition action; assigned via _smart_ptr op= (sub_1809CCE24: AddRef slot[1]/Release slot[2]); Released (slot[2]) in dtor sub_18090D288
    int32_t     m_priority;                // +0x38  ctor init -1
    int32_t     m_actionSequenceId;        // +0x3C  ctor init -1
    void*       m_pOwnerSubsystem;         // +0x40  ctor arg (creating subsystem)
    int32_t     m_contextPriority;         // +0x48  ctor init 0; recomputed in Start
    uint32_t    _pad4C;                    // +0x4C
};
static_assert(sizeof(C_Action<I_Action>) == 0x50, "C_Action base must be 0x50");

}  // namespace wh::framework
