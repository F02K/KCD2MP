#pragma once
#include <cstddef>
#include "rttr/type.h"

// -----------------------------------------------
// rttr::detail::base_class_info -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Element of the per-type static vector returned by type_data+0x60 get_base_types.
// Layout PROVEN corpus-wide (agent_type_data_fields): pair stores in the base-list
// lambdas (e.g. 0x18194BCD4, C_UIMenu's 0x180EE4C90 writes {type, cast} at +0x00/+0x08
// with 0x10 stride), register_type's `size & ~0xF` free, and register_base_class_info
// 0x1806A7C94 striding two qwords per element.
// The cast fn is rttr_cast_impl<Base, Derived> (static_cast pointer adjust): identity
// casts COMDAT-fold into 0x1805F5DA0 (the same `return this` as the trio's get_ptr);
// non-zero-offset bases get real adjustor fns (e.g. 0x181F47840 for C_UIMenu ->
// wh::I_UIMenu at +0x58).

namespace rttr {
namespace detail {

using rttr_cast_func = void* (*)(void*);

struct base_class_info {
    type           m_base_type;       // +0x00
    rttr_cast_func m_rttr_cast_func;  // +0x08
};
static_assert(sizeof(base_class_info) == 0x10, "base_class_info is {type, cast fn}");

}  // namespace detail
}  // namespace rttr
