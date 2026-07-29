#pragma once
#include "C_Cause.h"
#include "S_GenericReputationChangeData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GenericReputationChangeCause -- catch-all reputation-change cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4DE80  vtable 0x183A523A0.  sizeof 0x10 (PROVEN: factory sub_180784D40
// operator new(16)).  GetName [3] = "GenericReputationChange" (TLS-once static 0x1817C0A24);
// Accept [2] -> handler vtbl[37]; [1] = nullsub.  The factory immediately dispatches the new
// cause via sub_1809DB5A4 and deletes it if still owned.

namespace wh::rpgmodule {

class C_GenericReputationChangeCause : public C_Cause<S_GenericReputationChangeData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GenericReputationChangeCause;
};
static_assert(sizeof(C_GenericReputationChangeCause) == 0x10,
              "C_GenericReputationChangeCause must be 0x10 (operator new(16))");

}  // namespace wh::rpgmodule
