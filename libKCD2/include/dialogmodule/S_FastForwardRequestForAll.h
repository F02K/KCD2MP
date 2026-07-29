#pragma once
#include <cstdint>
#include <vector>
#include "S_FastForwardRequest.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_FastForwardRequestForAll -- skip the whole conversation
// (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B6EF90; vtable 0x183BCE468 (5 slots: own dtor, [1] override, own trio
// [2..4] -- verify pass); ctor sub_1815EF728; sender sub_182850284. CHEAT: push into
// the dialog message queue to skip an entire conversation programmatically.

namespace wh::dialogmodule::data {

struct S_FastForwardRequestForAll : S_FastForwardRequest {
    inline static constexpr auto RTTI = Offsets::RTTI_S_FastForwardRequestForAll;
    ~S_FastForwardRequestForAll() override;   // [0] own deleting dtor 0x180EB7FC8
    std::vector<uint64_t>* CollectTargetIds(std::vector<uint64_t>* out, void* ctx) override;  // [1] 0x18284EC8C enumerate-all (sub_18134FF14)
    RTTR_ENABLE(S_FastForwardRequest)  // [2..4] own trio: get_type 0x18102EE50, get_derived 0x1815B20B8; base lambda 0x180EC2840
};
static_assert(sizeof(S_FastForwardRequestForAll) == 0x08, "S_FastForwardRequestForAll adds no data");

}  // namespace wh::dialogmodule::data
