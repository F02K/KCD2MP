#pragma once
#include "C_SoulEffect.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_PerkUsedEffect -- report a perk being used (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CE4B80  vtable 0x183AF4C70.  sizeof 0x28 (PROVEN: factory sub_181055FBC
// operator new(40)).  Ctor (owner, targetWuid, &perkGuid).  Visitor slots [1]=13 / [2]=14
// (the previously-unmapped pair in the effect-visitor tables).

namespace wh::rpgmodule {

class C_PerkUsedEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerkUsedEffect;
    CryGUID m_perkId;   // +0x18  [guid kind INFERRED from width + perk-db analogy]
};
static_assert(sizeof(C_PerkUsedEffect) == 0x28, "C_PerkUsedEffect must be 0x28 (operator new(40))");

}  // namespace wh::rpgmodule
