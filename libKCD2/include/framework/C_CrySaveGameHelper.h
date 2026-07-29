#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::framework::C_CrySaveGameHelper -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// CryEngine-side SAVE serialization helper (vtable wh::framework::C_CrySaveGameHelper;
// ctor sub_180794078; alloc 56 in the C_SaveGameManager ctor). Owned by C_SaveGameManager
// @+0x170 and mirrored to the global qword_18547BD38. The +0x18..+0x37 region is io/stream
// state, zeroed at ctor and consumed by the CryEngine-data write path -- fields UNRESOLVED.

namespace wh::framework {

class C_CrySaveGameHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CrySaveGameHelper;
    virtual ~C_CrySaveGameHelper() = default;   // +0x00  vptr (slot order UNVERIFIED)
    int32_t  m_int08;             // +0x08  ctor init 3 (role unresolved)
    uint32_t _pad0C;              // +0x0C
    CryStringT<char> m_str10;     // +0x10  (target name/path -- INFERRED)
    uint64_t m_state18[4];        // +0x18..+0x38  io/stream state (ctor-zeroed; UNRESOLVED)
};
static_assert(sizeof(C_CrySaveGameHelper) == 0x38, "C_CrySaveGameHelper must be 0x38");

}  // namespace wh::framework
