#include "playermodule/S_AlchemyBrewState.h"

#include "Offsets/Offsets.h"
#include "playermodule/C_AlchemyResource.h"

// S_AlchemyBrewState engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, verified in utem).

namespace wh::playermodule {

float S_AlchemyBrewState::GetAverageBoilTurns(const std::vector<C_AlchemyResource*>& records,
                                              const CryGUID& itemClass,
                                              uint8_t accumulatorIndex)
{
    // sub_182E2C110: avg(records[i] + 4*index + 0x14) over GUID matches / S_Constants[381].
    using Fn = float(__fastcall*)(const std::vector<C_AlchemyResource*>*, const CryGUID*, uint8_t);
    static REL::Relocation<Fn> fn{ REL::ID(376215) };
    return fn(&records, &itemClass, accumulatorIndex);
}

}  // namespace wh::playermodule
