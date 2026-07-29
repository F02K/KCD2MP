#pragma once
#include <cstdint>
#include <vector>
#include "E_KeybindSlotType.h"

// -----------------------------------------------
// wh::playermodule::S_KeybindsSuperactionSlots -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50.
// -----------------------------------------------
// Per-controller slot block of S_KeybindsSuperaction (x3, indexed by
// E_KeybindController::Type).  STRUCT NAME INVENTED (no RTTI for the block itself);
// layout byte-exact from the parser sub_1808FB4CC and the mutators below.
// ctor sub_18164B420 (zero-init); the inner vectors are MSVC std::vector (end/cap are
// POINTERS: grow sub_1823CAEC8 reads (a1[1]-*a1)>>3), UNLIKE the manager-side
// boost::container::vector map values.
// Vector index = UI slot (<control slot="N">, else append order).  Classic edit->commit
// triple: at load (0x1808FBB05) register A, then C=copy(A), B=copy(A); rebinds edit ONLY B;
// C_Keybinds::CommitBindings (0x180F68DC8) unregisters C, C=copy(B), registers C.  An empty
// string in a slot = unbound.

namespace wh::playermodule {

struct S_KeybindsSuperactionSlots {
    E_KeybindSlotType::Type       m_type;           // +0x00  <superaction keyboard=/xboxpad=/ps4pad=> attr; Writeable gates ChangeInput
    uint32_t                      _pad04;           // +0x04
    std::vector<CryStringT<char>> m_defaultInputs;  // +0x08  A: factory defaults from <control input=>; ResetToDefaults/ResetSlotToDefault source
    std::vector<CryStringT<char>> m_appliedInputs;  // +0x20  C: the set currently registered in the engine action maps (registrar sub_180A111BC)
    std::vector<CryStringT<char>> m_pendingInputs;  // +0x38  B: live/edited set; ChangeInput (0x182E18250) and profile-load (sub_180F68D58) write here
};
static_assert(sizeof(S_KeybindsSuperactionSlots) == 0x50, "S_KeybindsSuperactionSlots must be 0x50");
static_assert(offsetof(S_KeybindsSuperactionSlots, m_defaultInputs) == 0x08, "defaults at 0x08");
static_assert(offsetof(S_KeybindsSuperactionSlots, m_pendingInputs) == 0x38, "pending at 0x38");

}  // namespace wh::playermodule
