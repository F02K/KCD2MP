#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <bitset>
#include "rttr/string_view.h"

// -----------------------------------------------
// rttr::detail::type_data -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC8 (heap).
// -----------------------------------------------
// One per registered type. Built lazily by each type::get<T> instantiation: TLS
// magic-static helper -> per-type creator (allocates 0xC8; e.g. C_UIBase 0x1816391F4,
// C_PlayerData 0x18176988C) -> registration_manager @0x18549D6F0 add_item 0x180929084
// -> type_register_private @0x18549D950 register_type 0x1806A7A8C. The binary carries
// 7,423 creators (corpus census, agent_type_data_fields: every field write of every
// creator was machine-extracted); 1,426 of them are vtable-reachable trio classes
// (analysis/rttr/reflected_classes.json). The +0x48 sizeof immediate in each creator
// is the binary's own statement of sizeof(T), used to mass-verify RE header sizes.
//
// Base layout is upstream rttr master (0.9.7-dev). TWO WARHORSE DEVIATIONS (proven):
//  * get_metadata at +0x70 is a std::function (0x40 bytes, SSO impl slot at +0xA8)
//    instead of upstream's plain fn ptr -- proven by the
//    _Func_impl_no_alloc<lambda, std::vector<metadata>&> vtable each creator stores at
//    +0x70 (e.g. 0x183E18820). This is the +0x38 size delta vs upstream's 0x90.
//  * two extra bool flags at +0xC1/+0xC2 (upstream ends with just is_valid + traits).
//
// Invalid-type singleton: type_data @0x18549D860 (ctor 0x180C0CE08, getter
// 0x180C0CDA4; raw/wrapped/array point at itself, is_valid = 0). Class creators SEED
// +0x00/+0x08/+0x10 with the invalid singleton (three 0x180C0CDA4 calls); registration
// then normalizes them (raw type of a plain class is itself).

