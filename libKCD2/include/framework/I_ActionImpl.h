#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::framework::I_ActionImpl -- action lifecycle interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// The MI SECONDARY base of C_Action<TPrivate>; its vtable lives at object offset +0x10.
// The lifecycle entry points here are thin adjustor stubs (this -= 0x10) that drive the
// C_Action state (flags at +0x28..+0x2A, the +0x18 signal) and then forward into the
// concrete-class hooks on the PRIMARY vtable (C_Action-own OnStart/OnStop/OnUpdate/OnTick).
//
// Verified from C_Action<I_CombatActorActionPrivate>: secondary vtable 0x183A60948 (7 slots),
// COL 0x184161B68.  a1 = this(+0x10) in every handler; a1-0x10 = the C_Action base.
//
// [vs KCD1] Shrank from 8 slots to 7: KCD1's separate Complete() slot ([5]) is gone --
// completion is now folded into Stop(reason) (Stop sets m_isCompleted = (reason == 1)).

namespace wh::framework {

class I_Action;

class I_ActionImpl {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ActionImpl;
    virtual ~I_ActionImpl() = default;                       // [0] 0x1818641A8  adjustor -> primary slot 56 (C_Action dtor hook)
    virtual bool Start() = 0;                                // [1] 0x18090D4F8  m_isStarted=1; sets m_contextPriority; calls OnStart (primary 57); false if OnStart fails
    virtual void Stop(int32_t reason) = 0;                   // [2] 0x18090D578  m_isStarted=0; m_isCompleted=(reason==1); emits m_onActionChanged; calls OnStop (primary 58)
    virtual void Update() = 0;                               // [3] 0x1813E6354  forwards to OnUpdate (primary slot 59)
    virtual void Tick() = 0;                                 // [4] 0x180737B70  forwards to C_Action-own hook (primary slot 60)
    virtual bool IsStarted() const = 0;                      // [5] 0x1821390F4  forwards to primary IsStarted (slot 4)
    virtual void GetParams(_smart_ptr<I_Action>& out) const = 0; // [6] 0x180D4BD4C  copies ref-counted params from base+0x30
};
static_assert(sizeof(I_ActionImpl) == 0x08, "I_ActionImpl is vtable-only");

}  // namespace wh::framework
