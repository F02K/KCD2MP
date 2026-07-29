#pragma once
#include <cstdint>
#include <vector>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_FastForwardRequest -- dialog fast-forward (SKIP) message
// (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B6F270; vtable 0x183E70390 (5 slots); ctor sub_18284C258. Polymorphic
// event object pushed into the dialog message queue: skips the CURRENT line. Senders
// live in the 0x18285026x..0x182855510 cluster. Derived: S_FastForwardRequestForAll
// (whole conversation), S_FastForwardRequestForRole (one role).
// CHEAT: the programmatic dialog-skip hook, complementing the wh_dlg_AutoSkip /
// wh_dlg_SkipCooldown ICVars cached on C_DialogInstance +0x370/+0x378.

namespace wh::dialogmodule::data {

struct S_FastForwardRequest {
    inline static constexpr auto RTTI = Offsets::RTTI_S_FastForwardRequest;
    virtual ~S_FastForwardRequest();   // [0]
    // [1] 0x1805B1B70 base: zero-inits the 24B sret vector (empty); overridden by ForAll
    // (enumerate) / ForRole (push one 8B id). sret ABI PROVEN (verify); name+elem INFERRED.
    virtual std::vector<uint64_t>* CollectTargetIds(std::vector<uint64_t>* out, void* ctx);
    RTTR_ENABLE()  // [2..4]: get_type 0x180EC27E0, get_derived 0x18284FD5C (ex-"ClonePayload"/"Clone" guesses)
};
static_assert(sizeof(S_FastForwardRequest) == 0x08, "S_FastForwardRequest is vptr-only");

}  // namespace wh::dialogmodule::data
