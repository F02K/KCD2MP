#pragma once
#include "C_ItemEffect.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_RemoveItemOnEquipBuffEffect -- detach an on-equip buff from an item
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E8D0  vtable 0x183F1EDC0.  sizeof 0x28 (PROVEN: factory sub_182CD447C
// operator new(40)).  Ctor (owner, targetItemWuid, &buffGuid) -- mirror of
// C_AddItemOnEquipBuffEffect.

namespace wh::rpgmodule {

class C_RemoveItemOnEquipBuffEffect : public C_ItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RemoveItemOnEquipBuffEffect;
    CryGUID m_buffId;   // +0x18  on-equip buff guid [kind INFERRED from width + buff-table analogy]
};
static_assert(sizeof(C_RemoveItemOnEquipBuffEffect) == 0x28,
              "C_RemoveItemOnEquipBuffEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
