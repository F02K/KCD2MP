#include "playermodule/C_MinigameManager.h"
#include "Offsets/Offsets.h"

// C_MinigameManager engine-function forwarders (KCD2 WHGame.dll 1.5.6, kd7u;
// ids = kcd2 address library).

namespace wh::playermodule {

I_Minigame* C_MinigameManager::FindOrCreateSession(uint32_t userId, E_MinigameType::Type type,
                                                   bool create, bool includeFinished)
{
    // 0x182024240
    using Fn = I_Minigame* (__fastcall*)(C_MinigameManager*, uint32_t, uint8_t, char, char);
    static REL::Relocation<Fn> fn{ REL::ID(204151) };
    return fn(this, userId, static_cast<uint8_t>(type), create, includeFinished);
}

}  // namespace wh::playermodule
