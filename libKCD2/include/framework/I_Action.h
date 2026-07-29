#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::framework::I_Action -- ref-counted action interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI base chain (from the C_ParamAction<...> RTTI BaseClassArray):
//   _i_multithread_reference_target<int>  ->  I_Action  ->  I_CombatActorAction
//     ->  I_CombatActorActionPrivate  ->  C_Action<TPrivate>  ->  C_ParamAction<...>
// I_ActionImpl is the MI secondary base of C_Action (see C_Action.h).
//
// I_Action inherits CryEngine's MT ref-counted base directly, giving vtable slots
//   [0] ~dtor  [1] AddRef (_InterlockedIncrement at +0x08)  [2] Release (_InterlockedExchangeAdd)
// and then adds the framework action interface, slots [3..25] (23 methods).
//
// [vs KCD1] The KCD1 reference header attributed these public action methods to
// I_CombatActorAction. That is WRONG for KCD2 (and likely for KCD1 too, mis-read as
// "shuffled"): the plain framework action C_Action<I_Action> (used by playermodule dice/
// item actions) exposes these SAME handlers at these SAME slots [3..25] with NO combat
// interface present -- proving they are declared on I_Action, above the combat layer.
// Evidence: C_Action<I_Action> primary vtable 0x18473F028 vs C_Action<I_CombatActorAction-
// Private> 0x183A60988 share slots [1..25] byte-for-byte; only the C_Action-own tail shifts.
//
// Slot handler addresses below are the C_Action<I_CombatActorActionPrivate> implementations
// (vtable 0x183A60988). Names recovered by decompiling each handler; unresolved slots are _vfN.

namespace wh::framework {

class I_Action : public _i_multithread_reference_target<int> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Action;
    // ---- framework action interface (primary vtable slots [3..25]) ----
    virtual bool     IsUniquelyReferenced() const = 0;   // [3]  0x1825049D8  returns m_nRefCounter == 1
    virtual bool     IsStarted() const = 0;              // [4]  0x181A731D0  returns m_isStarted   (+0x28)
    virtual bool     IsRunning() const = 0;              // [5]  0x181A75490  returns flag (+0x29; set on Stop) [KCD1-correlated name]
    virtual bool     IsCompleted() const = 0;            // [6]  0x181A7D930  returns m_isCompleted (+0x2A)
    virtual bool     CanInterrupt(const _smart_ptr<I_Action>& other) const = 0; // [7]  0x1814962EC  !other || other->GetPriority() > m_priority
    virtual bool     _vf8() const = 0;                   // [8]  0x180838AE0  returns false (base default)
    virtual void     _vf9() = 0;                         // [9]  0x1803B6E80  nullsub
    virtual void     _vf10() = 0;                        // [10] 0x1803B6E80  nullsub
    virtual int64_t  _vf11() const = 0;                  // [11] 0x18066CD10  returns 0 (base default)
    virtual bool     _vf12() const = 0;                  // [12] 0x18041A6A0  returns true (base default)
    virtual void     GetActionParams(_smart_ptr<I_Action>& out) const = 0; // [13] 0x180AB965C  copies ref-counted m_pActionParams (+0x30)
    virtual int32_t  GetActionTypeId() const = 0;        // [14] 0x181A72600  returns -1 (base default; leaf overrides)
    virtual int32_t  GetPriority() const = 0;            // [15] 0x181A73A40  returns m_priority          (+0x38)
    virtual int32_t  GetActionSequenceId() const = 0;    // [16] 0x181A72810  returns m_actionSequenceId  (+0x3C)
    virtual int32_t  GetContextPriority() const = 0;     // [17] 0x181A727A0  returns m_contextPriority   (+0x48)
    virtual const char* GetActionName() const = 0;       // [18] 0x181A72B70  returns static name string
    virtual void     SetActionSequenceId(int32_t id) = 0;// [19] 0x181A72920  writes m_actionSequenceId   (+0x3C)
    virtual void     ConnectOnChanged(void* delegate) = 0;    // [20] 0x18090BE68  Connect delegate to m_onActionChanged (+0x18)
    virtual void     DisconnectOnChanged(void* delegate) = 0; // [21] 0x180618538  Disconnect delegate from m_onActionChanged (+0x18)
    virtual void*    AsActionImpl() = 0;                 // [22] 0x181A72EC0  returns this+0x10 (the I_ActionImpl subobject)
    virtual void     _vf23(void* out) const = 0;         // [23] 0x18275AAB0  constructs an out-object (handle/pair)
    virtual void*    GetSelf() = 0;                      // [24] 0x1805F5DA0  returns this
    virtual void     _vf25(void* out) const = 0;         // [25] 0x18275A85C  out = { this, ... } (handle/pair)

    // No data members: _i_multithread_reference_target<int> provides
    //   +0x00 vtable   +0x08 volatile int m_nRefCounter   (+0x0C pad to 0x10)
};
static_assert(sizeof(I_Action) == 0x10, "I_Action = vtable + refcount");

}  // namespace wh::framework
