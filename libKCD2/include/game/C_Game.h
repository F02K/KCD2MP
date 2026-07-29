#pragma once

#include <cstdint>
#include "REL/ID.h"
#include "../Offsets/vtables/IGame.h"

// -----------------------------------------------
// C_Game — Warhorse KCD2 game module (CryEngine IGame implementation)
// -----------------------------------------------
// RTTI:  .?AVC_Game@game@wh@@
// Target: WHGame.dll 1.5.6 (KCD2), image base 0x180000000
// Constructor:            0x180C10F98  (sub_180C10F8C)
// Base destructor:        0x1839D7E90  (frees sub-objects; does NOT free `this`)
// Scalar deleting dtor:   0x1839D8468  (vtable slot 0) -> operator delete(this, 0x198)
// Total size: 0x198        (recovered from the deleting dtor's sub_181AB5160(this, 408))
//
// Five polymorphic bases, each one vptr (no data), laid out at:
//   +0x00 IGame                 (primary, 41-slot vtable 0x184095688)
//   +0x08 IGameFrameworkListener
//   +0x10 ILevelSystemListener
//   +0x18 IInputEventListener
//   +0x20 wh::framework::I_ModuleMessageListener
// C_Game's own members begin at +0x28.
//
// Singleton:
//   Global qword_18549D300 (RVA 0x549D300) holds the sole C_Game*.
//   The ctor writes it (this) and the dtor clears it. See GetInstance() below.
//   A second global qword_18547E6C0 (RVA 0x547E6C0) aliases the +0x148 sub-object.
//
// Confidence: base layout, sub-manager types (from their ctor vtables) and sizeof are
// VERIFIED from disasm of the ctor/dtor. Members marked /* tentative */ are inferred
// (role only) and members named m_unk*/_pad* are zero-initialised but otherwise
// unreferenced in the ctor/dtor — treat their types as provisional.

namespace wh::framework {
    class C_RuntimePrefabManager;   // +0x150 sub-manager (0x68 bytes)
}

namespace wh::game {

class C_LevelManager;               // +0xA8 sub-manager (0xF8 bytes)
class C_ExtraRewardsManager;        // +0xB0 sub-manager (0x68 bytes)
class C_NewGameHelper;              // +0xB8 sub-manager (0xC0 bytes)
class C_PlatformActivityManager;    // +0x180 sub-manager (0x08 bytes, vtable only)
class C_GameModel;                  // +0x178 game model (0x2F0 bytes)

class C_Game : public Offsets::IGame,                    // +0x00
               public Offsets::IGameFrameworkListener,   // +0x08
               public Offsets::ILevelSystemListener,     // +0x10
               public Offsets::IInputEventListener,      // +0x18
               public Offsets::I_ModuleMessageListener   // +0x20
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Game;
    static C_Game* GetInstance() { return *reinterpret_cast<C_Game**>(REL::ID(2355).address()); }

    // ===================================================================
    // Data members (+0x28 onwards, after the five interface vptrs)
    // ===================================================================

    void*       m_unk28;                    // +0x28  (ctor=0)
    uint32_t    m_levelLoadState;           // +0x30  load-state FSM, PARTIAL: 0=ctor, 1=IGame[34] init-once (0x1839d91e4); 5->6 via ILevelSystemListener; full value set UNRECOVERED
    uint32_t    m_unk34;                    // +0x34  (ctor=0)
    uint8_t     m_flag38;                   // +0x38  (ctor=0)
    uint8_t     _pad39[3];                  // +0x39
    uint32_t    m_unk3C;                    // +0x3C  int32 written by IGame vtable[11] setter sub_180B855F4 (mov [this+3Ch], a2); role/enum unknown
    void*       m_unk40;                    // +0x40  (ctor=0)
    uint8_t     m_flag48;                   // +0x48  (ctor=0)
    uint8_t     _pad49[3];                  // +0x49
    uint8_t     _blk4C[0x20];               // +0x4C  zero-init region (coalesced unaligned stores; ~4 ptr/id pairs) /* tentative */
    uint8_t     m_flag6C;                   // +0x6C  (ctor=0)
    uint8_t     _pad6D[3];                  // +0x6D
    void*       m_pInputHandler;            // +0x70  read by IInputEventListener::OnInputEventUI /* tentative */ (ctor=0)

