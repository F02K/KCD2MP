#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindCarryableItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (CONFIRMED: alloc 96).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindCarryableItem@entitymodule@wh@@ (TD 0x184C75B50, COL 0x18413DF88; CHD = [self,
// CScriptableBase])  vftable 0x183ACF210 (4 slots; [0] dtor sub_1825CDA88 -- plain CScriptableBase
// teardown, same COMDAT-folded fn as C_ScriptBindCarryItemPile)  ctor sub_180EF7A08, called from the
// entitymodule bind factory sub_180E30204 @0x180E30852 (collection slot [17]).
//
// GLOBAL Lua table "CarryableItemBind" (SetGlobalName sub_180B85140; m_bInstanceMethods=0): dot-call
// verbs for carryable world-item entities, keyed by acting entity + target item entity. No members
// beyond CScriptableBase.
//
// Lua fn table (2 fns; registered inline in the ctor):
//   CanPickUp  0x18174C170  (userId, itemEntityId)
//   PickUp     0x182AC04E0  (userId, itemEntityId)

namespace wh::entitymodule {

class C_ScriptBindCarryableItem : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindCarryableItem;
};
static_assert(sizeof(C_ScriptBindCarryableItem) == 0x60, "C_ScriptBindCarryableItem must be 0x60");

}  // namespace wh::entitymodule
