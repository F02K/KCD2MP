#pragma once
#include <cstdint>
#include "../framework/C_OverrideConflictManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartBehaviorStateManager :
// framework::C_OverrideConflictManager<S_BehaviorHandle,
// E_SmartBehaviorState::Type> -- smart-behavior state override manager (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof ~0x18 (>= 0x18 from ctor writes; no direct
// create-site constant captured [U]) -- NO static_assert.  ctor sub_18119C050;
// registers callbacks sub_18118A8B0 / sub_18041C37C.
// -----------------------------------------------
// RTTI TD rva 0x4F51CE0 (2 bases; base mangles as
// C_OverrideConflictManager<US_BehaviorHandle, W4Type@E_SmartBehaviorState...>).
// Vtable rva 0x3FE9890 (3 slots: [0]dtor 0x32C26D0 [1]0x870F68 [2]0x410304).
// +0x08 = head of 104-byte intrusive node (3 self-ptrs + WORD 257), +0x10 = 0
// -- exactly the template-base layout.  S_BehaviorHandle /
// E_SmartBehaviorState enumerators NOT recovered [U] -- declared minimally
// here.

namespace wh::xgenaimodule {

struct S_BehaviorHandle;   // layout [U] -- template parameter only

struct E_SmartBehaviorState {
    enum Type : int32_t {};   // values [U]
};

class C_SmartBehaviorStateManager
    : public framework::C_OverrideConflictManager<S_BehaviorHandle, E_SmartBehaviorState::Type> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartBehaviorStateManager;
    ~C_SmartBehaviorStateManager() override;   // [0] 0x32C26D0
    void OcmVf1() override;                    // [1] 0x870F68 [U role]
    void OcmVf2() override;                    // [2] 0x410304 [U role]
};
// sizeof ~0x18 (base only; no create-site constant) -- no static_assert.

}  // namespace wh::xgenaimodule
