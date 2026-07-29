#pragma once
#include <cstddef>
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::C_QAMSlot -- KCD2 WHGame.dll 1.5.6.  sizeof 0x30.
// -----------------------------------------------
// Constructor sub_1816A86C0 zeroes all fields. Weapon QAM accessors use m_mainItem/m_offItem as one
// pair; food QAM accessors use only m_mainItem. No inspected read, write, clear, validation, or
// destruction path identified the trailing two 16-byte blocks, so they remain explicitly opaque.

namespace wh::entitymodule {
class C_Item;
}

namespace wh::playermodule {

class C_QAMSlot {
public:
    entitymodule::C_Item* m_mainItem;    // +0x00
    entitymodule::C_Item* m_offItem;     // +0x08
    std::uint8_t m_unknown10[0x10];      // +0x10  semantics unrecovered
    std::uint8_t m_unknown20[0x10];      // +0x20  semantics unrecovered
};

static_assert(sizeof(C_QAMSlot) == 0x30, "C_QAMSlot must be 0x30");
static_assert(offsetof(C_QAMSlot, m_mainItem) == 0x00, "main item must be at +0x00");
static_assert(offsetof(C_QAMSlot, m_offItem) == 0x08, "off item must be at +0x08");

}  // namespace wh::playermodule
