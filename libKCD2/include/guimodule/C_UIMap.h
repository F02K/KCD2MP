#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UITreeList.h"
#include "guimodule/C_UIMapContent.h"
#include "guimodule/C_UIModalDialogQuestion.h"
#include "guimodule/C_UIModalDialogRandomEvent.h"
#include "guimodule/C_UIMapCloudAtlas.h"
#include "guimodule/C_ScriptBindMap.h"
#include "guimodule/C_CompassMark.h"
#include "guimodule/C_CheckpointMark.h"
#include "guimodule/S_EntityMapMark.h"
#include "playermodule/I_UIFastTravel.h"
#include "game/I_GameSideEffectCallback.h"
#include "framework/CryDeferrable.h"
#include "Offsets/vtables/IActionListener.h"

struct ICVar;

// -----------------------------------------------
// wh::guimodule::C_UIMap -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x750.
// -----------------------------------------------
// RTTI .?AVC_UIMap@guimodule@wh@@ (TD 0x184C929D0).
// Bases (CHD): C_UIBase @+0x00 (COL 0x4349C30, vtable 0x183ED8E78, 10 slots),
// IActionListener @+0x10 (COL 0x4349C58, vtable 0x183ED8EE8, 3),
// playermodule::I_UIFastTravel @+0x18 (COL 0x4349C80, vtable 0x183ED8F08, 5),
// game::I_GameSideEffectCallback @+0x20 (COL 0x4349CA8, vtable 0x183ED8ED0, 2),
// UnsafeOp::CryDeferrable<0> @+0x28 (empty; overlaps m_legendList).
// ctor sub_18146C3C4(this, C_UIApse* owner); dtor sub_182AFCDF8.
//
// NOT rttr-instantiated and NOT flash-bound: embedded BY VALUE in the rttr screen
// C_UIApse at +0x1508 (C_UIApse ctor sub_18146C89C @0x18146c9be; alloc 10344 = 0x2868,
// creator sub_18146C7B8; next member C_UIApseCodex @+0x1C58 pins sizeof = 0x750).
// INSTANCE ACCESS: S_GameContext (sub_1809155C8) +0xE8 = C_GUIModule ->
// rttr element getter sub_1805677CC (cached C_UIApse* @qword_18548B050) + 0x1508
// (pattern verbatim in UIMap.GoToCheckpointMark 0x182BA7810 and C_ShowMapMarker
// trigger 0x180C4DADC).
//
// [FUNDAMENTAL vs KCD1] KCD1 C_UIMap (0x4C8) was a standalone CUIManager event system
// binding the "Map" UIElement (hash-dispatch OnUIEvent @+0x338, 4 SUIEventSenderBlock
// signals for C_FastTravel, 15 wh_map_* CVar pointers, PodArray<POI>).  KCD2 demotes it
// to a non-flash C_UIBase sub-component of the Apse hub: flash traffic moved into the
// embedded C_UIMapContent ("ApseMap" element), the C_FastTravel signal blocks became
// the typed I_UIFastTravel base (registered into C_FastTravel+0x38 by Init via
// sub_180ED10A8), POIs became S_EntityMapMark records + live C_CompassMarks, and only
// 3 wh_map_* CVars remain (StickDeadZone / MoveCursorSens / ShowFastTravelPoints).
// KCD1's ForceStartFastTravel (fake-travel UI event) has NO KCD2 equivalent here --
// the fake/visual travel path now lives in playermodule::C_FastTravel (fake flag).
//
// Own vtable slots (C_UIBase shape): [0] dtor 0x182AFD948 | [1] Init 0x180ED0EBC
// (CVars; CloudAtlas init sub_180ED1410; Init(module) cascade into the 4 embedded
// screens; I_UIFastTravel registration; OnApseViewChanged delegate sub_180562300 onto
// the owner signal apse+0x78; side-effect callback registration for ids 0xC4/0xC5/0xC6
// via dispatcher vf+0x98) | [2] Deinit 0x182AFE6D0 (exact mirror, vf+0xA0) |
// [3] OnModuleMessage 0x18085E4E4 (0x18 = level reset: clouds + checkpoint marks +
// content cursor; 0x38 = cursor reset; 0x41 = re-evaluate POI filters 4/5/6/69/71) |
// [4] Update 0x180567BC8 (Update cascade into the embedded screens + IUIElement
// vf+0x200 on the content element while the apse view is 6/8..10) | [7..9] rttr trio.
// IActionListener::OnAction 0x18168C72C dispatches through m_actionHandlers.
// I_GameSideEffectCallback [0] 0x181F4B680 / [1] 0x181F4B710: side effects 0xC6/0xC5/
// 0xC4 gate POI filter groups (4,5)/(6,71)/(69).
//
// Save/load (serializer sub_180DCA18C): chunk 0xC1D = content cursor Vec2, 0xC1E =
// m_poiFilterVisible by NAME (mapper sub_180DCA50C), 0xC20 = checkpoint marks
// (Vec3 + level id each).
//
// POI-filter id domain (index into m_poiFilterVisible, save-name mapper sub_180DCA50C;
// these are the data-driven poi_type ids, NOT a binary enum):
//   0 Checkpoint, 1 Main, 2 Side, 3 Micro, 4 QuestGiver, 5 ActivityGiver, 6 Hub,
//   7 FastTravel, 8 FastTravelLevel, 9 Dog, 10 DLCs, 11 Home, 12 Shop, 13 Blacksmith,
//   14 Armourer, 15 Weaponsmiths, 16 Shoemaker, 17 Tailor, 18 Pub, 19 Apothecary,
//   20 Herbalist, 21 Miller, 22 HorseTrader, 23 Saddler, 24 Bailiff, 25 Arena,
//   26 ArcheryArena, 27 FistFight, 28 Alchemy, 29 SharpeningWheel, 30 Baths,
//   31 HuntingSpot, 32 HuntingSpotBoar, 34 FishingSpot, 36 Hive, 37 ForestGarden,
//   38 Camp, 39 CampEnemy, 42 MineEnrtrance(sic), 43 Grave, 44 Nest, 45 ConcCross,
//   46 Shrine, 47 Bed, 48 GeneralPoi, 49 Bakery, 50 Butchery, 51 VegetableShop,
//   52 Tanner, 53 Hunter, 55 Scribe, 56 Hotel, 57 HuntingSpotRoe, 58 HuntingSpotWolf,
//   59 Smithy, 69 SkillTeacher, 70 FightArena, 71 PoiTipster, 72 SmokingFood,
//   73 DryingFood, 74 Washing, 75 Indulgences, 76 Gunsmith, 77 DiceTable,
//   78 FistFightArena, 79 DLC0, 80 BedPlayer, 81 Barber, 82 ShieldPainter,
//   83 DLC1_BrushesWithDeath, 84 DLC2_LegacyOfTheForge, 85 DLC3_MysteriaEcclesiae,
//   86 Racing, 87 DLC2_smithing, 88 DLC2_dice, 89 DLC2_acquiringPackages,
//   90 DLC2_archery, 91 DLC2_donations, 92 DLC2_duels, 93 DLC2_stealingPackages,
//   94 DLC2_activities, 95 SellingChest, 96 FastTravelSedlec.
//   (Invalid/Pillory/Church/Unknown save-names map to -1 = not persisted.)

