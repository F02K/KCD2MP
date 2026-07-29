#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "../framework/I_Action.h"

// -----------------------------------------------
// wh::playermodule::I_AlchemyAction -- alchemy-action interface (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x10 (vptr + I_Action refcount).
// -----------------------------------------------
// RTTI .?AVI_AlchemyAction@playermodule@wh@@ (TD 0x184DD4C68).  Primary base of every alchemy
// minigame action (C_Action<I_AlchemyAction> vtable 0x183A67978, 39 slots).
//
// Adds EXACTLY 4 virtual slots [26..29] after I_Action's [0..25] -- proven by a 3-way vtable diff
// (C_Action<I_AlchemyAction> 0x183A67978 vs C_Action<I_Action> 0x18473F028 vs the
// C_UseBellowsAction leaf 0x183F60260): slots [1..25] are byte-identical, divergence starts at
// [26], and the 9-slot C_Action-own block follows shifted +4 ([30..38]).
//
// Slot roles (leaf evidence):
//   [26] OnAnimEvent(int,int,float)  pure; bellows sub_18173D0EC forwards (kind,param,time) to its
//        Mannequin listener sub_1806C37BC.  Every anim-listening leaf overrides it.
//   [27] GetActionNameString(CryStringT&) base 0x1808D2480 (builds from the empty-name byte pool
//        byte_183A3D1E0); consumed by the UI verb-hint query sub_1808D1ECC via vtbl+0xD8.
//        DE-OVERLOADED name: I_Action already declares GetActionName()->const char* at [18]
//        (adjacent same-name overloads get slot-reversed by MSVC -- distinct names avoid that).
//   [28] OnBackAction -- the "back" input handler.  The alchemy_reading "read_back" delegate
//        (registered by AlchemyReadingCtrl::RegisterActions 0x180A94FCC, functor _Do_call
//        0x1807DA360) invokes it on the LIVE C_OpenBookAction: its impl (0x182E18418 ->
//        0x180A95F74, misnamed "AutocookFinalize" in the idb -- it is the shared EXIT-READING
//        path autocook also uses) builds the class-5 close-book montage action from the
//        mannequin fragment cache, installs the completion cb (sub_180A96308/sub_1806C2898)
//        and pushes it.  Bellows impl returns true trivially [other impls unverified].
//   [29] GetCompatFlags()->int32, pure; bellows returns 0.  ANDed with C_Alchemy+0x668 by the
//        dispatch validator sub_1808D23E0 (vtbl+0xE8).
//
// The DIRECTOR-facing contract rides on I_Action: IsStarted[4] (+0x20) is the tick's running gate,
// GetActionSequenceId[16] (+0x80, reads +0x3C) is the DIRECTOR SLOT (alchemy base ctor writes 1;
// C_SwitchPotHalterAction writes 4 = parallel channel), and AsActionImpl[22] (+0xB0) hands the
// I_ActionImpl subobject the Director drives Start/Update/Tick/Stop through.

namespace wh::playermodule {

class I_AlchemyAction : public wh::framework::I_Action {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AlchemyAction;

    // Mannequin anim-event hook: (eventKind, param, time) -- leaf compares against its stored CRC32 event hashes.
    virtual void    OnAnimEvent(int32_t eventKind, int32_t param, float time) = 0;  // [26] +0xD0
    // UI verb name (CryString out) -- base 0x1808D2480 returns the empty/default name.
    virtual void    GetActionNameString(CryStringT<char>& out) const = 0;           // [27] +0xD8
    // "Back" input handler (read_back delegate target; see banner) -- returns handled.
    virtual bool    OnBackAction() = 0;                                             // [28] +0xE0
    // Compatibility mask, ANDed with C_Alchemy::m_compatMask (+0x668) before dispatch.
    virtual int32_t GetCompatFlags() const = 0;                                     // [29] +0xE8
};
static_assert(sizeof(I_AlchemyAction) == 0x10, "I_AlchemyAction = vptr + refcount");

}  // namespace wh::playermodule
