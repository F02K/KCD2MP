#include "framework/C_SaveGameManager.h"
#include "Offsets/Offsets.h"

// C_SaveGameManager engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, kd7u).

namespace wh::framework {

bool C_SaveGameManager::CreateSaveGame(E_SaveGameType::Type type, int32_t index, const char* name)
{
    // 0x180794928 (prototype verified): builds the C_SaveGameDescription, serializes the
    // world (0x8000-grow / 40 MB cap buffer) and writes + registers the .whs slot file.
    // index < 0 with a non-checkpoint type resolves the slot by name; LevelSwitchSave(6)
    // takes the queued async-checkpoint branch instead of writing directly.
    using Fn = bool (__fastcall*)(C_SaveGameManager*, E_SaveGameType::Type, int32_t, const char*);
    static REL::Relocation<Fn> fn{ REL::ID(22) };
    return fn(this, type, index, name);
}

}  // namespace wh::framework
