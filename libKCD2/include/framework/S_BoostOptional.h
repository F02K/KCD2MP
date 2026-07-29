#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::S_BoostOptional<T> -- layout mirror of boost::optional<T> (KCD2 ships boost).
// -----------------------------------------------
// boost::optional stores the initialized FLAG FIRST, then the aligned value storage --
// the OPPOSITE member order of MSVC std::optional (value first, flag after), so std::optional
// CANNOT represent these fields and real boost is not on this project's include path; this
// mirror exists solely to type the in-binary layout. Witness (C_Faction+0xD0
// "OverrideMemberNumber"): reader sub_1803F30C4 tests the bool @+0 then loads the value @+8.
// For T = uint64_t: sizeof 0x10 { bool m_initialized @0x00, T m_value @0x08 }.

namespace wh::framework {

template <typename T>
struct S_BoostOptional {
    bool m_initialized;   // +0x00  boost::optional flag (FIRST -- unlike std::optional)
    T    m_value;         // +alignof(T)  value storage (valid only when m_initialized)
};
static_assert(sizeof(S_BoostOptional<uint64_t>) == 0x10, "S_BoostOptional<u64> must be 0x10");

}  // namespace wh::framework