namespace rttr {
class variant;
class argument;

namespace detail {
class metadata;
class enumeration_wrapper_base;
struct class_data;
struct base_class_info;   // {type, rttr_cast_func} 0x10 -- rttr/detail/base_class_info.h

// Upstream type_trait_infos bit order -- CONFIRMED in-binary over all 7,423 creators
// (corpus counts): bit0 is_class (4,197), bit1 is_enum (234), bit2 is_array (never set
// by any registered type, but PROVEN semantically: derive_name 0x1804F3CBC gates its
// array branch on traits&4), bit3 is_pointer (2,973 -- zero violations vs
// get_pointer_dimension), bit4 is_arithmetic (16 fundamentals; wchar_t creator
// 0x18195A380), bit8 is_associative_container (27), bit9 is_sequential_container (244),
// bit10 is_template_instantiation (2,585 -- custom-name re-derivation gates on 0x400).
// Bits 5/6/7 (function/member-object/member-function pointer) are POSITIONALLY INFERRED
// from upstream order -- never set and never tested in this binary.
enum class type_trait_infos : std::size_t {
    is_class = 0,
    is_enum = 1,
    is_array = 2,
    is_pointer = 3,
    is_arithmetic = 4,
    is_function_pointer = 5,
    is_member_object_pointer = 6,
    is_member_function_pointer = 7,
    is_associative_container = 8,
    is_sequential_container = 9,
    is_template_instantiation = 10,
    TYPE_TRAIT_COUNT = 11
};

struct type_data {
    type_data* raw_type_data;        // +0x00  pointer types: the cv/ptr-stripped pointee's type (e.g. C_UIBase*
                                     //        creator 0x180D6D138 stores type::get<C_UIBase>); non-pointer
                                     //        creators seed the invalid singleton and register_type patches it
                                     //        to self @0x1806A7AF1 (when seeded raw's is_valid == 0)
    type_data* wrapped_type;         // +0x08  wrappers: wrapper_mapper<W>::wrapped_type (shared_ptr<C_UIBase> ->
                                     //        `C_UIBase*`; std::optional<int> -> `int`); else invalid
    type_data* array_raw_type;       // +0x10  read only under traits bit2 (derive_name array branch); invalid
                                     //        for all 7,423 registered types in this binary
    std::string name;                // +0x18  starts as the raw __FUNCSIG__ slice (range-ctor 0x1806A4FC8);
                                     //        re-derived (whitespace-stripped) at first registration -- see
                                     //        should_derive_name below
    string_view type_name;           // +0x38  raw spelling "class wh::guimodule::C_UIBase" {ptr +0x38, len +0x40}
                                     //        over the SAME literal the string was built from
    std::size_t get_sizeof;          // +0x48  sizeof(T), imm in every creator (0x10 C_UIBase, 0x30 C_PlayerData)
    std::size_t get_pointer_dimension; // +0x50  only 0 or 1 occurs in-binary (2,973 pointer types)
    variant (*create_variant)(const argument&); // +0x58  shared 0x180C82A70 for ALL 4,450 non-pointer types
                                     //        (builds an EMPTY variant: writes policy 0x1804FBC18 at out+0x10);
                                     //        each of the 2,973 pointer types has its own impl (e.g. 0x182B9217C
                                     //        copies the 8-byte pointer + installs a per-T* policy)
    std::vector<base_class_info> (*get_base_types)(); // +0x60  returns BY VALUE (register_type calls and frees
                                     //        it @0x1806A7AB5..CB); 0x180504820 = shared empty for baseless.
                                     //        Element = {type, rttr_cast_func} 0x10, PROVEN (pair stores
                                     //        0x18194BCD4; register_base_class_info 0x1806A7C94 strides two qwords)
    enumeration_wrapper_base* enum_wrapper; // +0x68  0 in ALL creators (even the 234 enums); written solely by
                                     //        registration_manager::add_enumeration 0x180929844
                                     //        (`w->vtbl[3](w,&t); t->enum_wrapper = w`)
    std::function<std::vector<metadata>&()> get_metadata; // +0x70..0xAF  WARHORSE DEVIATION (upstream: fn ptr);
                                     //        SSO impl ptr at +0xA8 == self+0x70 in all 7,423 creators
    variant (*create_wrapper)(const argument&); // +0xB0  0 for plain classes; NON-NULL for wrapper types
                                     //        (shared_ptr<T>: 0x183367458, stored @0x1816D0317)
    class_data& (*get_class_data)(); // +0xB8  per-type magic-static class_data factory (C_UIBase: 0x1816DE460;
                                     //        7,423 unique instances)
    bool is_valid;                   // +0xC0  creator writes word {1,0} at +0xC0 -> is_valid=1, has_custom_name=0
    bool has_custom_name;            // +0xC1  [name coined] set ONLY by register_custom_name 0x1804F1DD8
                                     //        @0x1804F1E72 (+ inlined copy 0x1804F36ED). Dispute SETTLED by
                                     //        exhaustive 1,759-site disp32-0xC1 scan: 4 game-code readers, all
                                     //        inlining `has_custom_name || (traits & 0x10)` over class_data
                                     //        type lists (+0x20/+0x98): 0x182095881, 0x180EE51FF, 0x182627CAD,
                                     //        0x182735077; move-ctor 0x1809291A8 also byte-copies it
    bool should_derive_name;         // +0xC2  [name coined] 1 in every creator, 0 only in the invalid singleton;
                                     //        read exactly once: find_or_insert 0x1806A7944 gates
                                     //        `name = derive_name(...)` on it (i.e. "name still holds the raw
                                     //        compiler spelling") [WARHORSE EXTRA vs upstream]
    std::bitset<static_cast<std::size_t>(type_trait_infos::TYPE_TRAIT_COUNT)>
        m_type_traits;               // +0xC4  dword bitset: 0x001 plain class, 0x401 template class (see the
                                     //        corpus bit table above; move-ctor 0x1809291A8 copies it as a DWORD)
};

static_assert(sizeof(std::string) == 0x20, "MSVC std::string is 0x20");
static_assert(sizeof(std::function<std::vector<metadata>&()>) == 0x40, "MSVC std::function is 0x40");
static_assert(sizeof(std::bitset<11>) == 4, "bitset<11> packs into one unsigned long");
static_assert(offsetof(type_data, name) == 0x18, "name at 0x18 (read by C_GUIModule map keyer sub_182B8D8D8)");
static_assert(offsetof(type_data, type_name) == 0x38, "type_name at 0x38 (creator stores ptr/len)");
static_assert(offsetof(type_data, get_sizeof) == 0x48, "get_sizeof at 0x48 (creator imm)");
static_assert(offsetof(type_data, create_variant) == 0x58, "create_variant at 0x58");
static_assert(offsetof(type_data, enum_wrapper) == 0x68, "enum_wrapper at 0x68");
static_assert(offsetof(type_data, get_metadata) == 0x70, "get_metadata std::function at 0x70");
static_assert(offsetof(type_data, create_wrapper) == 0xB0, "create_wrapper at 0xB0");
static_assert(offsetof(type_data, get_class_data) == 0xB8, "get_class_data at 0xB8");
static_assert(offsetof(type_data, is_valid) == 0xC0, "is_valid at 0xC0");
static_assert(offsetof(type_data, m_type_traits) == 0xC4, "type traits dword at 0xC4");
static_assert(sizeof(type_data) == 0xC8, "type_data must be 0xC8 (every creator allocates 0xC8)");

}  // namespace detail
}  // namespace rttr
