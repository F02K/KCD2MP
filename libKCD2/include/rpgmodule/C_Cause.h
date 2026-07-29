#pragma once
#include "I_Cause.h"

// -----------------------------------------------
// wh::rpgmodule::C_Cause<TData> -- typed RPG event-cause carrier (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// ~100 instantiations in RTTI (rtti_map.json `?$C_Cause@US_...Data@rpgmodule@wh@@`).  Layout
// PROVEN by C_Cause<S_FallDamageData> (TD 0x184D5D720, own vtable 0x183A4B320): factory
// sub_1807252E8 operator new(32) copies the 24-byte payload to +0x08 -- i.e. the template
// stores TData inline after the vfptr.  Some instantiations are used most-derived, others
// (e.g. C_Cause<S_GenericReputationChangeData>, TD 0x184D4DED0, no vtable) only as bases of
// named subclasses.

namespace wh::rpgmodule {

template <typename TData>
class C_Cause : public I_Cause {
public:
    TData m_data;   // +0x08
};

}  // namespace wh::rpgmodule
