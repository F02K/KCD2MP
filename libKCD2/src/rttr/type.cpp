#include "rttr/type.h"
#include "Offsets/Offsets.h"

// rttr::type binary forwarders -- KCD2 WHGame.dll 1.5.6 (kd7u), cross-version via
// REL::ID (ids resolved through analysis/addresslib/address_library/kcd2_id_registry.csv;
// steam RVAs
// noted per function). ABI shapes are the agent_type_api PROVEN signatures.

namespace rttr {

bool type::is_derived_from(const type& other) const
{
    // 0x1804F6364 -- raw_type pointer compare, else linear scan of the SORTED
    // class_data::m_base_types. THE kind-check idiom of the game (50+ call sites);
    // the shipped rttr_cast pattern is `is_derived_from ? static_cast : nullptr`
    // with no pointer adjustment.
    using Fn = bool(__fastcall*)(const type*, const type*);
    static REL::Relocation<Fn> fn{ REL::ID(29665) };
    return fn(this, &other);
}

type type::get_by_name(string_view name)
{
    // 0x1806A589C -- FNV-ish hash + binary search of the flat_map at
    // type_register_private+0x10 under the +0x178 mutex; miss returns the invalid
    // type (singleton 0x18549D860). Keyed on the SAME normalized name get_name()
    // returns, so get_by_name(t.get_name()) == t round-trips (22 game call sites).
    // ABI: static fn, hidden return slot in RCX, string_view by pointer in RDX.
    using Fn = type*(__fastcall*)(type*, const string_view*);
    static REL::Relocation<Fn> fn{ REL::ID(36975) };
    type result{ nullptr };
    fn(&result, &name);
    return result;
}

}  // namespace rttr
