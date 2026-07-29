#pragma once
#include <cstdint>
#include "I_CombatActionBlockHelperParams.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionBlockParams -- params of C_CombatActorActionBlock
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionBlockParams@combatmodule@wh@@  (struct; TD 0x184AFA610, COL 0x184160E70,
// vtable 0x183A8A430 -- 18 concrete reflect slots).
// RTTI BaseClassArray: numBaseClasses=2 = { self, I_CombatActionBlockHelperParams @ +0x00 } -- single
// inheritance; the base supplies the +0x00 vtable, this struct overrides all 18 reflect slots.
//
// Layout proved from the base ctor sub_180C55884: writes &S_CombatActorActionBlockParams::vftable to
// params+0x00 and 0 to params+0x08.  Embedded by value at +0x50 of the Block action (C_CombatActorObject
// begins at +0x60, so the params block is exactly 0x10).  Same single-qword payload as KCD1.

namespace wh::combatmodule {

struct S_CombatActorActionBlockParams : public I_CombatActionBlockHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionBlockParams;
    uint64_t m_data;         // +0x08  init 0  (payload; KCD1-correlated m_data)
};
static_assert(sizeof(S_CombatActorActionBlockParams) == 0x10,
              "S_CombatActorActionBlockParams = helper-params vtable + 1 qword");

}  // namespace wh::combatmodule
