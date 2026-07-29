#pragma once
#include <cstddef>
#include "rttr/string_view.h"
#include "rttr/detail/type_data.h"

// -----------------------------------------------
// rttr::type -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 8 (one type_data*).
// -----------------------------------------------
// master/0.9.7-dev pointer-based design: identity IS the type_data pointer (equality =
// pointer compare). NOT the 0.9.6 id-based type.
//
// ABI: returned BY HIDDEN POINTER on MSVC x64 because the copy ctor is user-provided
// (non-trivially-copyable). Every binary get_type takes the return slot in RDX -- e.g.
// C_UIBase 0x182B039B0 does `mov rcx, rdx ... call helper ... mov rax, rbx`. The
// user-provided copy ctor below is REQUIRED to keep tool-side calls through the
// RTTR_ENABLE trio ABI-correct; do NOT simplify it to =default (that would make the
// type trivially copyable and flip the return convention to RAX).
//
// Accessors below are the PROVEN subset (semantics read straight off type_data);
// the full binary method surface lands with the agent_type_api dossier.

namespace rttr {

class type {
public:
    // upstream: private ctor + friends; public here for tool use.
    explicit type(detail::type_data* data) noexcept : m_type_data(data) {}
    type(const type& other) noexcept : m_type_data(other.m_type_data) {}  // user-provided => hidden-ptr return ABI
    type& operator=(const type& other) noexcept { m_type_data = other.m_type_data; return *this; }

    bool operator==(const type& o) const noexcept { return m_type_data == o.m_type_data; }  // raw ptr compare -- PROVEN 0x182B992D7, 0x1804F6373
    bool operator!=(const type& o) const noexcept { return m_type_data != o.m_type_data; }
    bool operator<(const type& o) const noexcept { return m_type_data < o.m_type_data; }

    bool is_valid() const noexcept { return m_type_data->is_valid; }            // +0xC0 (game gate e.g. 0x181E2E81D)
    // Name (std::string at type_data+0x18; whitespace-normalized at registration). ALL 13
    // in-game name reads use +0x18 (out-of-line impl 0x1823D3690; the +0x38 raw view is
    // only read by rttr-internal name derivation). C_GUIModule's map keyer sub_182B8D8D8.
    string_view get_name() const noexcept { return {m_type_data->name.data(), m_type_data->name.size()}; }
    std::size_t get_sizeof() const noexcept { return m_type_data->get_sizeof; }  // +0x48 (no in-game consumers; layout-pure)
    type get_raw_type() const noexcept { return type(m_type_data->raw_type_data); }
    type get_wrapped_type() const noexcept { return type(m_type_data->wrapped_type); }

    // THE kind-check idiom of the whole game (50+ sites, e.g. 0x1804C441C): raw-type ptr
    // compare, else linear scan of class_data::m_base_types (base-depth sorted).
    // Game-side rttr_cast = `t.is_derived_from(T) ? static_cast<T*>(p) : nullptr` --
    // conversion_list is NEVER applied (no MI pointer adjust in the shipped idiom).
    bool is_derived_from(const type& other) const;   // binary impl 0x1804F6364 (declaration-only)
    // Registry lookup keyed on the SAME normalized name get_name() returns (round-trip
    // proven). Locks type_register_private+0x178; name map at +0x10. 22 game sites.
    static type get_by_name(string_view name);       // binary impl 0x1806A589C (declaration-only)

    bool is_class() const noexcept { return m_type_data->m_type_traits.test(static_cast<std::size_t>(detail::type_trait_infos::is_class)); }
    bool is_enumeration() const noexcept { return m_type_data->m_type_traits.test(static_cast<std::size_t>(detail::type_trait_infos::is_enum)); }
    bool is_array() const noexcept { return m_type_data->m_type_traits.test(static_cast<std::size_t>(detail::type_trait_infos::is_array)); }
    bool is_pointer() const noexcept { return m_type_data->m_type_traits.test(static_cast<std::size_t>(detail::type_trait_infos::is_pointer)); }
    bool is_template_instantiation() const noexcept { return m_type_data->m_type_traits.test(static_cast<std::size_t>(detail::type_trait_infos::is_template_instantiation)); }

    detail::type_data* m_type_data;   // +0x00  the one member
};
static_assert(sizeof(type) == 8, "rttr::type is one type_data*");

}  // namespace rttr
