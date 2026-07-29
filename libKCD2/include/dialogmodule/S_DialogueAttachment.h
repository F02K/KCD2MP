#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::S_DialogueAttachment -- "dialogue_attachment" generated-table row
// (KCD2 1.5.6, kd7u).  sizeof 0xA0 (raw table row 0x98 + 2 resolved indices).
// -----------------------------------------------
// Row/element of C_DialogueAttachmentDatabaseGenerated (by value, vector stride 160).
// From ParseRows sub_180C1EF9C / row-parse sub_180C1F098. Raw-column offsets are noted
// as raw+N; +0x98/+0x9C are computed at parse time (raw-id resolution via the shared
// global qword_185324230; attachment slot via sub_180C1F3A0). Field roles beyond the
// key are UNVERIFIED (attachment name / bone / offsets per the table schema).

namespace wh::dialogmodule {

struct S_DialogueAttachment {
    CryStringT<char> m_name;      // +0x00  key (raw+0)
    uint8_t  m_b08;               // +0x08  raw+8
    uint8_t  m_b09;               // +0x09  raw+9
    uint8_t  _pad0A[2];           // +0x0A
    uint32_t m_d0C;               // +0x0C  raw+12
    CryStringT<char> m_str10;     // +0x10  raw+16
    CryStringT<char> m_str18;     // +0x18  raw+24
    uint32_t m_rawId;             // +0x20  raw+32 (resolved via qword_185324230)
    uint8_t  _pad24[4];           // +0x24
    CryStringT<char> m_str28;     // +0x28  raw+40
    uint32_t m_d30;               // +0x30  raw+48
    uint32_t m_d34;               // +0x34  raw+56
    uint32_t m_d38;               // +0x38  raw+60
    uint8_t  m_pod3C[0x10];       // +0x3C  raw+64  16 bytes (Vec3+f / quat)
    uint32_t m_d4C;               // +0x4C  raw+80  low dword  (8-byte raw value; split to
    uint32_t m_d50;               // +0x50  raw+80  high dword  keep natural alignment)
    uint32_t m_d54;               // +0x54  raw+88
    uint8_t  m_pod58[0x10];       // +0x58  raw+92  16 bytes
    uint64_t m_q68;               // +0x68  raw+108
    uint32_t m_d70;               // +0x70  raw+116
    uint8_t  m_pod74[0x10];       // +0x74  raw+120  16 bytes
    uint8_t  _pad84[4];           // +0x84
    CryStringT<char> m_str88;     // +0x88  raw+136
    CryStringT<char> m_str90;     // +0x90  raw+144
    uint32_t m_resolvedIndex0;    // +0x98  computed (qword_185324230 lookup)
    uint32_t m_resolvedIndex1;    // +0x9C  computed (asset/attachment slot, sub_180C1F3A0)
};
static_assert(sizeof(S_DialogueAttachment) == 0xA0, "S_DialogueAttachment must be 0xA0 (vector stride 160)");
static_assert(offsetof(S_DialogueAttachment, m_str28) == 0x28);
static_assert(offsetof(S_DialogueAttachment, m_str88) == 0x88);
static_assert(offsetof(S_DialogueAttachment, m_resolvedIndex0) == 0x98);

}  // namespace wh::dialogmodule
