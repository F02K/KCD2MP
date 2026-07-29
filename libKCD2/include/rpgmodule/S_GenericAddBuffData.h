#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_GenericAddBuffData -- payload of C_GenericAddBuffCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_181361E2C tracked-alloc 32; same {qword, _OWORD} shape as
// S_GenericActivateBuffData).  8 create-sites; primary sub_181FD1A50 builds
// { *functor @+0 (buff GUID), xmmword_185583410 @+8 (16 zero bytes, verified) }.

namespace wh::rpgmodule {

struct S_GenericAddBuffData {
    uint64_t m_buffGuid;    // +0x00  buff GUID (= *functor result) [proposed]
    uint64_t m_params[2];   // +0x08  buff-apply params _OWORD (zero-inited at primary site) [U split]
};
static_assert(sizeof(S_GenericAddBuffData) == 0x18, "S_GenericAddBuffData must be 0x18");

}  // namespace wh::rpgmodule
