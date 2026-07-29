#include "rpgmodule/C_DistinctGuidCounter.h"
#include "Offsets/Offsets.h"

// C_DistinctGuidCounter engine-function forwarders (mirrors C_Soul.cpp).

namespace wh { namespace rpgmodule {

C_DistinctGuidCounter* C_DistinctGuidCounter::FromCounter(I_Counter* counter)
{
    // sub_182CE973C: rttr-checked downcast to the distinct-guid counter type; null on
    // mismatch (handles any pointer adjustment itself).
    using Fn = C_DistinctGuidCounter* (__fastcall*)(I_Counter*);
    static REL::Relocation<Fn> fn{ REL::ID(367573) };
    return counter ? fn(counter) : nullptr;
}

}}  // namespace wh::rpgmodule
