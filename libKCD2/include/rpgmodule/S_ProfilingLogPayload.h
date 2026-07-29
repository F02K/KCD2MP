#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ProfilingLogPayload -- 0x30-byte payload block of C_ProfilingLogEffect
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// NAME INVENTED (no TypeDescriptor; real name unknown).  Layout PROVEN by its copy-helper
// sub_1812C2FE8: {data, count, capacity} dynamic buffer of 16-byte elements (raw allocator
// sub_1804F75C0(16*count)) + 0x18 tail bytes copied verbatim; dtor sub_1804BF0C0 destroys
// elements then frees when capacity != 0.  [element/tail semantics UNVERIFIED]

namespace wh::rpgmodule {

struct S_ProfilingLogPayload {
    // 16-byte element: profiling metric name + EMA-smoothed sample value.
    struct Entry {
        CryStringT<char> name;   // +0x00  metric name ("mainThread","renderThread","waitForGPU",...)
        float            value;  // +0x08  EMA sample (writer sub_1804BEFB8: alpha*sample+(1-alpha)*prev)
        // +0x0C 4-byte padding
    };
    static_assert(sizeof(Entry) == 0x10, "Entry must be 0x10");
    Entry*   m_pData;      // +0x00  {data,count,capacity} buffer of Entry (alloc sub_1804F75C0(16*count))
    uint64_t m_count;      // +0x08
    uint64_t m_capacity;   // +0x10  0 => not owning
    uint64_t m_unk18;      // +0x18  uint64 from engine subsystem accessor (sub_1804BF0FC: *(uint64*)(*(*qword_18492D908+2192))(...)); role UNVERIFIED
    uint64_t m_unk20;      // +0x20  uint64 from engine subsystem accessor (sub_1804BF0FC: *(uint64*)(*(*qword_18492D908+2184))(...)); role UNVERIFIED
    uint32_t m_unk28;      // +0x28  int32 from engine accessor (sub_1804BF0FC: (int)(*(*qword_18492D8C0+1264))(qword_18492D8C0,1)); role UNVERIFIED
    // +0x2C 4-byte tail padding
};
static_assert(sizeof(S_ProfilingLogPayload) == 0x30, "S_ProfilingLogPayload must be 0x30");

}  // namespace wh::rpgmodule
