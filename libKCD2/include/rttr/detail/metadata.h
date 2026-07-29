#pragma once
#include "rttr/variant.h"

// -----------------------------------------------
// rttr::detail::metadata -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// {variant m_key, variant m_value}: ctor 0x1806E1750 moves arg2 -> +0x00 and arg3 ->
// +0x18; dtor 0x1806E1440 invokes the policies at +0x10/+0x28; element stride 0x30 in
// every array helper. WH metadata KEYS are not strings -- enum
// wh::conceptmodule::E_ConceptFunctionMetadata stored inline as int32 (policy
// 0x1804FA438); string VALUES use the std::string big-policy 0x1804F9D78. Consumer:
// metadata_handler<N>::get_metadata COMDATs 0x1804F8A7C (N=3, base+0x90) / 0x1804F8468
// (N=8, base+0xF0) -- key compare is a FULL variant operator== (policy op 0x12), found
// value returned as a CLONE of m_value. Class-level metadata (type_data+0x70 vector) is
// lazily heap-built empty -- the real metadata lives in method wrappers
// (layout: ...+0x50 = metadata[N], see agent_metadata_variant.md).

namespace rttr {
namespace detail {

class metadata {
public:
    variant m_key;     // +0x00
    variant m_value;   // +0x18
};
static_assert(sizeof(metadata) == 0x30, "metadata is two variants");

}  // namespace detail
}  // namespace rttr
