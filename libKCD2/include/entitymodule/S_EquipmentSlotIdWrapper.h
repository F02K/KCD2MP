#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::S_EquipmentSlotIdWrapper -- KCD2 WHGame.dll 1.5.6.  sizeof 0x04.
// -----------------------------------------------
// Strong ABI wrapper for a data-driven row id in the equipment_slot database. The C++ object has a
// fixed four-byte layout while the valid ids and names come from game data. RTTR exposes it publicly
// as wh::entitymodule::EquipmentSlotId and its vector as EquipmentSlotIds.
//
// Size is written as 4 by the RTTR type-data factory sub_18195A110; the name/id converters
// sub_182A04D88 and sub_1812C4350 read/write the sole dword. 0xFFFFFFFF is the invalid value.
// m_value is a semantic name; the original member spelling was not recovered.

namespace wh::entitymodule {

struct S_EquipmentSlotIdWrapper {
    std::uint32_t m_value;  // +0x00  equipment_slot database id
};

static_assert(sizeof(S_EquipmentSlotIdWrapper) == 0x04,
    "S_EquipmentSlotIdWrapper must be 0x04");
static_assert(alignof(S_EquipmentSlotIdWrapper) == 0x04,
    "S_EquipmentSlotIdWrapper must be dword-aligned");

}  // namespace wh::entitymodule
