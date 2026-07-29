#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyMode -- the brew-state mode/phase byte (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the byte at C_Alchemy+0x300 (S_AlchemyBrewState::m_mode; setter
// sub_1806C3094, reset write 0x1816E9C62).  Gates the whole interactive layer: PerformVerb
// (0x1815FC494), the per-frame drain (0x180737920) and the input delegates all require Brewing.
// Values proven by writers/readers:
//   0 ctor/reset default · 2 set by session start finalize (finish_grading_site.md, 0x1806C5164
//   path) · 3 GRINDING: C_UseMortarAction's start callback writes it through sub_1806C3094
//   (call at 0x1806C307F); the action's OnStop (slot 32, sub_180A97A44) restores mode 2 ·
//   4 READING: written by the open-book montage-done callback 0x181833484 (anim-event match ->
//   mode=4, then enable-all-read-rows sub_1809F2380(this,1)) -- the exact instant the vanilla
//   read_* prompts light up · 5 distilling (dirty-flag path sub_182E1719C) · 6 drain runs the
//   in-session table reset (sub_180737920 -> sub_1815FC18C) · 7 written by that reset (with
//   slot-1 drain-off) · 8 drain early-outs entirely (observed 0x180737996) [broader role
//   UNVERIFIED].
// Value 1 is an unobserved gap.

namespace wh::playermodule {

struct E_AlchemyMode {
    enum Type : uint8_t {
        Idle           = 0,   // ctor/reset default -- session not interactive yet
        Brewing        = 2,   // live interactive brewing; the PerformVerb/input/drain gate
        Grinding       = 3,   // temporary C_UseMortarAction mode; OnStop restores Brewing
        Reading        = 4,   // at the book, open montage done -- read_* rows are live
        Distilling     = 5,   // C_UseRetortAction's own state machine (writer 0x182E17206): N
                              // "alchemy_idle_bellow_distillation" loops, then phase 3 runs
                              // OnFinishRetortPotion (grade + Reset) -- distilled recipes FINISH
                              // here; mode returns to Brewing via the Reset
        RestartPending = 6,   // next drain performs the in-session table reset
        Resetting      = 7,   // set by the table reset sub_1815FC18C
        DrainSuspended = 8,   // per-frame drain skips verb+director entirely [role UNVERIFIED]
    };
};

}  // namespace wh::playermodule
