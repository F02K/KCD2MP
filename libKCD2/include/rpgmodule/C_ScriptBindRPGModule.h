#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"
#include "C_FactionScriptBind.h"
#include "C_LocationScriptBind.h"

// -----------------------------------------------
// wh::rpgmodule::C_ScriptBindRPGModule -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x130.
// -----------------------------------------------
// RTTI .?AVC_ScriptBindRPGModule@rpgmodule@wh@@ (CHD = [self, CScriptableBase], mdisp 0)
// vftable 0x183B552B0  ctor sub_1812E267C  creator sub_1812E2A34  owned by C_RPGModule @+0xF0.
// Embeds C_FactionScriptBind @+0x70 and C_LocationScriptBind @+0xD0 by value (0x70+0x60+0x60=0x130,
// offsets verified in ctor disasm).
//
// SetGlobalName("RPG") -- this IS the Lua global `RPG` table. After registration the ctor installs
// setmetatable(RPG, {__index = RPG._GetConstant, __newindex = RPG._SetConstant}) (pSS vf byte 0x30).
// NOTE: only _GetConstant is registered in sub_1812E1BD4; _SetConstant was NOT found in this
// registration fn (registered elsewhere or RPG is effectively read-only for unknown keys -- flagged).
//
// Lua fn table `RPG` (13 fns; registrar sub_1812E1BD4):
//   _GetConstant                0x1810BB7A4  (table, key)    metatable __index
//   GetLocations                0x182CF5570  ()
//   GetLocationById             0x182CF52AC  (id)
//   GetLocationByName           0x182CF536C  (name)
//   GetFactions                 0x182CF4904  ()
//   GetIndulgencePrice          0x182CF4D94  ()
//   GetFactionById              0x182CF46F8  (factionId)
//   IsPublicEnemy               0x182CF844C  (wuid)
//   ReconcileWithPublicFriends  0x182CFC740  ()
//   AddLocationPoint            0x18188F6BC  (entityId)
//   CaptionObjectUsed           0x182CF1598  (objectEntityId, isDiscovered)
//   NotifyLevelXpGain           0x182CF8D64  (skill)
//   AddStatXP                   0x182CEE3DC  (rpgEvent, soulId, statName, totalXp, oldProgress, newProgress)

namespace wh::rpgmodule {

class C_ScriptBindRPGModule : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindRPGModule;
    Offsets::IScriptSystem* m_pSS2;      // +0x60  raw ctor pSS (a2)
    C_RPGModule*            m_pOwner;    // +0x68  owning module (ctor a4 = the C_RPGModule that creates the bind, stored back at module+0xF0)
    C_FactionScriptBind     m_faction;   // +0x70  (0x60) faction-userdata metatable methods
    C_LocationScriptBind    m_location;  // +0xD0  (0x60) location-userdata metatable methods
};
static_assert(sizeof(C_ScriptBindRPGModule) == 0x130, "C_ScriptBindRPGModule must be 0x130");
static_assert(offsetof(C_ScriptBindRPGModule, m_faction) == 0x70, "faction sub-bind at 0x70");
static_assert(offsetof(C_ScriptBindRPGModule, m_location) == 0xD0, "location sub-bind at 0xD0");

}  // namespace wh::rpgmodule
