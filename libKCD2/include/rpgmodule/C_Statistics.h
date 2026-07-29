#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <boost/container/flat_map.hpp>
#include "../Offsets/vtables/ISystem.h"           // Offsets::ISystemEventListener replica
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../databasemodule/I_DatabaseListener.h"
#include "I_Statistics.h"
#include "C_EventCounter.h"

// -----------------------------------------------
// wh::rpgmodule::C_Statistics -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x880.
// -----------------------------------------------
// RTTI .?AVC_Statistics@rpgmodule@wh@@ (TD 0x184D8F1D0; bases I_Statistics @+0x00,
// I_DatabaseListener @+0x08, ISystemEventListener @+0x10).  The game-statistics registry:
// one lazily-created singleton (creator 0x1810E3404, factory sub_1810E34B0 alloc 2176,
// global qword_18548E320; C_RPGModule's RTTR "Statistics" getter resolves to it).  Holds one
// counter object per statistic.xml row, keyed by statistic_name ("PotionsBrewed" IntegerSum,
// "UniquePotionsBrewed" DistinctGuid, ...); counters are polymorphic I_Counter
// implementations (I_Counter TD 0x184D8F058; C_Counter/C_ScriptCounter/... family) --
// pointer-opaque here.  The inline C_EventCounter @+0x80 is the game-event intake feeding
// the counters.
//
// Distinct-guid counters keep an FNV1a64-hashed guid hash-set at facet+0x20 (0x20-byte list
// nodes, guid @node+0x10): AddGuid 0x182CF8054, insert 0x180B5F8A8, bucket find 0x180B5F9A0;
// the console/script adder chain 0x181FCE870 -> checked facet cast 0x182CE973C is the
// certification source.  State is save-persisted (per-statistic chunks, by-name dispatcher
// 0x181FE8000 ids 0x1778/0x1779).

namespace wh::rpgmodule {

class I_Counter;   // counter facet base; concrete counter classes not modeled

class C_Statistics
    : public I_Statistics                       // +0x00
    , public wh::databasemodule::I_DatabaseListener  // +0x08
    , public Offsets::ISystemEventListener      // +0x10
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Statistics;

    // Lazy singleton (creates on first use -- never null in practice).
    static C_Statistics* GetInstance();
    // Find-only over the entry table; null when no such statistic_name.
    I_Counter* GetCounterByName(const CryStringT<char>& name);
    
    bool HasDistinctGuid(const char* statisticName, const CryGUID& guid);

    boost::container::flat_map<CryStringT<char>, std::shared_ptr<I_Counter>>
                    m_counters;      // +0x18  sorted by name string (find 0x180C692B4 =
                                     //        lower_bound 0x180C69360 + trailing strcmp;
                                     //        shared_ptr per the _Ref_count@VC_Counter RTTR TDs)
    uint8_t         _unk30[0x78 - 0x30];  // +0x30  ctor-zeroed registry fields, unmapped
    uint8_t         m_flag78;        // +0x78  ctor 0
    uint8_t         _pad79[7];       // +0x79
    C_EventCounter  m_eventCounter;  // +0x80  the inline event intake (ctor sub_1810E3574)
    uint16_t        m_word878;       // +0x878 factory-zeroed flags [semantics unknown]
    uint8_t         _pad87A[6];      // +0x87A
};
static_assert(sizeof(boost::container::flat_map<CryStringT<char>, std::shared_ptr<I_Counter>>) == 0x18,
              "boost flat_map header is {begin,size,cap}");
static_assert(offsetof(C_Statistics, m_counters) == 0x18, "counter map at 0x18");
static_assert(offsetof(C_Statistics, m_eventCounter) == 0x80, "event counter at 0x80");
static_assert(sizeof(C_Statistics) == 0x880, "C_Statistics must be 0x880");

}  // namespace wh::rpgmodule
