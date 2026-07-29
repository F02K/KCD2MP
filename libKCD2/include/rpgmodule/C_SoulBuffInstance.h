#pragma once
#include <cstdint>
#include "C_BuffInstance.h"
#include "S_ModifierNode.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulBuffInstance : C_BuffInstance<C_Soul> -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_SoulBuffInstance@rpgmodule@wh@@ (TD 0x184CE4DD0)  vtable 0x183A38D08
// ctor sub_18046C4DC.  Base class for ALL concrete C++ buff implementations.
// SIZEOF NOTE: 0xD8 is inferred from the ctor write extent + the C_TimedSoulBuffInstance layer
// starting at 0xD8 -- no standalone allocation witness (flagged by the verify pass).
//
// [MODERATE vs KCD1] KCD1 had 7 staging lists (0x90..0xC8, sizeof 0xD0); KCD2 adds an 8th list
// @+0xC8 which CommitModifiers (sub_18063B7C8) appends UNSORTED (no eOp compare) into the soul,
// setting the dirty flag @C_Soul+0xE8 -- m_materialEffectHandle shifted +0xC8 -> +0xD0.
// The specific category->offset mapping of the first 7 lists is a KCD1 PRIOR (verify pass
// downgraded it to MEDIUM: KCD2 proves 8 heads + sorted-vs-unsorted commit, not which category
// occupies which slot). Terminate = sub_18063AA3C (deactivates every node in all 8 lists).
// [22..25] virtuals are KCD1 priors (ApplyMaterialEffect/RemoveMaterialEffect/HasLuaScript/
// OnModifierRegistered) -- KCD2 order UNVERIFIED.

namespace wh::rpgmodule {

class C_SoulBuffInstance : public C_BuffInstance<C_Soul> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulBuffInstance;
    virtual int16_t ApplyMaterialEffect(C_Soul* a_pSoul);   // [22]  [KCD1 prior]
    virtual void RemoveMaterialEffect(C_Soul* a_pSoul);     // [23]  [KCD1 prior]
    virtual bool HasLuaScript() { return false; }           // [24]  [KCD1 prior]
    virtual void OnModifierRegistered() {}                  // [25]  [KCD1 prior]

    // 8 modifier STAGING list heads (KCD2-verified count/behavior; category labels = KCD1 priors).
    S_ModifierNode* m_statMods;        // +0x90  cat0 base stats            [KCD1 prior label]
    S_ModifierNode* m_skillMods;       // +0x98  cat1 skills                [KCD1 prior label]
    S_ModifierNode* m_perkStatMods;    // +0xA0  cat2 perk-modifiable stats [KCD1 prior label]
    S_ModifierNode* m_derivedStatMods; // +0xA8  cat3 derived stats         [KCD1 prior label]
    S_ModifierNode* m_movementMods;    // +0xB0  cat4 movement              [KCD1 prior label]
    S_ModifierNode* m_specialMods;     // +0xB8  cat5 special/manual        [KCD1 prior label]
    S_ModifierNode* m_soulLevelMods;   // +0xC0  cat6 soul-level            [KCD1 prior label]
    S_ModifierNode* m_extraMods;       // +0xC8  cat7 NEW in KCD2: committed UNSORTED; drives the
                                       //        C_Soul+0xE8 dirty flag (category identity unresolved)
    uint16_t m_materialEffectHandle;   // +0xD0  flowgraph effect handle [KCD1 prior name; writer not re-decompiled]
    uint8_t  _padD2[6];                // +0xD2  genuine alignment pad before the 8-aligned double @0xD8
};
static_assert(sizeof(C_SoulBuffInstance) == 0xD8, "C_SoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
