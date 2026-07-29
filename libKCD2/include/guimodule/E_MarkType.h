#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_MarkType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Underlying int32.
// -----------------------------------------------
// The map/compass POI MARKER-CATEGORY (icon) domain.  This is the value called
// "mark_type" / "m_MarkType" everywhere it appears:
//   * poi_type table data column  <poi_type mark_type="14" .../>  (Tables.pak
//     Libs/Tables/rpg/poi_type.xml, 138 rows -- every row's mark_type is one of these).
//   * rpg POI-record serialized field "m_MarkType" (serializer sub_1803F1704 @+0x4C).
//   * S_EntityMapMark::m_type (+0x04) and C_CompassMark::m_type (+0x4C).
// The full-map POI icon shows its name on hover from this value: C_UIFlashMapPoiMarker's
// ctor (sub_1815B9D70) sets m_name = sub_180C4D664(m_type); Flash PoiMarker.as feeds that
// to OnShowTooltip.
//
// PROVENANCE (values TRIPLE-confirmed):
//   1. Enumerator NAMES+VALUES read VERBATIM from the value->string switch sub_180C4D664
//      (0x180C4D664) -- the exact fn the map/compass use to name a mark.
//   2. Cross-checked against poi_type.xml's mark_type column (138 rows) -- every value
//      matches, e.g. armourer=14, huntingSpotRoeDeer=57, gunsmith=76, divozenka=48(GeneralPoi).
//   3. Field token "mark_type"/"m_MarkType" is authoritative -- the identifier here is the
//      REAL field name, not a guess (the enum's C++ *wrapper* name is stripped, so the
//      struct+Type idiom mirrors sibling E_ConceptMarkerType).
//
// RELATED / DO NOT CONFUSE:
//   * E_ConceptMarkerType::Type (0..19, the C_ShowMapMarker "MarkerType" concept port,
//     defined in Libs/concept/definitions.xml) REMAPS into this domain via sub_180C4DC78.
//   * S_POIType (= poi_type_id GUID, poi_type.xml) is the SEPARATE data-driven POI identity
//     keyed into C_ObjectTableDatabase<S_POIType, S_POITypeDBData>; discovery state per POI
//     is wh::rpgmodule::E_LocationMarkStates {Unknown, KnownUndiscovered, Discovered}.
//     A poi_type row ties the two together: it has BOTH a mark_type (icon category, this
//     enum) AND a poi_type_id (its GUID identity).
//
// Any value not listed (gaps 33,35,40,41,54,60..68, or <0 / >96) falls to the switch
// default "?" (unk_183B797F0) -- the unknown/undiscovered glyph.
// Names kept EXACTLY as the binary spells them (incl. typos "MineEnrtrance",
// "Weaponsmiths") -- they double as Flash symbol / localization keys.

namespace wh::guimodule {

struct E_MarkType {
    enum Type : int32_t {
        Checkpoint             = 0,
        Main                   = 1,
        Side                   = 2,
        Micro                  = 3,
        QuestGiver             = 4,
        ActivityGiver          = 5,
        Hub                    = 6,
        FastTravel             = 7,
        FastTravelLevel        = 8,
        Dog                    = 9,
        Dlcs                   = 10,
        Home                   = 11,
        Shop                   = 12,
        Blacksmith             = 13,
        Armourer               = 14,
        Weaponsmiths           = 15,   // (sic)
        Shoemaker              = 16,
        Tailor                 = 17,
        Pub                    = 18,
        Apothecary             = 19,
        Herbalist              = 20,
        Miller                 = 21,
        HorseTrader            = 22,
        Saddler                = 23,
        Bailiff                = 24,
        Arena                  = 25,
        ArcheryArena           = 26,
        FistFight              = 27,
        Alchemy                = 28,
        SharpeningWheel        = 29,
        Baths                  = 30,
        HuntingSpot            = 31,
        HuntingSpotBoar        = 32,
        // 33 unused -> "?"
        FishingSpot            = 34,
        // 35 unused -> "?"
        Hive                   = 36,
        ForestGarden           = 37,
        Camp                   = 38,
        CampEnemy              = 39,
        // 40,41 unused -> "?"
        MineEnrtrance          = 42,   // (sic)
        Grave                  = 43,
        Nest                   = 44,
        ConcCross              = 45,
        Shrine                 = 46,
        Bed                    = 47,
        GeneralPoi             = 48,
        Bakery                 = 49,
        Butchery               = 50,
        VegetableShop          = 51,
        Tanner                 = 52,
        Hunter                 = 53,
        // 54 unused -> "?"
        Scribe                 = 55,
        Hotel                  = 56,
        HuntingSpotRoe         = 57,
        HuntingSpotWolf        = 58,
        Smithy                 = 59,
        // 60..68 unused -> "?"
        SkillTeacher           = 69,
        FightArena             = 70,
        PoiTipster             = 71,
        SmokingFood            = 72,
        DryingFood             = 73,
        Washing                = 74,
        Indulgences            = 75,
        Gunsmith               = 76,
        DiceTable              = 77,
        FistFightArena         = 78,
        DLC0                   = 79,
        BedPlayer              = 80,
        Barber                 = 81,
        ShieldPainter          = 82,
        DLC1                   = 83,
        DLC2                   = 84,
        DLC3                   = 85,
        Racing                 = 86,
        DLC2_smithing          = 87,
        DLC2_dice              = 88,
        DLC2_acquiringPackages = 89,
        DLC2_archery           = 90,
        DLC2_donations         = 91,
        DLC2_duels             = 92,
        DLC2_stealingPackages  = 93,
        DLC2_activities        = 94,
        SellingChest           = 95,
        FastTravelSedlec       = 96,
    };
};

// The value->string switch sub_180C4D664 (the exact fn the map/compass use to name a
// mark; every enumerator above was read from it). Unlisted values return the
// unknown-glyph string "?". Impl in src/guimodule/E_MarkType.cpp.
const char* GetMarkTypeName(E_MarkType::Type type);

}  // namespace wh::guimodule
