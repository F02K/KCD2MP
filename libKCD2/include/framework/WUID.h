#pragma once
#include <cstdint>
#include <functional>

// -----------------------------------------------
// wh::framework::WUID -- Warhorse unique id (KCD2 WHGame.dll 1.5.6, kd7u).  8 bytes.
// -----------------------------------------------
// A tagged 64-bit handle: (slot | gen<<18 | tag<<56). The high byte is the object-kind tag
// (e.g. 0x02 == Item, 0x03 == Inventory, 0x05 == Soul). A default/invalid WUID is 0. Used
// pervasively as the registry key for bindable objects and as the element/key of the game's
// std::unordered_set<WUID> / std::unordered_map<WUID,V> (which hash it with std::hash<WUID>).

namespace wh::framework {

struct WUID {
    uint64_t m_value;   // (slot | gen<<18 | tag<<56)

    constexpr uint32_t slot()       const { return (uint32_t)(m_value & 0x1FFFF); }
    constexpr uint16_t generation() const { return (uint16_t)(m_value >> 0x11); }
    constexpr uint8_t  tag()        const { return (uint8_t)(m_value >> 56); }

    constexpr bool operator==(const WUID& o) const { return m_value == o.m_value; }
    constexpr bool operator!=(const WUID& o) const { return m_value != o.m_value; }
    constexpr bool operator<(const WUID& o) const { return m_value < o.m_value; }   // std::map/std::set ordering
};
static_assert(sizeof(WUID) == 8);

}  // namespace wh::framework

// The binary's std::unordered_set<WUID> / std::unordered_map<WUID,V> use std::hash<WUID> (FNV-1a of
// the 8-byte handle, matching MSVC std::hash for a trivially-copyable key) and std::equal_to<WUID>.
template<>
struct std::hash<wh::framework::WUID> {
    size_t operator()(const wh::framework::WUID& w) const noexcept {
        uint64_t h = 0xCBF29CE484222325ULL;
        for (int i = 0; i < 8; ++i) h = (h ^ ((w.m_value >> (i * 8)) & 0xFF)) * 0x100000001B3ULL;
        return (size_t)h;
    }
};
