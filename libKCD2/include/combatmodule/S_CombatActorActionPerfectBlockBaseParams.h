#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionPerfectBlockBaseParams -- abstract primary base of the
// perfect-block-family params (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionPerfectBlockBaseParams@combatmodule@wh@@  (struct; TD 0x184AF9680).
// It has a TypeDescriptor but NO COL / NO standalone vtable -- it is never the most-derived object;
// its vtable materialises only inside the concrete leaf params (S_CombatActorActionPerfectBlockParams,
// S_CombatActorActionSyncPerfectBlockParams), both of which list it FIRST in their RTTI BaseClassArray
// at mdisp +0x00.  This is a NEW KCD2 grouping base (the KCD1 params carried an embedded
// I_CombatActorActionParamsReflect instead).
//
// Layout proved from the leaf params ctor sub_18164F43C: it writes the leaf vtable to params+0x00 and
// a single byte 0 to params+0x08 -- so this subobject spans [+0x00,+0x10): one vtable pointer + one
// byte of data.  Its effective vtable (as seen in S_CombatActorActionPerfectBlockParams+0x00,
// 0x183BE07E0) holds exactly ONE slot [0] = 0x181A73BA0 (called by the ctor).

namespace wh::combatmodule {

struct S_CombatActorActionPerfectBlockBaseParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionPerfectBlockBaseParams;
    virtual void _vf0() {}     // [0] 0x181A73BA0  (only virtual; role UNVERIFIED -- reflect/accessor)

    uint8_t m_field08;       // +0x08  init 0  (UNVERIFIED semantic)
    uint8_t _pad09[7];       // +0x09
};
static_assert(sizeof(S_CombatActorActionPerfectBlockBaseParams) == 0x10,
              "S_CombatActorActionPerfectBlockBaseParams: vtable + 1 byte, padded to 0x10");

}  // namespace wh::combatmodule
