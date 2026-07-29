#pragma once
#include <cstdint>
#include "C_Signal.h"
#include "ModelPropertyTraits.h"
#include "../combatmodule/CombatModelTraits.h"

// -----------------------------------------------
// wh::shared::C_ModelProperty<T,Def,Sig,Trace,Save,Own> -- KCD2 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// A "model property": one stored value unified with its change-notify C_Signal and its owner
// back-pointer. S_CombatActorState is a flat sequence of these (ctor sub_1810F10CC, field-init
// sub_1810F1D80; GetValue vfunc sub_181A7DE10 returns *(T*)(this+8)). Single-inheritance from the
// polymorphic read-only view I_ReadonlyModelProperty<T,Sig> (4 slots). The value is ALWAYS at
// +0x08 -- code reading a KCD1-style raw field must add 0x08 to the property offset.
// Custom-default instantiations additionally store their default at +0x2C (via the Def trait).

namespace wh::shared {

// Read-only view (4 vtable slots).
template<class T, class Sig>
class I_ReadonlyModelProperty {
public:
    virtual ~I_ReadonlyModelProperty() = default;              // [0]
    virtual const T&    GetValue() const = 0;                  // [1]  sub_181A7DE10: *(T*)(this+8)
    virtual const char* ToDbgStr() const = 0;                  // [2]  sub_1827602FC: "Y"/"N" etc.
    virtual const void* GetSignal() const = 0;                 // [3]  returns &m_signal (sub_181A72EC0 / sub_181A72EB0)
};

template<class T,
         class Def   = traits::C_StandardDefaultValueTrait<T>,
         class Sig   = wh::combatmodule::traits::C_CombatSignalWithNewValueTrait<T>,
         class Trace = wh::combatmodule::traits::C_CombatModelNoTrace,
         class Save  = traits::C_NoSaveLoad,
         class Own   = wh::combatmodule::traits::C_CombatActorModelOwnership>
class C_ModelProperty : public I_ReadonlyModelProperty<T, Sig> {
public:
    const T&    GetValue() const override { return m_value; }
    const char* ToDbgStr() const override { return ""; }
    const void* GetSignal() const override { return &m_signal; }

    T                       m_value;     // +0x08  the stored value
    typename Own::OwnerPtr  m_pOwner;    // +0x10  owning combat actor
    typename Sig::Signal    m_signal;    // +0x18  change-notify (0x10)
    Def                     m_default;   // +0x28  empty for Standard/Static; {reserved,T} -> default @+0x2C for Custom
};
static_assert(sizeof(C_ModelProperty<bool>) == 0x30, "C_ModelProperty must be 0x30");
static_assert(sizeof(C_ModelProperty<int, traits::C_CustomDefaultValueTrait<int>>) == 0x30,
              "C_ModelProperty (custom default) must be 0x30");

// GENERIC form under traits::C_OwnershipEmpty -- the owner slot is ELIDED: value @+0x08, C_Signal
// @+0x10, stride 0x20, no default slot (proven from the C_Player state-property vtables:
// GetValue -> *(u8*)(this+8), GetSignal -> this+0x10, deleting dtor touches only vtable+signal).
template<class T, class Def, class Sig, class Trace, class Save>
class C_ModelProperty<T, Def, Sig, Trace, Save, traits::C_OwnershipEmpty>
    : public I_ReadonlyModelProperty<T, Sig> {
public:
    const T&    GetValue() const override { return m_value; }
    const char* ToDbgStr() const override { return ""; }
    const void* GetSignal() const override { return &m_signal; }

    T                       m_value;     // +0x08
    wh::shared::C_Signal<T> m_signal;    // +0x10  change-notify (0x10)
};
static_assert(sizeof(C_ModelProperty<bool, traits::C_StandardDefaultValueTrait<bool>,
                                     traits::C_SignalWithNewValueTrait<bool>, traits::C_DebugNoTrace,
                                     traits::C_NoSaveLoad, traits::C_OwnershipEmpty>) == 0x20,
              "generic C_ModelProperty (OwnershipEmpty) must be 0x20");

}  // namespace wh::shared
