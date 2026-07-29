#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_GenericReputationChangeData -- payload of C_GenericReputationChangeCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Referenced by TD .?AV?$C_Cause@US_GenericReputationChangeData@... (0x184D4DED0).  The only
// cause factory (sub_180784D40, operator new(16)) never writes past the vfptr, so the payload
// is at most 8 bytes and carries no initialized state.  Modeled EMPTY [UNVERIFIED: an
// 8-byte never-initialized POD would produce the same allocation size].

namespace wh::rpgmodule {

struct S_GenericReputationChangeData {
};

}  // namespace wh::rpgmodule
