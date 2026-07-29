#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_InjectionExternalDataProvider -- injection external-data
// provider interface; tertiary MI base of C_SituationSubsystem (@+0x10) (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.  Counterpart of
// I_InjectionExternalDataUpdateListener.
// -----------------------------------------------
// RTTI mangled .?AVI_InjectionExternalDataProvider@xgenaimodule@wh@@.  No
// standalone vtable emitted (embedded-only); shape (3 slots) read from the
// C_SituationSubsystem tertiary vtable rva 0x3A83160: [0]sub_180BD93D8
// [1]sub_180838AE0 [2]sub_1813CC900.  Slot semantics NOT decompiled [U] --
// modeled as 3 pure slots (no dtor slot claimed [U]).

namespace wh::xgenaimodule {

class I_InjectionExternalDataProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InjectionExternalDataProvider;
    virtual void IedpVf0() = 0;   // [0] [U role]
    virtual void IedpVf1() = 0;   // [1] [U role]
    virtual void IedpVf2() = 0;   // [2] [U role]
};
static_assert(sizeof(I_InjectionExternalDataProvider) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
