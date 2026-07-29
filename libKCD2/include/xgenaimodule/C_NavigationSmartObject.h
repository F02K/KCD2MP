#pragma once
#include <cstdint>
#include "C_SmartObject.h"
#include "C_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_NavigationSmartObject : C_SmartObject -- navigation-linked
// smart object (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof >= 0x1D8 (INFERRED from
// ctor writes; create path not traced to an alloc constant) -- NO
// static_assert.  ctor sub_181296D4C, dtor sub_181296CB4.  Registers into the
// navigation-SO manager singleton qword_185496240.
// -----------------------------------------------
// RTTI TD rva 0x4F5A6D8 (11 bases).  Primary vtable rva 0x3B4A5A0 (41 slots;
// overrides vs C_SmartObject: [0]dtor 0x1296C70, [11]GetOwnership 0x1AA5E30,
// [40]0x8FE2E0).  Secondary vtables +0x20 0x3B4A528 / +0xA8 0x3B4A548 /
// +0xB0 0x3B4A568.  ctor writes *(navLink+8) = this (back-link).

namespace wh::xgenaimodule {

namespace navigation { class I_SmartObjectNavigationComponent; }   // RTTI .?AVI_SmartObjectNavigationComponent@navigation@xgenaimodule@wh@@; concrete: ...ComponentDoor/Ledge/Ladder

class I_Ownership;

class C_NavigationSmartObject : public C_SmartObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NavigationSmartObject;
    ~C_NavigationSmartObject() override;    // [0]  deleting dtor 0x1296C70
    I_Ownership* GetOwnership() override;   // [11] 0x1AA5E30
    void* _vf40() override;                 // [40] 0x8FE2E0

    C_Ownership m_ownership;   // +0x1A0  embedded (0x30)
    navigation::I_SmartObjectNavigationComponent* m_navLink;  // +0x1D0  owned nav component (base iface per RTTI; concrete Door/Ledge/Ladder, sub_1804CF5EC); ctor stores it and back-links *(m_navLink+8)=this
};
// sizeof >= 0x1D8 INFERRED (ctor writes only) -- no static_assert.

}  // namespace wh::xgenaimodule
