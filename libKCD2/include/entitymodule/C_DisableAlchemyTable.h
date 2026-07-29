#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::C_DisableAlchemyTable -- concept/flow node disabling alchemy tables
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_DisableAlchemyTable@entitymodule@wh@@ (TD 0x184BDED30); vtable 0x183C2D3A0
// (14 slots: [4] Activate = sub_180E544B4 run-once guard (node+0x0E bit0) -> sub_1804F4BB0;
// [5]/[7] port helpers; [6] GetSelf; [8]/[9] null getters; [11] smart-ptr release).  Holds its
// target tables as a reflected C_TypedArrayPortRef<std::vector<C_AlchemyTable*>> input port.
// TOGGLE PATH RESOLVED (table_enable_and_verbs.md §1): Activate -> sub_1804F4BB0 = generic RTTR
// reflected iterate over the AlchemyTables port, per-table property_wrapper::set writing
// C_AlchemyTable::m_disabled (+0x40; 0 = enabled).
// [Own size/field layout beyond the node header UNVERIFIED; no sizeof static_assert.]

namespace wh::entitymodule {

class C_DisableAlchemyTable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DisableAlchemyTable;
    virtual ~C_DisableAlchemyTable() = default;   // [0] dtor sub_1819B0228; see banner for the slot map
};

}  // namespace wh::entitymodule
