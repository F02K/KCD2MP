#pragma once
#include <cstdint>
#include "REL/ID.h"
#include "Offsets/vtables/IActorSystem.h"

// -----------------------------------------------
// wh::game::S_GameContext -- KCD2 WHGame.dll 1.5.6 (kd7u). Non-polymorphic POD registry.
// -----------------------------------------------
// Global qword_18549D388 (RVA 0x549D388) holds the singleton pointer.
// Getter sub_1809155C8 (allocs 0x1F0)  ctor sub_1819A7330 (zero-inits, no vtable).
// Holds pointers to all Warhorse game modules; module block built/registered by
// sub_180C9DA44. Flat POD, no RTTI.
//
// [MODERATE vs KCD1] sizeof 0x170 -> 0x1F0; module block relocated/reordered and
// gained C_MusicModule (+0x108) and C_ConceptModule (+0x120).
// [MODERATE vs KCD1] m_pPlayerModule +0xE8 -> +0x128.
// [MODERATE vs KCD1] m_pActorSystem  +0x128 -> +0x180; GetActor slot unchanged.
// [MODERATE vs KCD1] singleton global qword_1834FFD10 -> qword_18549D388.
//
// Verified members are named; unpinned module slots (+0xF0/+0x118/+0x140/+0x148/
// +0x158) and framework sub-objects are padded until they get their own KCD2 ports.

namespace wh::framework        { class C_ModulesManager; class C_ExpressionEngine; }
namespace wh::entitymodule     { class C_EntityModule; class C_Actor; }
namespace wh::animationmodule  { class C_AnimationModule; }
namespace wh::xbehaviormodule  { class C_XBehaviorModule; }
namespace wh::guimodule        { class C_GUIModule; }
namespace wh::combatmodule     { class C_CombatModule; }
namespace wh::soundmodule      { class C_SoundModule; }
namespace wh::musicmodule      { class C_MusicModule; }
namespace wh::shopmodule       { class C_ShopModule; }
namespace wh::conceptmodule    { class C_ConceptModule; }
namespace wh::questmodule      { class C_QuestModule; }
namespace wh::playermodule     { class C_PlayerModule; }
namespace wh::rpgmodule        { class C_RPGModule; }
namespace wh::environmentmodule{ class C_EnvironmentModule; }
namespace wh::dialogmodule     { class C_DialogModule; }
namespace wh::xgenaimodule     { class C_XGenAIModule; }
namespace wh::utilsmodule      { class C_UtilsModule; }
namespace wh::databasemodule   { class C_DatabaseModule; }

namespace wh::game {

struct S_GameContext {
    uint8_t                                 _pad00[0x18];         // +0x00
    void*                                   m_pFramework;         // +0x18  framework/system root (ctor arg0)  VERIFIED
    uint8_t                                 _pad20[0x40];         // +0x20
    framework::C_ExpressionEngine*          m_pExpressionEngine;  // +0x60  EE core (= qword_185168B30); bootstrap sub_180C103AC stores it @0x180C104EC  VERIFIED
    uint8_t                                 _pad68[0x50];         // +0x68
    framework::C_ModulesManager*            m_pModulesManager;    // +0xB8  dispatches C_ModuleMessage          VERIFIED
    uint8_t                                 _padC0[0x20];         // +0xC0
    entitymodule::C_EntityModule*           m_pEntityModule;      // +0xE0                                      VERIFIED
    guimodule::C_GUIModule*                 m_pGUIModule;         // +0xE8                                      VERIFIED
    animationmodule::C_AnimationModule*     m_pAnimationModule;   // +0xF0  registrar v2[30]=sub_180D24284  VERIFIED
    combatmodule::C_CombatModule*           m_pCombatModule;      // +0xF8                                      VERIFIED
    soundmodule::C_SoundModule*             m_pSoundModule;       // +0x100                                     VERIFIED
    musicmodule::C_MusicModule*             m_pMusicModule;       // +0x108  NEW in KCD2                        VERIFIED
    shopmodule::C_ShopModule*               m_pShopModule;        // +0x110                                     VERIFIED
    questmodule::C_QuestModule*             m_pQuestModule;       // +0x118  (registrar sub_180C9DA44)   VERIFIED
    conceptmodule::C_ConceptModule*         m_pConceptModule;     // +0x120  NEW in KCD2                        VERIFIED
    playermodule::C_PlayerModule*           m_pPlayerModule;      // +0x128  (was +0xE8)                        VERIFIED
    rpgmodule::C_RPGModule*                 m_pRPGModule;         // +0x130                                     VERIFIED
    environmentmodule::C_EnvironmentModule* m_pEnvironmentModule; // +0x138                                     VERIFIED
    utilsmodule::C_UtilsModule*             m_pUtilsModule;       // +0x140 registrar v2[40] vftable C_UtilsModule  VERIFIED
    databasemodule::C_DatabaseModule*       m_pDatabaseModule;    // +0x148 registrar v2[41]=sub_18192BBB4()+0x10 subobj  VERIFIED
    dialogmodule::C_DialogModule*           m_pDialogModule;      // +0x150                                     VERIFIED
    xbehaviormodule::C_XBehaviorModule*     m_pXBehaviorModule;   // +0x158 registrar v2[43] vftable C_XBehaviorModule  VERIFIED
    xgenaimodule::C_XGenAIModule*           m_pXGenAIModule;      // +0x160                                     VERIFIED
    uint8_t                                 _pad168[0x18];        // +0x168
    Offsets::IActorSystem*                  m_pActorSystem;       // +0x180  CActorSystem (CryAction)           VERIFIED
    uint8_t                                 _pad188[0x68];        // +0x188

    [[nodiscard]] static S_GameContext* GetInstance()
    {
        return *reinterpret_cast<S_GameContext**>(REL::ID(2357).address());
    }

    // Look up an actor by id and cast to C_Actor. GetActor slot (+0x18) unchanged KCD1->KCD2.
    [[nodiscard]] wh::entitymodule::C_Actor* GetActorById(EntityId id) const
    {
        return m_pActorSystem
            ? reinterpret_cast<wh::entitymodule::C_Actor*>(m_pActorSystem->GetActor(id))
            : nullptr;
    }
};
static_assert(sizeof(S_GameContext) == 0x1F0, "S_GameContext must be 0x1F0");

}  // namespace wh::game
