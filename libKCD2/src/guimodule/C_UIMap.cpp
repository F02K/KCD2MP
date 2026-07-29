#include "guimodule/C_UIMap.h"
#include "guimodule/C_UICompass.h"       // map-vertical wave G3a: compile the compass +
#include "guimodule/C_ShowMapMarker.h"   // concept-marker headers (not in kcd.h yet)
#include "game/S_GameContext.h"
#include "Offsets/Offsets.h"
#include "Offsets/vtables/IUIElement.h"   // IUIElement::CallFunction [69] (UIElement-registered getters)
#include "guimodule/SUITypes.h"           // SUIArguments / TUIData (native flash value types, global ns)

// C_UIMap forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// KCD1 PORT NOTE: KCD1's only C_UIMap forwarder, ForceStartFastTravel (KCD1
// sub_18112C494: fired "OnForceStartFastTravel{Pos, PlayerId, IconType}" to Flash and
// blocked the FT sim = the visual-only wh_pl_FastTravelTo warp), has NO KCD2
// equivalent on C_UIMap -- the string "OnForceStartFastTravel" is absent from the
// KCD2 binary and the fake/visual travel path moved wholly into
// playermodule::C_FastTravel (StartFastTravel fake flag, wh_pl_FakeFastTravelTo).
// SKIPPED, not ported.  The forwarders below are the KCD2 map cheat surface instead.

