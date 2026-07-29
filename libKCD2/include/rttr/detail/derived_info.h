#pragma once
#include <cstddef>
#include "rttr/type.h"

// -----------------------------------------------
// rttr::detail::derived_info -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Return type of the RTTR_ENABLE trio's get_derived_info. Every per-class instance
// (e.g. C_UIBase 0x182B038BC) writes {this, get_type()} through the hidden return
// pointer: `*a2 = this; a2[1] = type`. class_data+0x00 m_derived_info_func produces
// one polymorphically -- 0x180D6A220 virtual-calls the trio slot on the object.

namespace rttr {
namespace detail {

struct derived_info {
    void* m_ptr;    // +0x00  most-derived object pointer
    type  m_type;   // +0x08  its runtime type
};
static_assert(sizeof(derived_info) == 0x10, "derived_info is {ptr, type}");
static_assert(offsetof(derived_info, m_type) == 0x08, "type at 0x08");

}  // namespace detail
}  // namespace rttr
