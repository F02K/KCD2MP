#pragma once
#include <cstdint>
#include "IWindowMessageHandler.h"   // Offsets::IWindowMessageHandler (C_GameStartup secondary base @ +0x08)

// -----------------------------------------------
// IGameStartup / IWindowMessageHandler - KCD2 binary vtable order
// -----------------------------------------------
// Target: WHGame.dll 1.5.6, image base 0x180000000 (IDA "kd7u")
// Source: code\game\whgame\GameCore\GameStartup.cpp
//
// These are the two lean interface bases of wh::game::C_GameStartup.
//   IGameStartup          primary   vtable 0x183d18750 (12 slots, incl. 3 class extensions)
//   IWindowMessageHandler secondary vtable 0x183d18730 (3 slots)
//
// Slot identities VERIFIED by decompiling each vfunc on kd7u. Init is anchored by the
// literal framework-listener name "C_GameStartup" and GameStartup.cpp:277; GetRestartMod
// by its strncpy_s; GetPatch by its xor eax,eax (returns null). Names on [1] and [9..11]
// are inferred from behaviour and marked /* tentative */.

struct SSystemInitParams;   // CryEngine startup params (~4.7 KB); passed by ref to Init
struct IGame;               // CryEngine game interface
typedef IGame** IGameRef;   // CryEngine IGameRef == IGame** (returned by value from Init)
struct tagMSG;              // Win32 MSG (hwnd@0, message@8, wParam@0x10, lParam@0x18)

namespace Offsets {

// Primary base (+0x00). 9 stock slots [0..8]; C_GameStartup appends [9..11].
struct IGameStartup {
    virtual void Dtor(char flags) = 0;                                   // [0]  0x1839dd380  scalar-deleting dtor (operator delete size 0x10)
    virtual bool CheckCompatibility() = 0;                               // [1]  0x18194d9c8  scans Libs/Compatibility.xml, warns on conflicting apps  /* tentative name */
    virtual IGameRef Init(SSystemInitParams& params) = 0;                // [2]  0x180da2cd0  -> sub_180DA2D18: creates game, registers "C_GameStartup"  VERIFIED
    virtual void Shutdown() = 0;                                         // [3]  0x1839e4058  cleanup then self-dtor(flags=0)                            VERIFIED
    virtual int  Update(bool haveFocus, unsigned int updateFlags) = 0;   // [4]  0x180862e08  focus/r_Fullscreen events -> IGame::Update               VERIFIED
    virtual bool GetRestartLevel(char** ppLevelName) = 0;                // [5]  0x1839e12d8  writes *ppLevelName, returns bool                          VERIFIED
    virtual const char* GetPatch() const = 0;                            // [6]  0x18066cd10  xor eax,eax (returns nullptr)                             VERIFIED
    virtual bool GetRestartMod(char* pModNameOut, int nameLenMax) = 0;   // [7]  0x1839e1354  strncpy_s(pModNameOut, qword_18547BB90, nameLenMax)       VERIFIED
    virtual int  Run(const char* pAutoStartLevelName) = 0;               // [8]  0x18086413c  main run/message loop (848 bytes)                         VERIFIED
};

// C_GameStartup's secondary base (+0x08) is Offsets::IWindowMessageHandler,
// defined canonically in Offsets/vtables/IWindowMessageHandler.h (included above).

}  // namespace Offsets
