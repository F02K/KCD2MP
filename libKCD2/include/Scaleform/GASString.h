#pragma once
#include <cstdint>

// -----------------------------------------------
// GASString -- Scaleform GFx AS2 string cell (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI, SDK-correlated name. An 8-byte
// tagged pointer to an interned, refcounted string node: the low 2 bits are
// flag bits and are masked off (& ~3) at every access site.
//
// Node layout (all access sites mask the tag first):
//   +0x08  volatile int32 refcount -- _InterlockedExchangeAdd(node+8, -1) == 1
//          frees through the global GFx heap qword_1850C85C0 vf+0x60
//          (e.g. 0x181CA3243 in the loadMovie loader, 0x181CAF0CD in the drain)
//   +0x0C  inline NUL-terminated UTF-8 text -- the "img://" prefix compare in
//          AddLoadQueueEntry reads byte-by-byte at (node & ~3) + 12 + i
//          (0x181CA39A0); strcmp("imgps://") at 0x181CA3AAB reads +12 directly.
//          Also runtime-verified (2026-07-09 trace build read URLs this way).
//   +0x00 / +0x04 are NOT identified (hash/size by GFx-3 source correlation --
//          UNVERIFIED, kept opaque).
//
// Ctor/assign/release helpers observed: 0x181C23350 (default-construct cell),
// 0x181C23600 (assign), 0x181C23220 (build from AS value), 0x181C24D40
// (non-empty test), 0x181C25150 (substring/index access).

struct GASStringNode {
    size_t           m_size;      // +0x00  byte length (low 63 bits); bit63 = ASCII/char-len-verified cache flag (ctor 0x181C23278, GetLength 0x181C24D40)
    volatile int32_t m_refCount;  // +0x08  VERIFIED
    char             m_data[4];   // +0x0C  inline NUL-terminated text (VERIFIED; extends past the struct)
};

class GASString {
public:
    uintptr_t m_bits;   // GASStringNode* | 2 low tag bits

    GASStringNode* GetNode() const { return reinterpret_cast<GASStringNode*>(m_bits & ~uintptr_t(3)); }
    const char*    c_str() const   { return GetNode()->m_data; }
};
static_assert(sizeof(GASString) == 0x8, "GASString is one tagged pointer");
