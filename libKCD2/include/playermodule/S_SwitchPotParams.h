#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::S_SwitchPotParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Pot-halter action-param block (RTTI template arg US_SwitchPotParams@playermodule; POD).  Lives
// at action+0x50 of C_SwitchPotHalterAction (the parallel-channel slot-4 action that raises/
// lowers the cauldron chain).  VERIFIED via the enqueue helper sub_1815FC5F0: after construction
// it assigns CryStringT at action+0x50 and copies the bool at action+0x58 from the request
// {CryString name; bool flag}.  ctor sub_1815FC738 default-inits (empty string, flag 0).
// Because this param is 0x10, the C_AlchemyActionBase tail shifts: m_pAlchemy @+0x60,
// m_pManager @+0x68 (leaf sizeof 0x80).

namespace wh::playermodule {

struct S_SwitchPotParams {
    CryStringT<char> m_potName;   // +0x00  target pot/halter helper name
    bool             m_raise;     // +0x08  halter direction flag [up/down encoding UNVERIFIED]
    uint8_t          _pad9[7];    // +0x09
};
static_assert(sizeof(S_SwitchPotParams) == 0x10, "S_SwitchPotParams is {CryString, bool}");

}  // namespace wh::playermodule
