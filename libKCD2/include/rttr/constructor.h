#pragma once

// -----------------------------------------------
// rttr::constructor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 8.
// -----------------------------------------------
// Single-pointer front-end handle over a constructor_wrapper_base (upstream shape;
// class_data +0x80 vector). The binary instantiates
// constructor_wrapper<T, class_ctor, as_std_shared_ptr, ...> per UI screen class --
// that is how C_GUIModule::Init creates every registered C_UIBase subclass by
// enumeration. Method surface pending agent dossiers.

namespace rttr {
namespace detail {
class constructor_wrapper_base;
}  // namespace detail

class constructor {
public:
    const detail::constructor_wrapper_base* m_wrapper;   // +0x00
};
static_assert(sizeof(constructor) == 8, "constructor is one wrapper ptr");

}  // namespace rttr