namespace wh::guimodule {

class C_UIApse;   // Apse map/menu hub screen (0x2868, ctor sub_18146C89C); full model in C_UIApse.h

class C_UIMap
    : public C_UIBase                              // +0x00
    , public Offsets::IActionListener              // +0x10
    , public wh::playermodule::I_UIFastTravel      // +0x18
    , public wh::game::I_GameSideEffectCallback    // +0x20
    , public UnsafeOp::CryDeferrable<0>            // +0x28 (empty, overlaps m_legendList)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMap;
    // IActionListener OnAction lookup value: bound member function {address, this-adjust}.
    struct S_ActionHandler {
        void*   m_pfn;        // +0x00  handler body
        int64_t m_adjustor;   // +0x08  added to `this` before the call (0x18168C72C)
    };
    // Opaque 0x38 element of m_vec680 (per-element dtor sub_18178736C; fields not RE'd).
    struct S_Unk38 { uint8_t _unverified[0x38]; };

    // C_UIBase overrides (see banner for behavior).
    void Init(C_GUIModule* pModule) override;   // [1] 0x180ED0EBC
    void Deinit() override;                     // [2] 0x182AFE6D0
    void OnModuleMessage(void* msg) override;   // [3] 0x18085E4E4
    void Update() override;                     // [4] 0x180567BC8
    // IActionListener: dispatch through m_actionHandlers.
    void OnAction(const Offsets::SActionId& action, int activationMode, float value) override;   // 0x18168C72C
    // I_UIFastTravel[0] -- set the destination: guarded Apse->Map view request (slot[1],
    // return ignored) + nav path query (sub_182DE27D0) storing dest into C_FastTravel.m_destination and
    // filling m_pathNodeIds, then arms m_ftPathPending.  Does NOT move the player (that is
    // C_FastTravel::StartTravel).  Returns the travel-gate status (0 = armed OK).
    char SetDestination(const Vec3& dest, char a3, char forced) override;                        // 0x182B00C28
    void OnFastTravelStarted(bool a2) override;                                                  // 0x182AFEAE0
    void OnFastTravelFinished() override;                                                        // 0x182AFEA18
    void OnFastTravelUpdate(const Vec3* pos, const Vec3* dir, int remaining, ...) override;      // 0x182B02938
    void ShowRandomEventDialog(CryStringT<char>* text, void* a3, void* a4, void* a5) override;   // 0x182AFDE1C
    // I_GameSideEffectCallback (side effects 0xC4/0xC5/0xC6 gate POI filter groups).
    void OnSideEffectDeactivated(char sideEffectId) override;                                    // 0x181F4B680
    void OnSideEffectActivated(char sideEffectId) override;                                      // 0x181F4B710

