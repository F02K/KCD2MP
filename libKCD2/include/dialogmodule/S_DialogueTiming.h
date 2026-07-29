#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::S_DialogueTiming -- "dialogue_animation" generated-table row
// (KCD2 1.5.6, kd7u).  sizeof 0xE0 (raw table row 0xA0 + computed cache tail).
// -----------------------------------------------
// Row/element of C_DialogueTimingDatabaseGenerated (by value). From ParseRows
// sub_1809407F0 / row-parse sub_180942028. raw+N = raw-column source; fields from +0x98
// are computed at parse time (raw-id resolution via qword_185324230 + derived caches).
// Field roles beyond the key are UNVERIFIED.

namespace wh::dialogmodule {

struct S_DialogueTiming {
    CryStringT<char> m_name;      // +0x00  key (raw+0, via sub_180942380 transform)
    CryStringT<char> m_str08;     // +0x08  raw+8
    CryStringT<char> m_str10;     // +0x10  raw+16
    CryStringT<char> m_str18;     // +0x18  raw+24
    CryStringT<char> m_str20;     // +0x20  raw+32
    CryStringT<char> m_str28;     // +0x28  raw+40
    CryStringT<char> m_str30;     // +0x30  raw+48
    uint32_t m_rawId;             // +0x38  raw+56 (resolved via qword_185324230)
    uint32_t m_d3C;               // +0x3C  raw+64
    uint8_t  m_pod40[0x10];       // +0x40  raw+72  16 bytes
    uint8_t  m_pod50[0x10];       // +0x50  raw+88  16 bytes
    uint64_t m_q60;               // +0x60  raw+104
    uint32_t m_d68;               // +0x68  raw+112
    uint32_t m_d6C;               // +0x6C  raw+116 (raw uint64; modeled split for alignment)
    uint32_t m_d70;               // +0x70
    uint32_t m_d74;               // +0x74  raw+124
    uint64_t m_q78;               // +0x78  raw+128
    uint32_t m_d80[5];            // +0x80..+0x93  raw+136..152
    uint8_t  m_b94[4];            // +0x94..+0x97  raw+156..159
    uint32_t m_resolvedIndex0;    // +0x98  computed
    uint32_t m_resolvedIndex1;    // +0x9C  computed
    uint8_t  m_cacheA0[0x10];     // +0xA0  computed from raw fields
    uint64_t m_cacheB0;           // +0xB0  computed
    uint32_t m_cacheB8;           // +0xB8  computed
    uint8_t  m_cacheBC[0x10];     // +0xBC  copy of raw+88
    uint8_t  _padCC[4];           // +0xCC  (m_cacheCC region alignment; see note)
    uint64_t m_cacheD0;           // +0xD0  computed (dossier: cache uint64 @+0xCC -- modeled
                                  //        8-aligned @+0xD0, raw offset packing UNVERIFIED)
    uint32_t m_cacheD8;           // +0xD8  computed (dossier +0xD4; alignment-shifted, UNVERIFIED)
    uint8_t  _padDC[4];           // +0xDC
};
static_assert(sizeof(S_DialogueTiming) == 0xE0, "S_DialogueTiming must be 0xE0");
static_assert(offsetof(S_DialogueTiming, m_rawId) == 0x38);
static_assert(offsetof(S_DialogueTiming, m_resolvedIndex0) == 0x98);

}  // namespace wh::dialogmodule
