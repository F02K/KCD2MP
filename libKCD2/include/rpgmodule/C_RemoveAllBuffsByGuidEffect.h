#pragma once
#include "C_SoulEffect.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_RemoveAllBuffsByGuidEffect -- strip buffs by guid (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FE30  vtable 0x183A8AC60.  sizeof 0x28 (PROVEN: factory sub_180C61810
// operator new(40)).  Ctor (owner, targetWuid, &buffGuid).

namespace wh::rpgmodule {

class C_RemoveAllBuffsByGuidEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RemoveAllBuffsByGuidEffect;
    CryGUID m_buffId;   // +0x18  buff guid to remove [kind INFERRED]
};
static_assert(sizeof(C_RemoveAllBuffsByGuidEffect) == 0x28,
              "C_RemoveAllBuffsByGuidEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
