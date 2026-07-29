#pragma once
#include "I_Minigame.h"
#include "I_MinigameCamera.h"

// -----------------------------------------------
// wh::playermodule::C_Minigame -- shared minigame-session base (KCD2 WHGame.dll 1.5.6, kd7u).
// Modeled sizeof 0x10 (the two interface vptrs; own-data extent UNVERIFIED).
// -----------------------------------------------
// RTTI base chain of C_Alchemy (??_R2C_Alchemy walk): C_Alchemy : C_Minigame : {I_Minigame,
// I_MinigameCamera}.  Shared by the dice/blacksmith sessions -- the C_Minigame-family helper
// functions (e.g. the generic scorer sub_182E94138) operate on the DERIVED objects, and the dice
// session's fields at +0x50/+0xD8/+0x1C0/+0x468 were previously misattributed to C_Alchemy
// because of this shared ancestry (see solver_skill_model.md §0.0).
//
// [UNVERIFIED SPLIT] Whether C_Minigame owns data beyond the two vptrs could not be established
// (its ctor is inlined into each session ctor).  All post-vptr fields are therefore modeled on the
// derived classes (C_Alchemy models +0x18.. itself); if a later wave proves C_Minigame-owned
// members, move them here.

namespace wh::playermodule {

class C_Minigame : public I_Minigame, public I_MinigameCamera {
public:
    // no data modeled (see banner)
};
static_assert(sizeof(C_Minigame) == 0x10, "C_Minigame modeled as the two interface vptrs");

}  // namespace wh::playermodule
