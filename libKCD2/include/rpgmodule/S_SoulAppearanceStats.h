#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SoulAppearanceStats -- the coverage-weighted appearance
// summary computed by sub_180468D14 (KCD2 WHGame.dll 1.5.6, kd7u).
// [NON-RTTI NAME]: plain aggregate, no vtable/RTTI -- descriptive name.
// sizeof 0x50 (max observed field +0x4F) [UNPROVEN exact tail].
// -----------------------------------------------
// Embedded INSIDE C_Soul at TWO offsets: primary @ C_Soul+0x58 (query getter
// sub_180647AC0 -> flags +0x48..+0x4B) and secondary @ C_Soul+0xB50 (read by
// perception BuildPerceptionInformation sub_181FD3DF0).  Recompute drivers
// sub_180645F28 / sub_180645F90 (event-driven on appearance/equipment change;
// +0xB50 recursion @sub_180645F90) -- calls sub_180BE1D10 (armor) +
// sub_180C493C8 (noise) + sub_180468D14 (visibility/conspicuousness/disguise).
// This is the store behind DerivStat_Visibility (3) / DerivStat_Conspicuousness
// (5) and the apparent-faction/social-class disguise decision.

namespace wh::rpgmodule {

struct S_FactionDefinition;   // perceived-faction ref target -- not RE'd here

struct S_SoulAppearanceStats {
    float    m_charismaBlend;         // +0x00  sum(w*cha)/W_total (@0x180469265)
    float    m_visibility;            // +0x04  coverage-weighted, clamp +-1 (@0x18046927b) = DerivStat 3
    float    m_conspicuousness;       // +0x08  coverage-weighted, clamp +-1 (@0x180469281) = DerivStat 5
    float    m_factionMetric0C;       // +0x0C  signed faction metric, clamp [-1,1]
    float    m_factionMetric10;       // +0x10  metric, clamp [0,1] (@0x18046926f/...275)
    uint32_t _pad14;                  // +0x14
    int32_t  m_flags18[4];            // +0x18  flags block (+0x18 = soul+2928 copy; +0x20/+0x24 from sub_180646AF0) [U roles]
    uint64_t _unk28;                  // +0x28  [U]
    float    m_appearanceScalar30;    // +0x30  charisma/appearance scalar (sub_180645F28 @0x180645f76)
    uint32_t _pad34;                  // +0x34
    S_FactionDefinition* m_perceivedSuperfaction;  // +0x38  dominant equipped superfaction or null (@0x180469287)
    int32_t  m_perceivedSocialClass;  // +0x40  dominant equipped social-class id, -1 default (@0x180469298)
    uint32_t _pad44;                  // +0x44
    bool     m_query48;               // +0x48  query result case 1 (sub_180647AC0)
    bool     m_query49;               // +0x49  query result case 2
    bool     m_query4A;               // +0x4A  = (v9 & 2) == 0 (sub_181185B70)
    bool     m_query4B;               // +0x4B  = (v9 & 4) == 0
    bool     m_anyItemFlag;           // +0x4C  any special-property item worn (@0x180469295)
    uint8_t  _pad4D;                  // +0x4D
    bool     m_computedGuard4E;       // +0x4E  recompute guard (sub_180645F90 @0x180645fc5)
    bool     m_computedGuard4F;       // +0x4F  recompute guard (sub_180645F28 @0x180645f5d)
};
static_assert(sizeof(S_SoulAppearanceStats) == 0x50, "S_SoulAppearanceStats must be 0x50");

}  // namespace wh::rpgmodule
