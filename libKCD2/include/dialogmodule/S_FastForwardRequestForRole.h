#pragma once
#include <cstdint>
#include <vector>
#include "S_FastForwardRequest.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_FastForwardRequestForRole -- skip one role's lines
// (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI TD 0x184B6F1E0; vtable 0x183B8ED08 (5 slots: own dtor, [1] override, own trio
// [2..4] -- verify pass); ctor sub_18146A438; sender sub_1828502A0. Adds the target
// role field at +0x08 (width/encoding UNVERIFIED -- modeled as a qword).

namespace wh::dialogmodule::data {

struct S_FastForwardRequestForRole : S_FastForwardRequest {
    inline static constexpr auto RTTI = Offsets::RTTI_S_FastForwardRequestForRole;
    ~S_FastForwardRequestForRole() override;   // [0] own deleting dtor 0x180515DE0
    std::vector<uint64_t>* CollectTargetIds(std::vector<uint64_t>* out, void* ctx) override;  // [1] 0x18284ECD0 pushes one id resolved from m_role
    RTTR_ENABLE(S_FastForwardRequest)  // [2..4] own trio: get_type 0x18102EEA8, get_derived 0x1815B2098; base lambda 0x180EC2840
    uint64_t m_role;   // +0x08  target role [width/encoding UNVERIFIED]
};
static_assert(sizeof(S_FastForwardRequestForRole) == 0x10, "S_FastForwardRequestForRole must be 0x10");

}  // namespace wh::dialogmodule::data
