#pragma once
#include "C_PortRef.h"
#include "C_StateBase.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"
#include "rttr/variant.h"

// -----------------------------------------------
// wh::conceptmodule::C_StateVariable -- the Skald "State" node
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x160, vtable 0x183A45A40
// (44 slots).  10,212 corpus uses (tag `State` -- rttr name strips both the C_
// prefix AND the Variable suffix: "wh::conceptmodule::State", reg sub_180FDBD80).
// -----------------------------------------------
// Ctor 0x1806B1E44, creator 0x1806B15E0.  A typed graph variable: value lives in
// the inherited rttr::variant at +0x48.  Lifecycle: OnLifecycleEvent [34]
// 0x1806B0CA8 (event 5) initializes from the DefaultValue pin (else
// default-construct/first enumerator of TypeT); Reset [37] 0x1806B0DC4 re-applies
// it.  OnExecute [33] 0x18061E91C maps the FIRED PORT'S NAME to the operation:
// Exec -> set from Value pin; int TypeT -> Increment/Decrement; bool ->
// SetTrue/SetFalse; enum -> strip "Set" prefix, map to enumerator (GetPortValue
// [12] 0x1806ACC4C does name -> enumerator).  Changes route through [30]
// 0x1808B0F8C (change detect) -> OnStateChanged [42] 0x18061C19C: fires
// OnIncrease/OnDecrease | OnTrue/OnFalse | "On"+enumerator, then always OnExec.
// OnTypeTSet [41] 0x1806A75C0 builds the default variant for the new type.
// Persistence: no per-node save virtual located [open question].

namespace wh::conceptmodule {

class C_StateVariable : public C_StateBase<rttr::variant> {
public:
    RTTR_ENABLE(C_StateBase<rttr::variant>)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1806ACC4C Set<E> name -> enumerator value
    void EnumerateNodeVariants() override;    // [27] 0x180691160 descriptor "State", category 7
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x1804F029C base + int: Increment/Decrement/OnIncrease/OnDecrease, bool: SetTrue/SetFalse/OnTrue/OnFalse
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18061E91C fired-port-name -> operation
    void OnLifecycleEvent(int32_t event) override;  // [34] 0x1806B0CA8 event 5: init from DefaultValue
    void Reset() override;                    // [37] 0x1806B0DC4 re-apply DefaultValue (no notify)
    void OnTypeTSet(bool valid) override;     // [41] 0x1806A75C0 default-construct m_value for the new type
    void OnStateChanged(rttr::variant const& oldValue, rttr::variant const& newValue, bool changed) override;  // [42] 0x18061C19C fire On* then OnExec
    bool IsAtDefaultValue() override;         // [43] 0x1808B1208

    C_PortRef m_state;                 // +0x60  rttr "State"
    C_PortRef m_defaultValue;          // +0xA0  rttr "DefaultValue" (also tagged "RandomEventPlace" in the registration [purpose U])
    C_PortRef m_valuePort;             // +0xE0  rttr "Value" -- In (set source)
    C_TypedPortRef<S_Trigger> m_exec;  // +0x120 rttr "Exec" -- In trigger (ctor retypes the C_PortRef)
};
static_assert(sizeof(C_StateVariable) == 0x160, "C_StateVariable must be 0x160");

}  // namespace wh::conceptmodule
