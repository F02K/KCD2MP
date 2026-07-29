#pragma once

// -----------------------------------------------
// rttr::destructor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 8.
// -----------------------------------------------
// Single-pointer front-end handle over a destructor_wrapper_base. class_data ctor
// 0x18092C0BC seeds m_dtor (+0xB0) with the lazily-init static wrapper OBJECT at
// 0x1849302E0 (.data, magic-static guarded); that object's vptr is the NAMED vtable
// ??_7destructor_wrapper_base@detail@rttr@@6B@ at 0x183D2F9C0 (.rdata). Method
// surface pending.

namespace rttr {
namespace detail {
class destructor_wrapper_base;
}  // namespace detail

class destructor {
public:
    const detail::destructor_wrapper_base* m_wrapper;   // +0x00
};
static_assert(sizeof(destructor) == 8, "destructor is one wrapper ptr");

}  // namespace rttr
