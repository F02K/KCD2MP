#pragma once
#include "../framework/C_OverrideConflictManager.h"
#include "E_AddRemove.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AreaLabelManager :
// framework::C_OverrideConflictManager<S_AreaLabelHandle, E_AddRemove::Type>
// -- area label override manager SINGLETON at qword_185496128 (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x18 (create inside sub_180CC1A0C: operator new(24) ->
// ctor sub_180CC1AA8; 2nd variant sub_1832C1BB0).  Registers itself with
// C_AreaCallbackDispatch (thunk sub_18070A630).
// -----------------------------------------------
// RTTI TD rva 0x4F51DB0, COL 0x45548D8.  Vtable rva 0x3FE99F0 (3 slots:
// [0] deleting dtor sub_1832C2478, [1]sub_1808728B0, [2]sub_1808728D4 --
// apply-vs-revert assignment [U]).

namespace wh::xgenaimodule {

struct S_AreaLabelHandle;   // layout [U] -- template parameter only

class C_AreaLabelManager
    : public framework::C_OverrideConflictManager<S_AreaLabelHandle, E_AddRemove::Type> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaLabelManager;
    ~C_AreaLabelManager() override;   // [0] sub_1832C2478
    void OcmVf1() override;           // [1] sub_1808728B0 (apply?) [U role]
    void OcmVf2() override;           // [2] sub_1808728D4 (revert?) [U role]
};
static_assert(sizeof(C_AreaLabelManager) == 0x18, "must be 0x18 (new 24 at sub_180CC1A0C)");

}  // namespace wh::xgenaimodule
