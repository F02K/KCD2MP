#pragma once
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_UIData -- payload of C_UICause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: create fn sub_1811A0BD0 tracked-alloc 16; CryStringT copy
// sub_1804FD208(v5+1,a3) @0x1811A0C3C).  Factory caller sub_1811A0A64 (site 0x1811A0AB3).

namespace wh::rpgmodule {

struct S_UIData {
    CryStringT<char> m_name;   // +0x00  UI event/name string [proposed]
};
static_assert(sizeof(S_UIData) == 0x08, "S_UIData must be 0x08");

}  // namespace wh::rpgmodule
