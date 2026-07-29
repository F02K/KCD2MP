#pragma once
#include "C_ItemEffect.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_AddItemOnEquipBuffEffect -- attach an on-equip buff to an item
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D60170  vtable 0x183F27938.  sizeof 0x28 (PROVEN: factory sub_182CD3F34
// operator new(40)).  Ctor (owner, targetItemWuid, &buffGuid).

namespace wh::rpgmodule {

class C_AddItemOnEquipBuffEffect : public C_ItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AddItemOnEquipBuffEffect;
    CryGUID m_buffId;   // +0x18  on-equip buff guid [kind INFERRED from width + buff-table analogy]
};
static_assert(sizeof(C_AddItemOnEquipBuffEffect) == 0x28,
              "C_AddItemOnEquipBuffEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
