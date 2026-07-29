#pragma once
#include "C_ConstantSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_ConstantContextCheckingSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// ABSTRACT structural layer: the binary emits NO own COL / ??_7 vtable for this class -- it only
// appears as a base sub-object (mdisp 0) in the RTTI base chains of context-checking leaves
// (e.g. C_CombatContextSoulBuffInstance). Adds NO data (context/tag is read from the buff
// definition); sizeof 0xD8 is therefore inherited/structural, not read from an alloc site
// (flagged). New in KCD2 -- no KCD1 analog.

namespace wh::rpgmodule {

class C_ConstantContextCheckingSoulBuffInstance : public C_ConstantSoulBuffInstance   // +0x00  (0xD8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ConstantContextCheckingSoulBuffInstance;
};
static_assert(sizeof(C_ConstantContextCheckingSoulBuffInstance) == 0xD8,
              "C_ConstantContextCheckingSoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
