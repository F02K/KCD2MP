#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::framework::C_CryLoadGameHelper -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40.
// -----------------------------------------------
// CryEngine-side LOAD deserialization helper (vtable wh::framework::C_CryLoadGameHelper;
// ctor sub_1807940BC; alloc 64 in the C_SaveGameManager ctor). Owned by C_SaveGameManager
// @+0x178 and mirrored to the global qword_18547BD40; consumed by LoadCryEngineData
// (sub_1825BD8D8 -> sub_180793894 / sub_1807937F8). +0x10..+0x3F is io/stream state, zeroed
// at ctor -- fields UNRESOLVED.

namespace wh::framework {

class C_CryLoadGameHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CryLoadGameHelper;
    virtual ~C_CryLoadGameHelper() = default;   // +0x00  vptr (slot order UNVERIFIED)
    CryStringT<char> m_str08;     // +0x08  (source name/path -- INFERRED)
    uint64_t m_state10[6];        // +0x10..+0x40  io/stream state (ctor-zeroed; UNRESOLVED)
};
static_assert(sizeof(C_CryLoadGameHelper) == 0x40, "C_CryLoadGameHelper must be 0x40");

}  // namespace wh::framework
