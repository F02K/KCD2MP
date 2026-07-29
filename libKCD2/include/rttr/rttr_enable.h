#pragma once

// -----------------------------------------------
// RTTR_ENABLE() -- KCD2 WHGame.dll 1.5.6 (kd7u) statically links rttr (rttr.org),
// master/0.9.7-dev lineage (pointer-based rttr::type, NOT the 0.9.6 id-based one),
// with Warhorse modifications (see rttr/detail/type_data.h).
// -----------------------------------------------
// The macro injects the reflection trio IN PLACE among a class's virtuals:
//   virtual rttr::type get_type() const;             // per-class: TLS magic-static -> type_data creator -> registration
//   virtual void* get_ptr();                         // `return this` -- COMDAT/ICF-folds to the ONE 0x1805F5DA0
//                                                    //   shared by all 4,353 reflected vtables
//   virtual rttr::detail::derived_info get_derived_info();  // per-class: returns {this, get_type()}
//
// Declaration position == vtable position. That is why the trio sits at slots [0..2] in
// C_UIMenuButton (macro before everything), [1..3] in C_PlayerData (after the dtor) and
// [7..9] in C_UIBase (after all its own virtuals). A common C++ base class CANNOT
// reproduce those layouts -- the macro-in-place mechanism is the only faithful model
// (binary-proven; do not "unify" this into a real base).
//
// RTTR_ENABLE(Base, ...) in a derived class OVERRIDES the trio (no new vtable slots) and
// records the bases in base_class_list, which registration walks to build
// class_data::m_base_types. Census: of the 4,353 reflected vtables, 1,382 declare their
// own trio and 2,971 inherit it unchanged (analysis/rttr/reflected_classes.json).
//
// DEVIATIONS from upstream rttr_enable.h, both deliberate:
//  * declaration-only -- upstream defines the three bodies inline; we never define them
//    (the binary implementations live at the EAs commented at each use site).
//  * ends with `public:` instead of `private:` -- RE headers keep every member public;
//    a single access-control group also guarantees declaration-order layout.

namespace rttr {
class type;
namespace detail {
struct derived_info;
template <typename... T> struct type_list {};
}  // namespace detail
}  // namespace rttr

#define RTTR_ENABLE(...)                                          \
public:                                                           \
    virtual rttr::type get_type() const;                          \
    virtual void* get_ptr();                                      \
    virtual rttr::detail::derived_info get_derived_info();        \
    using base_class_list = rttr::detail::type_list<__VA_ARGS__>; \
public:
