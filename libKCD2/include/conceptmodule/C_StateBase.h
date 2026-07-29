#pragma once
#include "C_TemplatedNode.h"

// -----------------------------------------------
// wh::conceptmodule::C_StateBase<T> -- stateful node mixin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  Adds T at +0x48 and 2 vtable slots.
// -----------------------------------------------
// Instantiations (RTTI-proven): <rttr::variant> -> C_StateVariable (the generic
// "State" node); <rpgmodule::E_TimerState> -> rpgmodule::C_Timer;
// <rpgmodule::E_TimeOfDayState> -> C_TimeOfDayWatch; <framework::
// E_GameReleaseVersion> -> C_ModuleVersionState; <playermodule::
// E_SaveGameWithNotificationState> -> playermodule::C_SaveGameWithNotification.
// The <rttr::variant> instantiation's GetPortDefinitions (0x1804F5E14) synthesizes
// the dynamic pin set: global "OnExec" out-trigger, and for an ENUM TypeT three
// pins per enumerator -- "Set<E>" (In trigger), "On<E>" (Out trigger), "<E>" (bool
// Out data).  Change flow: [30] applies a new value with change detection and,
// when changed, calls OnStateChanged [42], which fires the matching On* triggers
// then always "OnExec".

namespace wh::conceptmodule {

template <typename T>
class C_StateBase : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] <variant>: 0x1804F5E14 dynamic enum pins
    virtual void OnStateChanged(T const& oldValue, T const& newValue, bool changed);  // [+0] <variant>: 0x18061C19C fires On*/OnIncrease/OnTrue/... then OnExec [sig LIKELY]
    virtual bool IsAtDefaultValue();   // [+1] <variant>: 0x1808B1208 compare against the DefaultValue pin

    T m_value;   // +0x48  the state payload
};

}  // namespace wh::conceptmodule
