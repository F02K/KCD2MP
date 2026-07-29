#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::E_AddRemove -- add-vs-remove selector enum (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// Known only as a template argument (nested E_AddRemove::Type):
// C_AreaLabelManager : framework::C_OverrideConflictManager<S_AreaLabelHandle,
// E_AddRemove::Type> and the C_AreaLabelEffect add/remove port
// C_TypedPortRef<E_AddRemove::Type>.  Enumerator names/values NOT recovered
// from the binary [U] -- declared empty; underlying type assumed int32 [U].

namespace wh::xgenaimodule {

struct E_AddRemove {
    enum Type : int32_t {};   // values [U]
};

}  // namespace wh::xgenaimodule
