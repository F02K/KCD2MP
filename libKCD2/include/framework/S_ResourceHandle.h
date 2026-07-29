#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::S_ResourceHandle -- 0x10 unbound-resource-handle sentinel (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Recurring ctor pattern across quest/environment classes:
//   *(base) = &unk_XXXXXXXX; *(base+8) = (uint16)-1; *(base+0xA) = (uint16)-1;
// where &unk_... points at a per-site static all-0xFF blob and the two -1 words are an
// index/generation pair (some sites write one dword -1 -- same bytes). Sites: C_StreamingHelper
// +0x08/+0x18, C_Quest +0xA0, C_Objective +0x48, C_QuestManager +0x08..+0x38 (x4),
// C_ActivityCollection +0xA0/+0xB0.
// NAME IS SYNTHETIC; the exact type is UNVERIFIED (asset-ref / WUID-style registry handle /
// model-bound property are the candidates). Only the 0x10 footprint and the ctor writes are
// certified -- treat as opaque until a consumer is RE'd.

namespace wh::framework {

struct S_ResourceHandle {
    DynArray<std::array<uint8_t, 16>> m_data;  // +0x00  WHDynStorage array (8B single ptr); empty=&unk_ game sentinel; elem stride 0x10, element type UNRESOLVED
    uint16_t    m_index;        // +0x08  ctor -1
    uint16_t    m_generation;   // +0x0A  ctor -1
    uint32_t    _pad0C;         // +0x0C  not written
};
static_assert(sizeof(S_ResourceHandle) == 0x10, "S_ResourceHandle must be 0x10");

}  // namespace wh::framework
