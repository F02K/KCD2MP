#pragma once
#include <cstdint>
#include <vector>
#include "../Offsets/vtables/IUIActionListener.h"

// -----------------------------------------------
// wh::dialogmodule::C_BarberExtensionController -- barber UI action controller
// (KCD2 1.5.6, kd7u).  sizeof ~0x60 UNVERIFIED.
// -----------------------------------------------
// RTTI TD 0x184B49C50; vtable 0x183E63460 (3 slots : IUIActionListener -- [0]/[1] UI
// action callbacks, [2] nullsub at the dtor position); ctor
// sub_181E5DFB0(this, barberExt, a3). Bridges flash UI actions to
// C_DialogueBarberExtension; copies the style vectors from the extension. Total size
// and tail roles UNVERIFIED -- offsets of the pinned fields are asserted.

namespace wh::dialogmodule {

class C_BarberExtensionController : public Offsets::IUIActionListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BarberExtensionController;
    C_DialogueBarberExtension* m_pBarberExt;  // +0x08  ctor sub_181E5DFB0 arg2 (non-owning back-ptr)
    uint64_t _q10;                     // +0x10  [not walked]
    std::vector<void*> m_stylesA;      // +0x18  copied from ext +0x30 [elem type UNVERIFIED]
    std::vector<void*> m_stylesB;      // +0x30  copied from ext +0x48 [elem type UNVERIFIED]
    uint64_t _q48;                     // +0x48  [not walked]
    uint8_t  m_listenerNode[0x10];     // +0x50  intrusive listener-list node [UNVERIFIED]
};
static_assert(offsetof(C_BarberExtensionController, m_stylesA) == 0x18);
static_assert(offsetof(C_BarberExtensionController, m_stylesB) == 0x30);
static_assert(offsetof(C_BarberExtensionController, m_listenerNode) == 0x50);
// sizeof == 0x60 by construction; total size ~0x60 UNVERIFIED (see header note).

}  // namespace wh::dialogmodule
