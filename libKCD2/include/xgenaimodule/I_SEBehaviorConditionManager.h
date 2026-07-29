#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SEBehaviorConditionManager -- smart-entity behavior
// condition manager interface; primary MI base of C_SituationManager (@+0x0)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SEBehaviorConditionManager@xgenaimodule@wh@@.  No
// standalone vtable emitted (embedded-only); shape (2 slots) read from the
// C_SituationManager primary vtable rva 0x3FE8708: [0]sub_1832B98D8
// [1]sub_1832B760C.  [1] sits in the 0x1832B7xxx deleting-dtor thunk cluster
// -- modeled as the dtor slot; placement [U].  Slot semantics NOT decompiled.

namespace wh::xgenaimodule {

class I_SEBehaviorConditionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SEBehaviorConditionManager;
    virtual void BcmVf0() = 0;               // [0] [U role]
    virtual ~I_SEBehaviorConditionManager(); // [1] [U placement]
};
static_assert(sizeof(I_SEBehaviorConditionManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
