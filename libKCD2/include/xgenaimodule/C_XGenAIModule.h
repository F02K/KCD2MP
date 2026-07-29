#pragma once
#include <cstdint>
#include <cstddef>
#include "../framework/C_BaseModule.h"
#include "../framework/I_ReadinessTask.h"
#include "../Offsets/vtables/IGameFrameworkListener.h"
#include "../Offsets/vtables/ISystem.h"
#include "../CryEngine/CryCommon/CryString.h"
#include "../game/S_GameContext.h"
#include "S_RPGPerceptionHelper.h"

// -----------------------------------------------
// wh::xgenaimodule::C_XGenAIModule -- the XGen AI module spine (KCD2 1.5.6, kd7u).
// sizeof 0x1D0 (alloc 464).
// -----------------------------------------------
// RTTI TD rva 0x4FF3590. ctor sub_181889578; registrar sub_180C9DA44 (alloc 464, ctor,
// Init, publishes into S_GameContext +0x160). GetModuleId = 12 (sub_181A74D00),
// GetModuleName = "XGenAIModule" (sub_181A71A00).
//
// Base subobjects (RTTI BCD walk + ctor vtable writes, all [V]):
//   +0x00  framework::C_BaseModule (primary vtable 0x184012f10, 10 slots =
//          C_BaseModule shape [0..6] + RTTR trio [7..9]; brings I_ModuleMessageListener)
//   +0x10  Offsets::IGameFrameworkListener  (subobject vtable 0x184012ec0, 9 slots;
//          only [1] OnPostUpdate sub_180FA08C0 and [6] OnPreRender sub_180EC4660 non-null)
//   +0x18  wh::I_ReadinessTask (: I_ReadinessDebuggable; subobject vtable 0x184012f88,
//          exactly 5 slots, NO dtor -- the readiness chain is dtor-less, see I_ReadinessTask.h)
//   +0x20  Offsets::ISystemEventListener (subobject vtable 0x184012f68, 3 slots;
//          [2] OnSystemEvent sub_181350E18)
//
// Init (slot [2], sub_1807D2BB4): registers rttr type "C_XGenAIModule"/"XGen", loads
// "TypeDefinitions.xml", registers cvar wh_ai_PostLoadErrorForceCancel (sub_181AAB740),
// registers the three listener subobjects (+0x10 GameFramework "XGen", +0x18 readiness
// system, +0x20 system-event dispatcher "C_XGenAIModule"), builds the singletons hub
// (+0xA8, alloc 8) and the navmesh system (+0x78, alloc 448), publishes +0x70 into three
// global registries (sub_1823CB3DC()+416 / *sub_1823CE054() / sub_1823CD9FC()+112).
//
// Cheat roots: m_pSingletons (+0xA8) is the AI service locator (66 getters -- crime,
// perception, hearing, key access, light/stealth; see I_XGenAIModuleSingletons.h).
// Only +0x78 / +0xA0 / +0xA8 member types are certified; the rest are opaque regions
// with ctor init values preserved in comments (offsets [V], C++ types [U]).

namespace wh::xgenaimodule::navigation { class C_Navigation; }   // recast/detour navmesh system (unported); RTTI .?AVC_Navigation@navigation@xgenaimodule@wh@@
namespace wh::shared { template <typename T> class C_RegularGridDynamic; }