    // --- Native methods (RVA forwarders in src/guimodule/C_UIMap.cpp) ---
    // The C_UIMap instance: S_GameContext::m_pGUIModule -> rttr apse getter
    // sub_1805677CC (cached C_UIApse* @qword_18548B050) + 0x1508.  Composition is
    // verbatim from UIMap.GoToCheckpointMark 0x182BA7810; null until the GUI module
    // has built its screens.
    [[nodiscard]] static C_UIMap* GetInstance();
    // Read a POI-filter visibility flag (the id is first mapped through the DLC-group
    // folder sub_180A1B1C4; ids > 0x60 -> false).
    bool IsPoiFilterVisible(uint32_t filterId) const;                             // 0x180C4DAB0
    // Show/hide every mark of a POI-filter group: materializes/destroys the live
    // C_CompassMarks for matching m_entityMarks and sends "AddPoiMarkers"/
    // "RemovePoiMarkers" to the "ApseMap" flash element while the map view is active.
    void SetPoiMarkersVisible(uint32_t filterId, bool visible);                   // 0x182B02B34
    // Find (by {type, obj, param}) or create+append a persistent entity-mark record.
    std::shared_ptr<S_EntityMapMark>& GetOrCreateEntityMark(
        std::shared_ptr<S_EntityMapMark>& out, uint32_t type,
        wh::xgenaimodule::C_LinkableObject* pObject, uint32_t param);             // 0x180C4DBD8
    // Register an entity-mark record: append to m_entityMarks (or bump m_useCount),
    // and if its filter is visible spawn the live C_CompassMark (m_activeMarks +
    // C_UICompass::m_marks via the cached compass getter sub_180C4E098).
    void RegisterEntityMark(std::shared_ptr<S_EntityMapMark>& mark);              // 0x180C4DD5C
    // Current level's checkpoint mark (null shared_ptr if none).
    std::shared_ptr<C_CheckpointMark>& FindCurrentLevelCheckpointMark(
        std::shared_ptr<C_CheckpointMark>& out);                                  // 0x180DCB460
    // Its map position (x,y; zeroed when there is no mark).
    void GetCheckpointMarkPos(Vec2& out);                                         // 0x182AFEE18
    // Place/replace the current level's checkpoint mark at the flash map cursor
    // ("GetCurrentCursorPosX/Y" element getters); gated on m_flag608.
    void PlaceCheckpointMarkAtCursor();                                           // 0x182AFDC74
    // Remove the current level's checkpoint mark: destroy its live C_CompassMark, erase it
    // from m_checkpointMarks (+0x588), send flash "ResetCheckpoint" (sub_180DCB1A0) and
    // refresh the "ui_invhint_checkpoint" help hint.  KCD2 analogue of KCD1's flash
    // "RemoveCheckpoint" invoke.  No-op when the level has no mark.
    void RemoveCheckpointMark();                                                  // 0x182B00ACC
    // The "ApseMap" cursor position (same space as GetCheckpointMarkPos), read on the bound
    // m_content element via the engine flash-var primitive: IUIElement::CallFunction [69] of the
    // registered "GetCurrentCursorPosX"/"GetCurrentCursorPosY" getters (NOT IFlashPlayer::Invoke,
    // which can't see UIElement <functions>).  False if unbound or a call fails.
    bool GetMapCursorWorldPos(Vec2& out);                                         // sub_180B5BBB8 x2

