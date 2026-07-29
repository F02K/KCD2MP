#pragma once
#include <cstdint>
#include "E_MinigameType.h"

// -----------------------------------------------
// wh::playermodule::I_Minigame -- minigame-session interface (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08 (vptr only).
// -----------------------------------------------
// Primary interface of the per-player minigame sessions stored in the player action subsystem
// (owner+0x20 hash: userId -> list<I_Minigame*>).  C_Alchemy implements it as the head of its
// primary vtable chain (0x183F60B08).  Slot knowledge is PARTIAL -- only the slots exercised by
// the generic session plumbing and the alchemy session are named; everything else is _vfN filler.
// DO NOT call through unnamed slots.  Total slot count UNVERIFIED (>= 36; fillers stop at [35]).
//
// Evidence: FindOrCreateAction sub_182024240 matches sessions by vf[0]()==type and consults
// vf[7](); C_Alchemy::End = primary slot [5] (sub_1809F1A8C, vtable+0x28); the start path invokes
// +0x108 self-destruct on failure, +0x110 push-action-map-context (CryString), +0x118 the
// minigame-exit hint toggler (sub_1809F1E1C).

namespace wh::playermodule {

class I_Minigame {
public:
    virtual E_MinigameType::Type GetMinigameType() const = 0;   // [0]  matched by FindOrCreateAction (type 3 = Alchemy)
    virtual void _vf1() = 0;                                    // [1]
    virtual void _vf2() = 0;                                    // [2]
    virtual void _vf3() = 0;                                    // [3]
    virtual void _vf4() = 0;                                    // [4]
    virtual void End() = 0;                                     // [5]  session teardown (C_Alchemy: sub_1809F1A8C -- unregisters "alchemy" filters, audio/entity cleanup)
    virtual void _vf6() = 0;                                    // [6]
    virtual bool _vf7() = 0;                                    // [7]  liveness/completion query consulted by FindOrCreateAction (matchExtra || !vf7())
    virtual void _vf8() = 0;                                    // [8]
    virtual void _vf9() = 0;                                    // [9]
    virtual void _vf10() = 0;                                   // [10]
    virtual void _vf11() = 0;                                   // [11]
    virtual void _vf12() = 0;                                   // [12]
    virtual void _vf13() = 0;                                   // [13]
    virtual void _vf14() = 0;                                   // [14]
    virtual void _vf15() = 0;                                   // [15]
    virtual void _vf16() = 0;                                   // [16]
    virtual void _vf17() = 0;                                   // [17]
    virtual void _vf18() = 0;                                   // [18]
    virtual void _vf19() = 0;                                   // [19]
    virtual void _vf20() = 0;                                   // [20]
    virtual void Reset() = 0;                                   // [21] in-session reset.  C_Alchemy impl
                                                                //      sub_1806C4F1C: full table reset
                                                                //      (content visuals/buckets/props/state/
                                                                //      fire/verbs, kind-entity ids KEPT);
                                                                //      run by the finish cbs AFTER grading
    virtual void _vf22() = 0;                                   // [22]
    virtual void _vf23() = 0;                                   // [23]
    virtual void _vf24() = 0;                                   // [24]
    virtual void _vf25() = 0;                                   // [25]
    virtual void _vf26() = 0;                                   // [26]
    virtual void _vf27() = 0;                                   // [27]
    virtual void _vf28() = 0;                                   // [28]
    virtual void _vf29() = 0;                                   // [29]
    virtual void _vf30() = 0;                                   // [30]
    virtual void _vf31() = 0;                                   // [31]
    virtual void _vf32() = 0;                                   // [32]
    virtual void SelfDestruct() = 0;                            // [33] +0x108  invoked when Start fails (session discards itself)
    virtual void PushActionMapContext(void* cryStrContext) = 0; // [34] +0x110  Start pushes CryString("alchemy") through this
    virtual void ToggleExitHint(bool enable) = 0;               // [35] +0x118  sub_1809F1E1C -- "minigame_exit" action/hint
};
static_assert(sizeof(I_Minigame) == 8, "I_Minigame is a vptr-only interface");

}  // namespace wh::playermodule
