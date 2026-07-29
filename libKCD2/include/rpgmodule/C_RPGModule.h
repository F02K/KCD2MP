#pragma once
#include <cstdint>
#include <vector>
#include "../framework/C_BaseModule.h"
#include "../framework/C_ParallelModuleUpdater.h"
#include "S_RPGCVars.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::C_RPGModule -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x120 (CONFIRMED: new(0x120)).
// -----------------------------------------------
// RTTI .?AVC_RPGModule@rpgmodule@wh@@ (TD 0x184D60440; CHD 0x1843E8540, 5 bases)
// primary vtable 0x183A84028 (10 slots)  secondary vtable 0x183A84080 (@+0x10)
// ctor sub_180BE8EEC  dtor sub_182CED32C  factory sub_180C9DA44  SINGLETON qword_1853322A0.
//
// [FUNDAMENTAL vs KCD1] KCD1 was 0x5F0; KCD2 shrank to 0x120 -- C_BaseModule collapsed to 0x10
// and the former inline members became owned heap sub-objects. KCD1's "0x48 unknown" is now
// identified as C_Calendar and the "0x1430 unknown" as C_RPGMinigames. The 0xA8..0x118 tail was
// fully resolved by the backlog deep map (Init sub_1807D1954 wires it): the cvar POD, two class-
// registry handles, a body-part index vector, the four RPG ScriptBinds, and two lifecycle-toggle
// registration records.
//
// NOT MEMBERS (RTTR reflected getters resolve to EXTERNAL globals, verified):
//   "Statistics"      -> qword_18548E320 (C_Statistics, 2176B)
//   "FactionManager"  -> qword_1854B88C0 (C_FactionManager Meyers singleton)
//   "SkirmishManager" -> qword_18548E308 (C_SkirmishManager, 232B)
//   "Constants"       -> &unk_1855E63D0 (S_Constants)
// ("SoulList" / "Calendar" getters DO return the +0x80 / +0x88 members.)

namespace wh::rpgmodule {

class C_SoulList;
class C_Calendar;             // 0xA0  (ctor sub_1818424B4)
class C_RPGDialog;            // 0x08  (vtable-only shell)
class C_RPGMinigames;         // 0x1438 (ctor sub_1817B45CC; module back-ptr @pointee+8)
class C_RPGQuest;             // 0x08  (vtable-only shell)
class C_EventManager;         // 0x40
class C_RPGLocationManager;   // 0x2B0 (ctor sub_180E0F960; MI subobj @+8; KCD1 was 0x298)
class C_ScriptBindRPGModule;  // 0x130 (embeds C_FactionScriptBind @+112, C_LocationScriptBind @+208)
class C_ScriptBindSoul;       // 0x78
class C_ScriptBindCalendar;   // 0x68
class C_ScriptBindStatistics; // 0x60

// 16-byte lifecycle-toggle registration record (SYNTHETIC name; no RTTI). Registers the module (via
// module+0x88 object) as a named "RPGModule" provider in one of two manager lists selected by m_type.
struct S_RPGModuleRegistration {
    CryStringT<char> m_name;   // +0x00  = "RPGModule"
    int32_t          m_type;   // +0x08  0 -> slot +0x110, 1 -> slot +0x118
    uint32_t         _pad0C;   // +0x0C
};
static_assert(sizeof(S_RPGModuleRegistration) == 0x10, "S_RPGModuleRegistration must be 0x10");

class C_RPGModule
    : public wh::framework::C_BaseModule              // +0x00  (0x10; vptr also serves I_ModuleMessageListener)
    , public wh::framework::C_ParallelModuleUpdater   // +0x10  (0x70; vptr also serves IGameFrameworkListener)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGModule;
    // Owned sub-objects, all ctor-allocated (alloc size + pointee RTTI vtable verified):
    // Instance global qword_1853322A0 (written by ctor sub_180BE8EEC; also read by the
    // soul ability worker sub_1809DCC70). Impl in src/rpgmodule/rpgmodule.cpp.
    static C_RPGModule* GetInstance();

    C_SoulList*     m_pSoulList;          // +0x80  (0x100220, ctor sub_180BEAE80)  VERIFIED
    C_Calendar*     m_pCalendar;          // +0x88  (0xA0)  VERIFIED
    C_RPGDialog*    m_pRPGDialog;         // +0x90  (8)  VERIFIED
    C_RPGMinigames* m_pMinigames;         // +0x98  (0x1438)  VERIFIED
    C_RPGQuest*     m_pRPGQuest;          // +0xA0  (8)  VERIFIED
    S_RPGCVars*     m_pCVars;             // +0xA8  the 0x158 cvar POD (raw alloc, no ctor; consumer sub_181010980)
    C_EventManager* m_pEventManager;      // +0xB0  (0x40)  VERIFIED
    C_RPGLocationManager* m_pLocationManager;  // +0xB8  (0x2B0)  VERIFIED
    void*    m_soulClassReg;              // +0xC0  NON-owning class-registry handle ("soul_class"): C_RPGModule dtor sub_182CED32C never releases a1[24]; ctor zero-inits +0xC0; assigning writer not located
    void*    m_skaldCharacterReg;         // +0xC8  NON-owning class-registry handle ("skald_character"): C_RPGModule dtor sub_182CED32C never releases a1[25]; assigning writer not located
    std::vector<int32_t> m_bodyPartIndexTable;  // +0xD0  head-face/body-part index table (sized dword_185324478+1)
    int32_t  m_headFaceAttachIdx;         // +0xE8  init -1 (from "head_face" lookup)
    uint32_t _padEC;                      // +0xEC
    C_ScriptBindRPGModule* m_pScriptBindRPG;    // +0xF0  (0x130, creator sub_1812E2A34)  VERIFIED
    C_ScriptBindSoul*      m_pScriptBindSoul;   // +0xF8  (0x78)  VERIFIED
    C_ScriptBindCalendar*  m_pScriptBindCalendar; // +0x100 (0x68)  VERIFIED
    C_ScriptBindStatistics* m_pScriptBindStatistics; // +0x108 (0x60)  VERIFIED
    S_RPGModuleRegistration* m_pReg0;     // +0x110  type-0 registration (dtor sub_1808DB160)
    S_RPGModuleRegistration* m_pReg1;     // +0x118  type-1 registration
};
static_assert(sizeof(C_RPGModule) == 0x120, "C_RPGModule must be 0x120");
static_assert(offsetof(C_RPGModule, m_pSoulList) == 0x80, "own members start at 0x80");
static_assert(offsetof(C_RPGModule, m_bodyPartIndexTable) == 0xD0, "body-part table at 0xD0");
static_assert(offsetof(C_RPGModule, m_pReg1) == 0x118, "last member at 0x118");

}  // namespace wh::rpgmodule