    // Draw the FT route polyline: each node's (x,y) -> flash "FastTravelPath", then
    // "ShowFastTravelPath" on the "ApseMap" element (Z dropped).  Both the hover preview
    // (sub_182AFFE7C) and the deferred map-show draw (sub_181F4A750) funnel here.
    void DrawFastTravelRoute(std::vector<Vec3>& nodes);                          // 0x182B016B0

    C_UITreeList               m_legendList;         // +0x028  (0x1E0) "ApseMapLegendList" flash list
    C_UIMapContent             m_content;            // +0x208  (0x168) "ApseMap" flash screen
    C_UIApse*                  m_pApse;              // +0x370  owner (ctor a2)
    C_UIModalDialogQuestion    m_modalDialog;        // +0x378  (0xE8) "ApseModalDialog"
    C_UIModalDialogRandomEvent m_randomEventDialog;  // +0x460  (0x128) "ApseModalDialog"; FT ambush prompt
    std::vector<std::shared_ptr<C_CheckpointMark>> m_checkpointMarks;   // +0x588  one per level (save chunk 0xC20)
    std::vector<std::shared_ptr<S_EntityMapMark>>  m_entityMarks;       // +0x5A0  persistent mark requests
    std::map<int32_t, std::shared_ptr<C_CompassMark>> m_activeMarks;    // +0x5B8  mark id -> live mark (visible ones only)
    void*                      m_pFTDestination;     // +0x5C8  FT destination object -- set by the hover-highlight draw (sub_182AFFE7C), NOT by SetDestination; vf[0] -> name for flash "FastTravelStarted"; zeroed on finish + by the route clears
    bool                       m_isFastTravelling;   // +0x5D0  set/cleared by I_UIFastTravel [1]/[2]
    uint8_t                    _pad5D1[7];           // +0x5D1
    bool                       m_ftPathPending;      // +0x5D8  DEFERRED-DRAW flag: SetDestination [0] sets it; the map-show handler sub_181F4A750 is the SOLE consumer (draws m_ftPathNodes then clears it).  NOT reset by finish/hide/cancel -- zero it after StartTravel or the route redraws on the next map open
    uint8_t                    _pad5D9[7];           // +0x5D9
    std::vector<Vec3>          m_ftPathNodes;        // +0x5E0  route polyline nodes: filled by SetDestination, drawn via sub_181F4A750 -> DrawFastTravelRoute
    bool                       m_ftFlag5F8;          // +0x5F8  companion-icon enum (1=Group/2=Woman/3=Couple), read by sub_181F4A750; SetDestination arg a3
    uint8_t                    _pad5F9[7];           // +0x5F9
    uint64_t                   m_unk600;             // +0x600  ctor memset 0 [role UNVERIFIED]
    bool                       m_flag608;            // +0x608  gates PlaceCheckpointMarkAtCursor [writer not traced -- UNVERIFIED]
    uint8_t                    _pad609[7];           // +0x609
    CryStringT<char>           m_currentMapName;     // +0x610  ctor: TLS magic-static empty literal; 4th flash "SetPlayer" arg [role INFERRED]
    bool                       m_poiFilterVisible[0x61];   // +0x618  POI-filter visibility by id (fill sub_181F4F6C0; save chunk 0xC1E)
    uint8_t                    _pad679[7];           // +0x679
    std::vector<S_Unk38>       m_vec680;             // +0x680  0x38-byte elements (dtor sub_18178736C); role UNVERIFIED
    C_UIMapCloudAtlas          m_cloudAtlas;         // +0x698  (0x20) fog-of-war cloud overlay ("Libs\UI\mapClouds.xml")
    void*                      m_pFaderHandle;       // +0x6B8  owning smart-handle to a refcounted "UIMap" fade obj: OnFastTravelStarted acquires m_pModule->m_pFaderController->vtbl[0](&out,"UIMap",0), move-assigns via sub_180C08F90, activates/releases pointee via vf[1](bool); pointee class UNRESOLVED
    ::ICVar*                   m_pCVarMoveCursorSens;// +0x6C0  cached "wh_map_MoveCursorSens"
    C_ScriptBindMap            m_scriptBindMap;      // +0x6C8  (0x70) Lua "UIMap" (GoToCheckpointMark only)
    std::map<const char*, S_ActionHandler> m_actionHandlers;   // +0x738  action id -> handler (OnAction 0x18168C72C; K comparator UNVERIFIED)
    bool                       m_mapViewActive;      // +0x748  set while the apse shows a map view (OnApseViewChanged 0x180562300); gates flash pushes
    bool                       m_flag749;            // +0x749  ctor 0 [role UNVERIFIED]
    bool                       m_flag74A;            // +0x74A  ctor 0 [role UNVERIFIED]
    uint8_t                    _pad74B[5];           // +0x74B
};
static_assert(sizeof(C_UIMap) == 0x750, "C_UIMap must be 0x750 (C_UIApse +0x1508..+0x1C58)");
static_assert(offsetof(C_UIMap, m_legendList) == 0x028, "legend list at 0x28 (ctor 0x18146c418)");
static_assert(offsetof(C_UIMap, m_content) == 0x208, "map content at 0x208 (ctor 0x18146c424)");
static_assert(offsetof(C_UIMap, m_randomEventDialog) == 0x460, "random-event dialog at 0x460");
static_assert(offsetof(C_UIMap, m_checkpointMarks) == 0x588, "checkpoint marks at 0x588");
static_assert(offsetof(C_UIMap, m_currentMapName) == 0x610, "map name at 0x610");
static_assert(offsetof(C_UIMap, m_poiFilterVisible) == 0x618, "filter flags at 0x618 (0x61 entries)");
static_assert(offsetof(C_UIMap, m_cloudAtlas) == 0x698, "cloud atlas at 0x698");
static_assert(offsetof(C_UIMap, m_scriptBindMap) == 0x6C8, "script bind at 0x6C8");
static_assert(offsetof(C_UIMap, m_actionHandlers) == 0x738, "action map at 0x738");

}  // namespace wh::guimodule
