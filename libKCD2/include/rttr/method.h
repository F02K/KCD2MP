#pragma once

// -----------------------------------------------
// rttr::method -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 8.
// -----------------------------------------------
// Single-pointer front-end handle over a method_wrapper_base (upstream shape; the
// binary's class_data +0x68 vector holds these -- registration appends via
// 0x1806A7F88(type_data, 0x68)). Method surface pending agent dossiers.

namespace rttr {
namespace detail {
class method_wrapper_base;
}  // namespace detail

class method {
public:
    const detail::method_wrapper_base* m_wrapper;   // +0x00
};
static_assert(sizeof(method) == 8, "method is one wrapper ptr");

}  // namespace rttr
