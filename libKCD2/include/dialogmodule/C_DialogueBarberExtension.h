#pragma once
#include <cstdint>
#include <vector>
#include "I_DialogueExtension.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueBarberExtension -- barber (haircut) minigame extension
// (KCD2 1.5.6, kd7u).  sizeof ~0x70 UNVERIFIED (no clean alloc found).
// -----------------------------------------------
// RTTI TD 0x184B4E780; vtable 0x183E67418 (11 slots); ctor sub_181E68500. Drives the
// barber UI through C_BarberExtensionController. Observed ctor writes: +0x18 int64 =
// -100000 (price sentinel), style vectors @+0x30/+0x48 (40-byte records), shared-ptr-ish
// slots around +0x28/+0x60, float 0.5 @+0x6C. The +0x60 region observations overlap --
// exact tail layout UNVERIFIED; offsets of the pinned fields below are asserted, the
// total size is provisional.

namespace wh::rpgmodule { class C_Barber; }

namespace wh::dialogmodule {

class C_DialogInstance;
class C_BarberExtensionController;

class C_DialogueBarberExtension : public I_DialogueExtension {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueBarberExtension;
    bool     m_active;                 // +0x08  [per-extension convention; UNVERIFIED]
    uint8_t  _pad09[7];                // +0x09
    uint16_t m_u10;                    // +0x10  ctor 0 (WORD store; NOT the owner -- owner is at +0x20)
    uint8_t  _pad12[6];                // +0x12
    int64_t  m_priceSentinel;          // +0x18  ctor -100000
    C_DialogInstance* m_pOwner;        // +0x20  owning dialog (ctor sub_181E68500 arg a2; reads a2+0x280 = C_DialogInstance::m_pParticipant)
    C_BarberExtensionController* m_pController;  // +0x28  owned; ctor sub_181E68060 (alloc 0x60, ctor sub_181E5DFB0), released on reassign
    std::vector<void*> m_stylesA;      // +0x30  style records (40B stride) [elem type UNVERIFIED]
    std::vector<void*> m_stylesB;      // +0x48  style records (40B stride) [elem type UNVERIFIED]
    wh::rpgmodule::C_Barber* m_pBarber;  // +0x60  owned; ctor sub_181FD3D60 (alloc 0x78, ctor sub_182D25140 stores wh::rpgmodule::C_Barber vftable), released on reassign
    uint32_t _d68;                     // +0x68
    float    m_f6C;                    // +0x6C  ctor 0.5f
};
static_assert(offsetof(C_DialogueBarberExtension, m_priceSentinel) == 0x18);
static_assert(offsetof(C_DialogueBarberExtension, m_stylesA) == 0x30);
static_assert(offsetof(C_DialogueBarberExtension, m_stylesB) == 0x48);
static_assert(offsetof(C_DialogueBarberExtension, m_f6C) == 0x6C);
// sizeof == 0x70 by construction; total size ~0x70 UNVERIFIED (see header note).

}  // namespace wh::dialogmodule