    // File-profiling holder (+0x78..+0x98, 0x28; ctor sub_180C10D70 registers the
    // "WH_Enable_FileProfiling" CVar callback). Holds a std::vector + a std::set/map:
    std::vector<std::string> m_profRecords;    // +0x78  begin/end/cap (0x18); element std::string (0x20), elem dtor sub_1804F44F4 = std::string::_Tidy
    void*       m_pProfListNode;            // +0x90  std::set/map _Myhead: RB-tree sentinel head (3 self-links + WORD 0x0101 {_Color=1,_Isnil=1}); nodes 0x60 (0x40 payload), recursive _Erase sub_1839D6F1C, freed 0x60 in dtor
    size_t      m_profList_x98;             // +0x98  std::set/map _Mysize (element count; ctor=0)

    // Sub-managers (all heap-allocated in the ctor; types from their own ctor vtables)
    void*                         m_pManagerA0;             // +0xA0  ptr to heap std::set/map (_Tree{_Myhead,_Mysize}=0x10; sentinel sub_180C11954: 3 self-links + WORD 0x0101; ctor sub_180C10E04; _Erase sub_180C130FC, freed 0x10 in dtor)
    wh::game::C_LevelManager*     m_pLevelManager;          // +0xA8  0xF8 bytes (sub_180C10CB0)  VERIFIED type
    wh::game::C_ExtraRewardsManager* m_pExtraRewardsManager;// +0xB0  0x68 bytes (sub_180C10BFC)  VERIFIED type
    wh::game::C_NewGameHelper*    m_pNewGameHelper;         // +0xB8  0xC0 bytes (sub_180C12704)  VERIFIED type

    // Embedded C_GameModeStartupScript (derives CScriptableBase); ctor sub_180C126E0.
    uint8_t     m_gameModeStartupScript[0x70];              // +0xC0  size 0x70  VERIFIED (dtor rebinds &CScriptableBase::vftable @+0xC0)

    void*       m_pReleasable130;           // +0x130  polymorphic heap obj (has vtable), deleted via vtable[0](this,1) in dtor; IGame[40] sub_180EC7810 writes its arg to pointee+0x70 (ctor=0)
    void*       m_pReleasable138;           // +0x138  polymorphic heap obj, deleted via vtable[0](this,1) in dtor; memory-reported via IGame[3] GetMemoryStatistics (sub_1839D9030) (ctor=0)
    void*       m_pReleasable140;           // +0x140  polymorphic heap obj, deleted via vtable[0](this,1) in dtor; returned by IGame[32] getter sub_180834590 (ctor=0)
    void*       m_pSingleton148;            // +0x148  ptr to 8-byte {T* head} intrusive registry (alloc sub_1804F75C0(8), head=0); aliased by global qword_18547E6C0; dtor sub_18257D59C releases the head chain via vtable[0](this,1) then frees 0x08
    wh::framework::C_RuntimePrefabManager* m_pRuntimePrefabManager; // +0x150  0x68 bytes (sub_180C10A34)  VERIFIED type
    void*       m_unk158;                    // +0x158  uninitialised in ctor (reserved/padding) /* tentative */

    // std::vector<T> (0x40-byte elements), begin/end/capacity_end; &vector via IGame vtable[39]
    std::vector<void*> m_vec;   // +0x160  begin/end/capacity (0x18); element type unidentified

    wh::game::C_GameModel*  m_pGameModel;   // +0x178  0x2F0 bytes (alloc sub_180C127C0, ctor sub_180C12834 stores &wh::game::C_GameModel::vftable); IGame vtable[35] getter sub_181A77FA0
    wh::game::C_PlatformActivityManager* m_pPlatformActivityManager; // +0x180  8-byte heap obj (sub_180C12760)  VERIFIED type

    uint32_t    m_unk188;                    // +0x188  (unreferenced) /* tentative */
    uint8_t     m_flag18C;                   // +0x18C  (ctor=0)
    uint8_t     _pad18D[3];                  // +0x18D
    uint32_t    m_unk190;                    // +0x190  int32 written by IGame vtable[36] setter sub_181AB4D00 (mov [this+190h], a2); read in shutdown (slot 5) -> sub_1823DD420
    uint32_t    _pad194;                     // +0x194
};
static_assert(sizeof(C_Game) == 0x198);

} // namespace wh::game
