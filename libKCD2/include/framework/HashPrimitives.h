#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// wh::shared -- hash primitives for the game's std::unordered_map / std::unordered_set.
// -----------------------------------------------
// The Warhorse hash containers ARE stock MSVC std::unordered_map / std::unordered_set (the 0x40
// _Hash header: float _Max_load_factor @0, std::list{head,size} @8, bucket std::vector @0x18,
// size_t _Mask @0x30, size_t _Maxidx @0x38). There are NO custom replica containers -- callers use
// the real std:: types directly. What lives here are only the hash FUNCTIONS the engine keys hash
// with, so a struct key (WUID, CryGUID) can be given a matching transparent hasher:
//
//   FNV-1a 64  -- MSVC std::hash's algorithm for trivially-copyable keys; the explicit hasher for
//                 16-byte GUID / 8-byte WUID keys. Seen in C_InventoryManager::GetValueForWuid
//                 (sub_18047A85C hashes the 16-byte key with seed 0xCBF29CE484222325).
//   FNV-1  32  -- for null-terminated strings (info labels, names).
//   hash_combine (boost pattern) -- folds field hashes for composite keys.

namespace wh::shared {

// FNV-1a 64-bit over `len` raw bytes.
inline uint64_t fnv1a(const void* data, size_t len) {
    uint64_t h = 0xCBF29CE484222325ULL;
    auto p = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < len; ++i)
        h = (h ^ p[i]) * 0x100000001B3ULL;
    return h;
}

// FNV-1 32-bit over a null-terminated string.
inline uint32_t fnv1_32(const char* str) {
    uint32_t h = 0x811C9DC5u;
    while (*str)
        h = (h * 0x01000193u) ^ static_cast<uint8_t>(*str++);
    return h;
}

// boost::hash_combine.
inline uint64_t hash_combine(uint64_t seed, uint64_t value) {
    seed ^= value + 0x9E3779B9ULL + (seed << 6) + (seed >> 2);
    return seed;
}

// Default hasher (FNV-1a over sizeof(K) raw bytes) for struct keys whose std::hash the project does
// not specialize (WUID, CryGUID). Pass as the Hash arg:
//   std::unordered_map<WUID, V, wh::shared::S_DefaultHash<WUID>>
// Matches MSVC std::hash for trivially-copyable K.
template<typename K>
struct S_DefaultHash {
    uint64_t operator()(const K& key) const { return fnv1a(&key, sizeof(K)); }
};

}  // namespace wh::shared
