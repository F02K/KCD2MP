#pragma once
#include <cstdint>
#include "I_DialogueExtension.h"
#include "../framework/I_SourceMonitorListener.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueMapPlayerPositionExtension -- map player-position
// extension (KCD2 1.5.6, kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI TD 0x184B4FD50. Bases: I_DialogueExtension (mdisp 0, vtable 0x183E67C90, 11
// slots) + framework::I_SourceMonitorListener (mdisp 0x8, vtable 0x183E67C80, 1 slot).
// ctor sub_181E6B4F0: subscribes the +0x8 listener subobject to the source monitor as
// "DialogueMapPlayerPositionExtension" (type/prio 11). Member roles UNVERIFIED (the MI
// listener shifts the conventional active/owner pair to +0x10/+0x18).

namespace wh::dialogmodule {

class C_DialogueMapPlayerPositionExtension
    : public I_DialogueExtension, public framework::I_SourceMonitorListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueMapPlayerPositionExtension;
    // +0x00 I_DialogueExtension vptr / +0x08 I_SourceMonitorListener vptr
    bool    m_active;     // +0x10  [role UNVERIFIED]
    uint8_t _pad11[7];    // +0x11
    C_DialogInstance* m_pOwner;  // +0x18  owning dialog (ctor sub_181E6B4F0 arg a2)
    uint8_t _b20;         // +0x20  ctor zeroes 1 byte [role UNVERIFIED; not a pointer]
    uint8_t _pad21[7];    // +0x21
};
static_assert(sizeof(C_DialogueMapPlayerPositionExtension) == 0x28,
              "C_DialogueMapPlayerPositionExtension must be 0x28");

}  // namespace wh::dialogmodule
