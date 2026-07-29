#pragma once
#include "C_TwoStateSoulBuffInstance.h"
#include "C_ManlyOdourBase.h"

// -----------------------------------------------
// wh::rpgmodule::C_ManlyOdourStealthBuff : C_TwoStateSoulBuffInstance +
// C_ManlyOdourBase -- the "Manly Odour" perk stealth buff (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0xE0 [INFERRED: primary base 0xD8 + empty secondary
// base @0xD9 rounded to 0xE0; no direct alloc witness -- instantiated via the
// RTTR buff factory sub_18063AE40 "wh.rpgmodule.buff.<Type>"; 0xD8 possible if
// EBO applies -- no static_assert].
// -----------------------------------------------
// RTTI TD rva 0x4CE8498; COL 0x184392B00; vtable 0x3F02100 (32 slots; overrides
// C_TwoStateSoulBuffInstance base 0x183EFAB80 at {10,15,16,17,31}):
//  [10] sub_181664730 deleting-dtor variant
//  [15] sub_181A90140 GetTag -> &unk_18532AC30 (runtime guard tag)
//  [16] sub_182BD7EB8 two-state Activate, TAG-GUARDED: if sub_18046B774(a2,
//       &unk_18532AC30) skip; else base sub_182BC7FB8
//  [17] sub_182BD7E78 two-state Deactivate, tag-guarded (base sub_182BC7F78)
//  [31] sub_182BD6FC0 leaf hook -> sub_182BD6CD4(m_statMods+0x88) [U role]
// Adds NO members (ctor sub_182BD59B0 = base ctor sub_18046C4DC + vtable).
// Pure behavioral leaf: the effect is data-driven, gated by
// PerkManlyOdourDirtinessThreshold (0xDF0) -- suppresses scent-based detection
// while dirty [effect target U].

namespace wh::rpgmodule {

class C_ManlyOdourStealthBuff : public C_TwoStateSoulBuffInstance,
                                public C_ManlyOdourBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ManlyOdourStealthBuff;
};
// sizeof 0xE0 inferred (empty secondary base placement) -- no assert [UNPROVEN]

}  // namespace wh::rpgmodule
