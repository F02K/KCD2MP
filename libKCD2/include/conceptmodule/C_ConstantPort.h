#pragma once
#include "C_OutputDataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_ConstantPort -- XML literal pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x48, vtable 0x183A46A90 (32 slots).
// -----------------------------------------------
// Created by C_ConceptGraphDeserializer::CreatePorts (0x1806A03D4) for
// <Constant Name="X" Value="..."/>: ctor 0x1806B24F4(name, std::string(value)) wraps
// the RAW literal text into an rttr::variant with the std::string policy
// (0x1804F9D78) -- NO parsing at load; the string -> S_TimeSpan/int/bool/...
// conversion happens lazily at the consumer via the registered rttr
// type_converters (C_TypedPortRef<T>::Get() -> variant::convert<T>()).
// GetValue [16] = 0x18061EF2C: plain variant copy.  Resets IsPassThrough [10] and
// [11] to false (a constant never forwards upstream).  Alloc 0x48 @0x1806B2557.

namespace wh::conceptmodule {

class C_ConstantPort : public C_OutputDataPort {
public:
    RTTR_ENABLE(C_OutputDataPort)      // [5] 0x1806B1C68, [7] 0x1819508B8
    rttr::variant GetValue() override; // [16] 0x18061EF2C copy m_value verbatim

    rttr::variant m_value;   // +0x30  the literal (std::string policy until converted)
};
static_assert(sizeof(C_ConstantPort) == 0x48, "C_ConstantPort must be 0x48");

}  // namespace wh::conceptmodule
