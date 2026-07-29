#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_ConceptMarkerType -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Warhorse enum-wrapper idiom (mangles as W4Type@E_ConceptMarkerType@guimodule@wh@@).
// rttr enumeration "wh::guimodule::ConceptMarkerType" (+ "...ConceptMarkerTypeWrapper"
// for the struct), registered by sub_181AC4380 -- ALL 20 name/value pairs below are
// read verbatim from that registration (enumeration_wrapper<...,20,0> TD 0x184CAD7F0).
//
// Marker category selected by the concept-graph C_ShowMapMarker node ("MarkerType"
// port); converted to the map POI-filter id domain by sub_180C4DC78 before a
// S_EntityMapMark is created.  KCD2-NEW (no KCD1 counterpart).
// "FirstFightArena" is the game's own typo (value 5).

namespace wh::guimodule {

struct E_ConceptMarkerType {
    enum Type : int32_t {
        QuestGiver             = 0,
        ActivityGiver          = 1,
        QuestTipster           = 2,
        PoiTipster             = 3,
        FightArena             = 4,
        FirstFightArena        = 5,   // (sic)
        DLC0                   = 6,
        Barber                 = 7,
        ShieldPainter          = 8,
        DLC1_BrushesWithDeath  = 9,
        DLC2_LegacyOfTheForge  = 10,
        DLC3_MysteriaEcclesiae = 11,
        Dog                    = 12,
        DLC2_smithing          = 13,
        DLC2_dice              = 14,
        DLC2_acquiringPackages = 15,
        DLC2_archery           = 16,
        DLC2_donations         = 17,
        DLC2_duels             = 18,
        DLC2_stealingPackages  = 19,
    };
};

}  // namespace wh::guimodule
