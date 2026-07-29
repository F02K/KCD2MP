#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

// -----------------------------------------------
// wh::dialogmodule::C_DialogInteraction -- speaker/listener request record
// (KCD2 1.5.6, kd7u).  sizeof ~0xE8 (232) UNVERIFIED.
// -----------------------------------------------
// RTTI TD 0x184B4DF50; vtable 0x183A42510 (1 slot: deleting dtor); ctor
// sub_180614DCC(this, a2, a3, a4:byte); dtor sub_180614C80. A dialog request pairing
// two participants (a2/a3 = ids/handles copied to +0x58/+0x68). No behavior beyond the
// dtor; field roles UNVERIFIED. Pinned ctor writes: +0xC0 qword = 1, +0xC8 word = 1,
// +0xCA byte = a4. Total size approximate -- offsets asserted, sizeof not.

namespace wh::dialogmodule {

struct DialogInteractionLine;      // [SYNTHETIC] 0x58 record (2x CryStringT<char> + nested vector)
class C_DialogInteractionState;    // [SYNTHETIC] 0x38 state obj (ctor sub_180614F90)

class C_DialogInteraction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogInteraction;
    virtual ~C_DialogInteraction();   // [0] (only vtable slot)

    std::vector<DialogInteractionLine> m_records;  // +0x08  0x58B records (2x CryStringT<char> + nested vector); dtor 0x181439A64
    uint8_t  _unk20[0x38];           // +0x20..+0x57  zeroed by ctor, no dtor cleanup [not walked]
    uint8_t  m_participantA[0x10];    // +0x58  speaker id/handle (ctor a2) [encoding UNVERIFIED]
    uint8_t  m_participantB[0x10];    // +0x68  listener id/handle (ctor a3) [encoding UNVERIFIED]
    uint8_t  _unk78[0x30];           // +0x78..+0xA7  zeroed by ctor, no dtor cleanup [not walked]
    C_DialogInteractionState* m_state;  // +0xA8  owning ptr, 0x38B state obj (ctor 0x180614F14 alloc 56 -> sub_180614F90; dtor deletes); pointee+0x35 = a4 (0=dialog,1=chat_interaction)
    uint8_t  _unkB0[0x10];           // +0xB0..+0xBF  zeroed by ctor, no dtor cleanup [not walked]
    uint64_t m_qC0;                   // +0xC0  ctor 1
    uint16_t m_wC8;                   // +0xC8  ctor 1
    uint8_t  m_bCA;                   // +0xCA  ctor a4
    uint8_t  m_bCB;                  // +0xCB  ctor 0
    uint8_t  _padCC[4];              // +0xCC..+0xCF  alignment padding
    std::vector<uint32_t> m_dwList;  // +0xD0..+0xE7  4-byte trivial elems; dtor 0x180A608D4
};
static_assert(offsetof(C_DialogInteraction, m_participantA) == 0x58);
static_assert(offsetof(C_DialogInteraction, m_qC0) == 0xC0);
// sizeof == 0xE8 by construction; total size ~0xE8 UNVERIFIED (see header note).

}  // namespace wh::dialogmodule
