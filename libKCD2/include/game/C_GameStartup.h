#pragma once
#include <cstdint>
#include "../Offsets/vtables/IGameStartup.h"

// -----------------------------------------------
// C_GameStartup - Warhorse game bootstrap / DLL entry object
// -----------------------------------------------
// RTTI:  .?AVC_GameStartup@game@wh@@
// Source: code\game\whgame\GameCore\GameStartup.cpp
// Target: WHGame.dll 1.5.6, image base 0x180000000 (IDA "kd7u")
//
// Factory export : CreateGameStartup 0x1819db050  (returns &the singleton)
// Deleting dtor  : 0x1839dd11a  (frees 0x10; body 0x1839dd110)
// primary  vtable: 0x183d18750  (IGameStartup,          12 slots)
// secondary vtable: 0x183d18730 (IWindowMessageHandler,  3 slots)  at +0x08
//
// SHAPE: this is a GLOBAL SINGLETON (its storage is the .bss block at 0x1856647C8,
// returned by CreateGameStartup). It has NO data members -- CreateGameStartup only
// writes the two vtable pointers. All engine state lives in file-scope globals, not
// in the object:
//   g_pGame      @ 0x18549D330  (IGame*)          g_pFramework @ 0x18549D328 (IGameFramework*)
//   g_pModule    @ 0x185168B38  (400-byte holder) hLibModule   (game DLL handle)
//
// SIZE: operator delete in the scalar-deleting dtor (0x1839dd380) passes N = 0x10,
// i.e. exactly the two inherited vtable pointers. VERIFIED sizeof == 0x10.

namespace wh::game {

class C_GameStartup : public Offsets::IGameStartup,          // +0x00  primary vtable   0x183d18750
                      public Offsets::IWindowMessageHandler   // +0x08  secondary vtable 0x183d18730
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GameStartup;
    // ---- C_GameStartup's own virtuals (extend the primary/IGameStartup vtable) ----
    virtual bool     _vf9(int* pOut) = 0;   // [9]  0x181ab13b0  *pOut = 0; return false                                 /* tentative */
    virtual intptr_t _vf10() = 0;           // [10] 0x1839e3f1c  forwards to IGame(g_pGame)::vftable[36] (+0x120)         /* tentative */
    virtual IGameRef Reset() = 0;           // [11] 0x180da3164  destroys g_pGame, C_Game::CreateInstance, returns IGameRef /* tentative */

    // No data members: state is held in file-scope globals (see header banner).
};
static_assert(sizeof(C_GameStartup) == 0x10);  // two vtable pointers; operator delete size = 0x10

}  // namespace wh::game
