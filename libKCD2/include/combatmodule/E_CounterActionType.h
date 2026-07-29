#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CounterActionType -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum.
// -----------------------------------------------
// Selector of C_CombatActor::DispatchCounterAction (core sub_181483580, RVA 0x1483580). The
// entry switch (0x1814835B0) accepts ONLY the four values below; any other value nulls the out
// pointer and returns. Each value maps to an internal trigger kind (the same constants KCD1
// used: looked up through the director's trigger table, sub_180B277A0) and then to an
// action-type id global (resolved from combat_action_type.tbl; global->name pairing proven by
// the string walk of the registration fn sub_1812B8D48):
//   1 -> kind 4 -> dword_1853204A8 "perfectBlock"      (PerfectBlockFactory @actionMgr+0x60)
//   4 -> kind 6 -> dword_1853204AC "syncPerfectBlock"  (PerfectBlockFactory)
//   5 -> kind 8 -> dword_1853204B4 "masterStrike"      (PerfectBlockFactory)
//   6 -> kind 2 -> block dispatch via the action manager (sub_180C55BB4), gated on the
//                  opponent state's blocking/reach properties
//
// [DELTA vs KCD1] KCD1's domain was 0/3/4/5/6/7 = PerfectBlock/SyncDodge/SyncRiposte/SyncBlock/
// PerfectBlockDodge/SyncPerfectBlock (KCD1 sub_1805633D0). SyncDodge and PerfectBlockDodge are
// GONE (their combat_action_type rows were removed in KCD2); trigger kind 8 now dispatches
// masterStrike (KCD1 dispatched syncRiposte there); the surviving values were renumbered.

namespace wh::combatmodule {

enum class E_CounterActionType : int32_t {
    // 0        -- invalid in KCD2 (KCD1: PerfectBlock)
    PerfectBlock     = 1,   // kind 4 -> "perfectBlock"      (engine wrapper sub_18148340C)
    // 2, 3     -- invalid
    SyncPerfectBlock = 4,   // kind 6 -> "syncPerfectBlock"  (engine wrapper sub_1814832E8)
    MasterStrike     = 5,   // kind 8 -> "masterStrike"      (engine wrapper sub_181483540)
    SyncBlock        = 6,   // kind 2 -> "block"             (engine wrapper sub_181483250)
    // 7+       -- invalid
};

}  // namespace wh::combatmodule