namespace wh::xgenaimodule {

class C_XGenAIModuleSingletons;
class C_AIObject;
class C_SmartAreaManager;
class C_SmartObjectsManager;

class C_XGenAIModule
    : public framework::C_BaseModule            // +0x00  (0x10; brings I_ModuleMessageListener)
    , public Offsets::IGameFrameworkListener    // +0x10  (0x08)
    , public wh::I_ReadinessTask                // +0x18  (0x08; brings I_ReadinessDebuggable)
    , public Offsets::ISystemEventListener      // +0x20  (0x08)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_XGenAIModule;
    uint8_t   m_list0[0x10];          // +0x28  ctor sub_1808DDC28 -- intrusive list/deque head [type UNVERIFIED]
    uint8_t   m_list1[0x10];          // +0x38  ctor sub_1808DDC28 [type UNVERIFIED]
    uint8_t   m_list2[0x10];          // +0x48  ctor sub_1808DDC28 [type UNVERIFIED]
    std::vector<void*> m_vec58;       // +0x58  std::vector {first,last,end} 0x18; ctor zeroes all
                                      //        3 (0x181889600..0x18188960c); dtor 0x18342FAB5 frees
                                      //        first, cap=(end-first)&~7, no per-elem dtor -> 8-byte POD
                                      //        elems [element type UNVERIFIED]
    shared::C_RegularGridDynamic<C_AIObject*>* m_publishedHandle;  // +0x70  writer sub_1817A346C:
                                      //        alloc 0x238, built by sub_1817A359C (feeds std::function<bool(C_AIObject*)>
                                      //        callbacks). Init mirrors into 3 global registries. [name kept for static_assert]
    navigation::C_Navigation* m_pNavigation;   // +0x78  navmesh system (Init: alloc 448,
                                      //        ctor sub_180D39C78(_, qword_185493D30+8))
    uint8_t   m_b80;                  // +0x80  ctor 0
    uint8_t   _pad81[7];              // +0x81
    void*     m_p88;                  // +0x88  ctor 0
    C_SmartAreaManager* m_pSmartAreaManager;   // +0x90  Init sub_1807D24D8(): alloc 0x460,
                                      //        ctor sub_180BEB26C, also cached in singleton qword_185493BB0
    C_SmartObjectsManager* m_pSmartObjectsManager;  // +0x98  Init sub_1807D2540(): alloc 0x5C0,
                                      //        ctor sub_180BEB428, also cached in singleton qword_185493D30
    S_RPGPerceptionHelper m_perceptionHelper;  // +0xA0  embedded (0x08, vtable 0x184772508)
    C_XGenAIModuleSingletons* m_pSingletons;   // +0xA8  THE HUB (Init: alloc 8, sub_1807D3078)
    CryStringT<char> m_strB0;         // +0xB0  ctor assigns global empty-string literal
                                      //        [CryStringT flavor UNVERIFIED]
    uint8_t   _padB8[0x10];           // +0xB8  not ctor-inited [UNVERIFIED region]
    uint8_t   _zeroC8[0x50];          // +0xC8  ctor zero-fill (opaque POD region)
    uint8_t   m_b118;                 // +0x118 ctor 0
    uint8_t   _pad119[3];             // +0x119
    uint32_t  m_d11C;                 // +0x11C ctor 0
    void*     m_rttrList;             // +0x120 ctor = &unk_18566BFB8 (shared static empty buffer,
                                      //        entry count at ptr-4); head of a growable array of 16-byte
                                      //        {ctx,handler} listener entries. ctor self-registers via
                                      //        sub_1805FEFF4 (linear find) + sub_1817A24F8 (insert). [type kept opaque]
    uint16_t  m_w128;                 // +0x128 ctor 0xFFFF
    uint16_t  m_w12A;                 // +0x12A ctor 0xFFFF
    uint8_t   _pad12C[4];             // +0x12C
    uint8_t   m_b130;                 // +0x130 ctor 1
    uint8_t   _pad131[7];             // +0x131
    uint16_t  m_w138;                 // +0x138 ctor 0
    uint8_t   _pad13A[6];             // +0x13A
    int64_t   m_q140;                 // +0x140 ctor -1
    uint8_t   m_map148[0x48];         // +0x148 ctor sub_18078BDEC -- open-addressing
                                      //        hashmap, elem size 24 [opaque]
    void*     m_p190;                 // +0x190 ctor 0
    int32_t   m_atomic198;            // +0x198 ctor InterlockedExchange 0
    uint8_t   _pad19C[4];             // +0x19C
    uint16_t  m_w1A0;                 // +0x1A0 ctor 0
    uint8_t   m_b1A2;                 // +0x1A2 ctor 0
    uint8_t   _pad1A3[5];             // +0x1A3
    uint64_t  m_q1A8;                 // +0x1A8 ctor 0
    uint64_t  m_q1B0;                 // +0x1B0 ctor 0
    uint64_t  m_q1B8;                 // +0x1B8 ctor 0
    uint16_t  m_w1C0;                 // +0x1C0 ctor 0
    uint8_t   _pad1C2[6];             // +0x1C2
    int32_t   m_d1C8;                 // +0x1C8 ctor 0x7FFFFFFF (last ctor write)
    uint8_t   _pad1CC[4];             // +0x1CC -> sizeof 0x1D0

    [[nodiscard]] static C_XGenAIModule* GetInstance()
    {
        return game::S_GameContext::GetInstance()->m_pXGenAIModule;
    }
};
static_assert(sizeof(C_XGenAIModule) == 0x1D0, "C_XGenAIModule must be 0x1D0 (alloc 464)");
static_assert(offsetof(C_XGenAIModule, m_list0) == 0x28, "first member after 4 base vptr blocks");
static_assert(offsetof(C_XGenAIModule, m_publishedHandle) == 0x70, "published handle at 0x70");
static_assert(offsetof(C_XGenAIModule, m_pNavigation) == 0x78, "navmesh at 0x78");
static_assert(offsetof(C_XGenAIModule, m_perceptionHelper) == 0xA0, "perception helper at 0xA0");
static_assert(offsetof(C_XGenAIModule, m_pSingletons) == 0xA8, "singletons hub at 0xA8");
static_assert(offsetof(C_XGenAIModule, m_strB0) == 0xB0, "string at 0xB0");
static_assert(offsetof(C_XGenAIModule, m_rttrList) == 0x120, "rttr list at 0x120");
static_assert(offsetof(C_XGenAIModule, m_map148) == 0x148, "hashmap at 0x148");
static_assert(offsetof(C_XGenAIModule, m_d1C8) == 0x1C8, "sentinel int at 0x1C8");

}  // namespace wh::xgenaimodule
