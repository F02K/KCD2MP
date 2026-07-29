#pragma once
#include <cstddef>
#include <vector>
#include "rttr/type.h"
#include "rttr/detail/derived_info.h"
#include "rttr/detail/base_class_info.h"
#include "rttr/property.h"
#include "rttr/method.h"
#include "rttr/constructor.h"
#include "rttr/destructor.h"

// -----------------------------------------------
// rttr::detail::class_data -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xB8 (heap).
// -----------------------------------------------
// Byte-for-byte upstream rttr master. One per registered CLASS, created lazily by the
// per-class get_class_data factory stored at type_data+0xB8 (magic-static; e.g.
// C_UIBase 0x1816DE460 -> alloc 0x18092C004, ctor 0x18092C0BC). Populated during
// registration: register_base_class_info 0x1806A7C94 fills m_base_types +
// m_conversion_list (parallel arrays, base-depth sorted -- VERIFIED, comparator
// 0x1808558CC), update_class_list 0x1806A84E8 appends properties (+0x50) and
// methods (+0x68, via 0x1806A7F88(td, 0x68)). m_derived_types is back-filled by each
// derived class's registration.

namespace rttr {
namespace detail {

// rttr_cast_func comes from base_class_info.h
using get_derived_info_func = derived_info (*)(void*);  // binary-observed shape

struct class_data {
    get_derived_info_func       m_derived_info_func;  // +0x00  0x180D6A220: virtual-calls the trio slot on the object
    std::vector<type>           m_base_types;         // +0x08  all transitive bases, sorted by the is_base_of
                                                      //        relation (std::sort @0x1806A7DA1; comparator
                                                      //        0x1808558CC: raw-type equality, else membership
                                                      //        scan of a's m_derived_types). NOT pointer-sorted.
                                                      //        is_derived_from 0x1804F6364 scans it linearly
    std::vector<type>           m_derived_types;      // +0x20  back-filled by derived registrations
    std::vector<rttr_cast_func> m_conversion_list;    // +0x38  PARALLEL to m_base_types (same loop, same index --
                                                      //        PROVEN, agent_class_data_casts); the shipped game
                                                      //        cast idiom never applies these (static_cast only)
    std::vector<property>       m_properties;         // +0x50
    std::vector<method>         m_methods;            // +0x68
    std::vector<constructor>    m_ctors;              // +0x80
    std::vector<type>           m_nested_types;       // +0x98  move-ctor'd in by 0x18092C0BC
    destructor                  m_dtor;               // +0xB0  -> shared static destructor_wrapper_base OBJECT
                                                      //        0x1849302E0 (its vptr = named vtable
                                                      //        ??_7destructor_wrapper_base 0x183D2F9C0)
};

static_assert(sizeof(std::vector<type>) == 0x18, "MSVC std::vector is 0x18");
static_assert(offsetof(class_data, m_base_types) == 0x08, "base types at 0x08");
static_assert(offsetof(class_data, m_conversion_list) == 0x38, "conversion list at 0x38");
static_assert(offsetof(class_data, m_properties) == 0x50, "properties at 0x50 (update_class_list 0x1806A84E8)");
static_assert(offsetof(class_data, m_methods) == 0x68, "methods at 0x68 (0x1806A7F88(td,0x68))");
static_assert(offsetof(class_data, m_ctors) == 0x80, "ctors at 0x80");
static_assert(offsetof(class_data, m_nested_types) == 0x98, "nested types at 0x98");
static_assert(offsetof(class_data, m_dtor) == 0xB0, "dtor at 0xB0");
static_assert(sizeof(class_data) == 0xB8, "class_data must be 0xB8 (alloc 0x18092C004)");

}  // namespace detail
}  // namespace rttr
