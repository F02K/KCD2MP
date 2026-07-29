#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindCarryItemPile -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (CONFIRMED: alloc 96).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindCarryItemPile@entitymodule@wh@@ (TD 0x184C75B90, COL 0x18413DF60; CHD = [self,
// CScriptableBase])  vftable 0x183AB7F40 (4 slots; [0] dtor sub_1825CDA88 -- plain CScriptableBase
// teardown, same COMDAT-folded fn as C_ScriptBindCarryableItem)  ctor sub_180E30184, called from the
// entitymodule bind factory sub_180E30204 @0x180E30802 (collection slot [16]).
//
// GLOBAL Lua table "CIPileBind" (SetGlobalName sub_180B85140; m_bInstanceMethods=0): dot-call verbs
// for carry-item piles (stacked carryables, e.g. quest hay/wood piles), keyed by acting entity +
// pile entity. No members beyond CScriptableBase.
//
// Lua fn table (5 fns; registrar sub_180EF7B70, called from ctor):
//   CanDeposit      0x180D7C7C0  (userId, pileId)
//   Deposit         0x180D7BC5C  (userId, pileId)
//   CanPickUp       0x180BE13B8  (userId, pileId)
//   PickUp          0x18139141C  (userId, pileId)
//   CanPackAndPick  0x180BE1408  (userId, pileId)

namespace wh::entitymodule {

class C_ScriptBindCarryItemPile : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindCarryItemPile;
};
static_assert(sizeof(C_ScriptBindCarryItemPile) == 0x60, "C_ScriptBindCarryItemPile must be 0x60");

}  // namespace wh::entitymodule
