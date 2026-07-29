#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyDirectorSlot -- alchemy action-director slot ids (KCD2 1.5.6, utem).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the slot an action occupies = its m_actionSequenceId (I_Action +0x3C),
// written as a constant by each action ctor.  Values verified from the ctors:
//   1 = the alchemy base-action ctor default (every player VERB action; the channel the
//       verb-wait polls via IsSlotOccupied),
//   2 = C_PotEffectAction ctor 0x1818C29B8 (session-start ambient: boil/fire sim, writes
//       m_potBoilingState + pot audio/render params every frame),
//   3 = C_HourglassEffectAction ctor 0x181945C4C (session-start ambient: sandglass effect),
//   4 = C_SwitchPotHalterAction ctor (parallel pot-halter channel; see I_AlchemyAction.h).
// Slot 0 exists in the vector but no writer of 0 has been observed [UNVERIFIED role].

namespace wh::playermodule {

struct E_AlchemyDirectorSlot {
    enum Type : int32_t {
        Verb            = 1,   // player verb channel (one verb action at a time)
        PotEffect       = 2,   // ambient boil/fire simulation (C_PotEffectAction)
        HourglassEffect = 3,   // ambient sandglass effect (C_HourglassEffectAction)
        PotHalter       = 4,   // parallel pot-halter channel (C_SwitchPotHalterAction)
    };
};

}  // namespace wh::playermodule
