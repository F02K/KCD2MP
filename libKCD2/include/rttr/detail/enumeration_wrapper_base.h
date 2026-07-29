#pragma once
#include "rttr/type.h"
#include "rttr/string_view.h"
#include "rttr/variant.h"

// -----------------------------------------------
// rttr::detail::enumeration_wrapper_base -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Base of enumeration_wrapper<Enum,N,MetadataCount> (base vtable 0x183A63ED0,
// RTTI-named; 452 instantiations = 226 enum types, ALL MetadataCount=0 -> the
// metadata_handler base occupies zero bytes). Derived layout beyond this base:
// std::array<string_view,N> m_names @+0x10, std::array<Enum,N> m_values @+0x10+0x10*N,
// std::array<variant,N> m_var_values next 8-aligned; alloc size = 0x10 + 0x10*N +
// align8(4*N) + 0x18*N (validated against N=0/3/4/6/22 allocations). Always
// heap-allocated, owned by registration_manager's unique_ptr vector (+0xB0, singleton
// 0x18549D6F0); type_data+0x68 is wired in add_item 0x180929844 @0x18092986F using this
// wrapper's own get_type() [3]. Extracted registry (202/226 enums, 1,322 name/value
// pairs): analysis/rttr/enum_registry_kcd2.json; dossier: agent_enum_wrapper.md.
// Vfunc ret/arg shapes beyond [0]/[1]/[3] follow upstream order; sret out-params
// modeled as void* [shapes UNVERIFIED].

namespace rttr {
namespace detail {

class enumeration_wrapper_base {
public:
    virtual ~enumeration_wrapper_base();                                    // [0] scalar-deleting dtor
    virtual bool is_valid() const;                                          // [1] base `xor al,al`; derived `mov al,1`
    virtual type get_underlying_type() const;                               // [2]
    virtual type get_type() const;                                          // [3] the registered enum type
    virtual void get_names(void* outArrayRange) const;                      // [4] sret array_range<string_view> [shape UNVERIFIED]
    virtual void get_values(void* outArrayRange) const;                     // [5] sret array_range [shape UNVERIFIED]
    virtual void value_to_name(void* outStringView, void* argument) const;  // [6] scans m_values -> m_names [shape UNVERIFIED]
    virtual void name_to_value(void* outVariant, string_view name) const;   // [7] scans m_names -> m_values [shape UNVERIFIED]
    virtual void get_metadata(void* outVariant, const variant& key) const;  // [8] [inferred from upstream order; no non-trivial impl exists]

    type m_declaring_type;   // +0x08  seeded invalid @0x1803886A6, overwritten @0x180388964
};
static_assert(sizeof(enumeration_wrapper_base) == 0x10, "vptr + declaring type");

}  // namespace detail
}  // namespace rttr