namespace wh::guimodule {

C_UIMap* C_UIMap::GetInstance()
{
    // Composition verbatim from the UIMap.GoToCheckpointMark Lua handler 0x182BA7810:
    // apse = sub_1805677CC(S_GameContext+0xE8); uiMap = apse + 0x1508.  The getter
    // caches the rttr-resolved C_UIApse* in qword_18548B050 and walks
    // C_GUIModule::m_uiElements on a cache miss.
    using GetApseFn = void*(__fastcall*)(void* pGUIModule);
    static REL::Relocation<GetApseFn> getApse{ REL::ID(15) };  // sub_1805677CC

    auto* ctx = wh::game::S_GameContext::GetInstance();
    if (!ctx || !ctx->m_pGUIModule)
        return nullptr;
    void* pApse = getApse(ctx->m_pGUIModule);
    if (!pApse)
        return nullptr;
    return reinterpret_cast<C_UIMap*>(reinterpret_cast<uintptr_t>(pApse) + 0x1508);
}

bool C_UIMap::IsPoiFilterVisible(uint32_t filterId) const
{
    // sub_180C4DAB0: folds DLC ids through sub_180A1B1C4, bounds-checks (> 0x60 ->
    // false) and reads m_poiFilterVisible[id] (+0x618).
    using Fn = bool(__fastcall*)(const C_UIMap*, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(31) };
    return fn(this, filterId);
}

void C_UIMap::SetPoiMarkersVisible(uint32_t filterId, bool visible)
{
    // sub_182B02B34: for every m_entityMarks record of the (folded) filter group,
    // materializes (sub_181F47470 make_shared<C_CompassMark> + C_UICompass add
    // sub_180C4D1E8 + m_activeMarks insert) or tears down the live mark, then --
    // while m_mapViewActive -- sends "AddPoiMarkers"/"RemovePoiMarkers"("PoiMarkers",
    // ids) to the "ApseMap" flash element.  Also the OnModuleMessage-0x41 and
    // side-effect handlers' worker.
    using Fn = void(__fastcall*)(C_UIMap*, uint32_t, char);
    static REL::Relocation<Fn> fn{ REL::ID(88) };
    fn(this, filterId, visible);
}

std::shared_ptr<S_EntityMapMark>& C_UIMap::GetOrCreateEntityMark(
    std::shared_ptr<S_EntityMapMark>& out, uint32_t type,
    wh::xgenaimodule::C_LinkableObject* pObject, uint32_t param)
{
    // sub_180C4DBD8: find_if over m_entityMarks by {type, pObject, param}
    // (sub_181F46B30); on miss make_shared<S_EntityMapMark> (sub_180C4E2E8, id from
    // the global counter).  Does NOT register -- pair with RegisterEntityMark
    // (the C_ShowMapMarker trigger 0x180C4DADC calls both).
    using Fn = std::shared_ptr<S_EntityMapMark>*(__fastcall*)(
        C_UIMap*, std::shared_ptr<S_EntityMapMark>*, uint32_t,
        wh::xgenaimodule::C_LinkableObject*, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(32) };
    return *fn(this, &out, type, pObject, param);
}

void C_UIMap::RegisterEntityMark(std::shared_ptr<S_EntityMapMark>& mark)
{
    // sub_180C4DD5C: append to m_entityMarks (duplicate -> ++m_useCount only); if the
    // mark's filter group passes IsPoiFilterVisible + the global gate sub_180C4D10C,
    // spawn the live C_CompassMark into m_activeMarks and C_UICompass::m_marks.
    using Fn = char(__fastcall*)(C_UIMap*, std::shared_ptr<S_EntityMapMark>*);
    static REL::Relocation<Fn> fn{ REL::ID(33) };
    fn(this, &mark);
}

std::shared_ptr<C_CheckpointMark>& C_UIMap::FindCurrentLevelCheckpointMark(
    std::shared_ptr<C_CheckpointMark>& out)
{
    // sub_180DCB460: current level id (S_GameContext+0xC8 -> obj -> vf[0]) matched
    // against C_CheckpointMark::m_levelId over m_checkpointMarks; out = {null, null}
    // when the level has no player mark.
    using Fn = std::shared_ptr<C_CheckpointMark>*(__fastcall*)(
        C_UIMap*, std::shared_ptr<C_CheckpointMark>*);
    static REL::Relocation<Fn> fn{ REL::ID(37) };
    return *fn(this, &out);
}

void C_UIMap::GetCheckpointMarkPos(Vec2& out)
{
    // sub_182AFEE18: xy of the current level's checkpoint mark (C_CompassMark::m_pos),
    // (0, 0) when none.  UIMap.GoToCheckpointMark (0x182BA7810) teleports the player
    // here with z = terrain elevation.
    using Fn = void(__fastcall*)(C_UIMap*, Vec2*);
    static REL::Relocation<Fn> fn{ REL::ID(85) };
    fn(this, &out);
}

void C_UIMap::PlaceCheckpointMarkAtCursor()
{
    // sub_182AFDC74: gated on m_flag608; pulls the cursor via the "ApseMap" element
    // getters "GetCurrentCursorPosX"/"GetCurrentCursorPosY", z from terrain, then
    // replaces the current level's checkpoint mark (make_shared wrapper
    // sub_182AFB3DC) and pushes it to flash.  Bound as a map action handler
    // (m_actionHandlers; action-thunk sub_181F4E3B0).
    using Fn = void(__fastcall*)(C_UIMap*);
    static REL::Relocation<Fn> fn{ REL::ID(84) };
    fn(this);
}

void C_UIMap::RemoveCheckpointMark()
{
    // sub_182B00ACC: FindCurrentLevelCheckpointMark; if present, drop the live compass
    // mark (C_UICompass sub_180A193C0), erase the shared_ptr from m_checkpointMarks
    // (sub_182AFAE9C @ +0x588), push flash "ResetCheckpoint" (sub_180DCB1A0) and refresh
    // the help hint (sub_180DC99C4).  No-op when the level has no mark.
    using Fn = void(__fastcall*)(C_UIMap*);
    static REL::Relocation<Fn> fn{ REL::ID(86) };
    fn(this);
}

void C_UIMap::DrawFastTravelRoute(std::vector<Vec3>& nodes)
{
    // sub_182B016B0: emit each node's (x,y) as SUIArguments to flash "FastTravelPath", then
    // "ShowFastTravelPath" on the "ApseMap" element -- the visible route polyline (Z dropped).
    using Fn = void(__fastcall*)(C_UIMap*, std::vector<Vec3>*);
    static REL::Relocation<Fn> fn{ REL::ID(87) };
    fn(this, &nodes);
}

bool C_UIMap::GetMapCursorWorldPos(Vec2& out)
{
    Offsets::IUIElement* el = m_content.m_pElement;

    if (!el)
        return false;

    SUIArguments empty;
    TUIData      x, y;

    if (!el->CallFunction("GetCurrentCursorPosX", empty, &x, nullptr) || !el->CallFunction("GetCurrentCursorPosY", empty, &y, nullptr))
        return false;

    out.x = x.AsFloat();
    out.y = y.AsFloat();

    return true;
}

}  // namespace wh::guimodule
