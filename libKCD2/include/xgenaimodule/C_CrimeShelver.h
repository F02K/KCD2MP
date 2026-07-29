#pragma once
#include "../playermodule/I_Shelver.h"

// -----------------------------------------------
// wh::xgenaimodule::C_CrimeShelver : playermodule::I_Shelver -- shelves/restores
// crime-system state around player-mode transitions (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08 (create-site proven: alloc 8 in the lazy-init getter
// sub_1819A3BF0 -> global qword_185496218; vtable-only, no data members).
// -----------------------------------------------
// RTTI TD rva 0x4FE8BA8; COL rva 0x45BB668 [PRIMARY] cdOffset=0; vtable
// 0x184770758, 4 slots.  Sole direct base playermodule::I_Shelver @+0x0.
// [2] is nullsub_1 (sub_1803B6E80).  Slot roles UNRESOLVED (shelve/unshelve
// pair presumed among [0]/[1]); dtor placement unverified -- mirrored as plain
// slots to match the interface (layout identical).

namespace wh::xgenaimodule {

class C_CrimeShelver : public playermodule::I_Shelver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CrimeShelver;
    void _vf0() override;   // [0] sub_1832A5EA0 [U role]
    void _vf1() override;   // [1] sub_1832A6554 [U role]
    void _vf2() override;   // [2] sub_1803B6E80 = nullsub_1
    void _vf3() override;   // [3] sub_181AA5660 [U role]
};
static_assert(sizeof(C_CrimeShelver) == 0x08, "C_CrimeShelver must be 0x08 (alloc 8, vtable only)");

}  // namespace wh::xgenaimodule
