#include "rpgmodule/C_Statistics.h"
#include "rpgmodule/C_DistinctGuidCounter.h"
#include "Offsets/Offsets.h"

// C_Statistics engine-function forwarders: thin wrappers around verified KCD2 1.5.6 RVAs
// (mirrors C_Soul.cpp).

namespace wh { namespace rpgmodule {

C_Statistics* C_Statistics::GetInstance()
{
    // sub_1810E3404: mutex-guarded create-on-first-use of qword_18548E320.
    using Fn = C_Statistics* (__fastcall*)();
    static REL::Relocation<Fn> fn{ REL::ID(92548) };
    return fn();
}

I_Counter* C_Statistics::GetCounterByName(const CryStringT<char>& name)
{
    // sub_180C69270: find-only lookup in m_counters; null when absent.
    using Fn = I_Counter* (__fastcall*)(C_Statistics*, const CryStringT<char>*);
    static REL::Relocation<Fn> fn{ REL::ID(66785) };
    return fn(this, &name);
}

bool C_Statistics::HasDistinctGuid(const char* statisticName, const CryGUID& guid)
{
    auto* counter =
        C_DistinctGuidCounter::FromCounter(GetCounterByName(CryStringT<char>(statisticName)));
    return counter && counter->Contains(guid);
}

}}  // namespace wh::rpgmodule
