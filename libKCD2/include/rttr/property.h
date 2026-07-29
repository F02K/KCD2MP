#pragma once

// -----------------------------------------------
// rttr::property -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 8.
// -----------------------------------------------
// Single-pointer front-end handle over a property_wrapper_base (upstream shape; the
// binary's class_data +0x50 vector holds these -- registration update_class_list
// 0x1806A84E8 appends to it). Method surface pending agent dossiers.

namespace rttr {
namespace detail {
class property_wrapper_base;
}  // namespace detail

class property {
public:
    const detail::property_wrapper_base* m_wrapper;   // +0x00
};
static_assert(sizeof(property) == 8, "property is one wrapper ptr");

}  // namespace rttr
