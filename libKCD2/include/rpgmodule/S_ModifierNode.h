#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ModifierNode -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (pool-allocated).
// -----------------------------------------------
// One stat-modifier node. Buff instances own singly-linked STAGING lists of these (8 category
// heads on C_SoulBuffInstance @+0x90..+0xC8); CommitModifiers (sub_18063B7C8) moves the first 7
// categories into C_Soul's SORTED lists @C_Soul+0xA8 (insert keyed ascending on eOp @+0x2C) and
// appends the 8th category UNSORTED (setting the soul dirty flag @C_Soul+0xE8).
//
// KCD2-VERIFIED fields: pNextStaging @+0x18, pNextSorted @+0x20, nTargetId @+0x28, eOp @+0x2C
// (sort key), node stride 0x40, bActive cleared by Terminate (sub_18063AA3C). All REMAINING
// fields are KCD1 PRIORS carried over byte-for-byte (KCD1 S_ModifierNode had the identical 0x40
// layout) -- KCD2-UNVERIFIED and flagged as such. KCD1 additionally templated the node on its
// E_ModifierCategory / per-category target-id enum; the KCD2 enum values are unverified, so
// nTargetId/eOp are kept as raw int32 here.

namespace wh::rpgmodule {

class C_BuffInstanceBase;
class C_Soul;

struct S_ModifierNode {
    C_BuffInstanceBase* pOwner;        // +0x00  owning buff instance            [KCD1 prior]
    C_Soul*             pSoul;         // +0x08  set during CommitModifiers      [KCD1 prior]
    uint8_t             bActive;       // +0x10  1 = active (Terminate clears)   KCD2-verified use
    uint8_t             bTimeBased;    // +0x11                                  [KCD1 prior]
    uint8_t             bNonWearing;   // +0x12                                  [KCD1 prior]
    uint8_t             bExtraFlag;    // +0x13                                  [KCD1 prior]
    uint8_t             _pad14[4];     // +0x14
    S_ModifierNode*     pNextStaging;  // +0x18  next in staging list            VERIFIED KCD2
    S_ModifierNode*     pNextSorted;   // +0x20  next in soul's sorted list      VERIFIED KCD2
    int32_t             nTargetId;     // +0x28  id in the category's namespace  VERIFIED KCD2 (width)
    int32_t             eOp;           // +0x2C  operation / ascending sort key  VERIFIED KCD2 (KCD1 E_ModifierOp)
    float               fValue;        // +0x30  current modifier value          [KCD1 prior]
    float               fOrigValue;    // +0x34  original/base value             [KCD1 prior]
    int32_t             nExtra;        // +0x38                                  [KCD1 prior]
    uint8_t             bSpecialFlag;  // +0x3C                                  [KCD1 prior]
    uint8_t             _pad3D[3];     // +0x3D
};
static_assert(sizeof(S_ModifierNode) == 0x40, "S_ModifierNode must be 0x40");

}  // namespace wh::rpgmodule
