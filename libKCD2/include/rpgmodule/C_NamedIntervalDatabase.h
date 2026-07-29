#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"

// -----------------------------------------------
// wh::rpgmodule::C_NamedIntervalDatabase -- the "nervousness_flag" table: a
// GENERIC named min/max interval database (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof ~0xB0 (ctor writes through +0xA8; std::string SSO @+0x50/+0x70)
// [UNPROVEN alloc -- global static, no size assert].
// -----------------------------------------------
// RTTI TD rva 0x4D37B98; vtable 0x3ACDF58; COL(??_R4) 0x18412BB08.  Stack:
// C_ObjectDatabaseKeyIndexed< C_ObjectTableDatabase<C_NamedInterval,
// S_NamedIntervalDBData>, Key > -> C_ObjectDatabase -> C_ObjectDatabaseBase ->
// I_ObjectDatabase (all mdisp 0).  GLOBAL STATIC @ 0x18532DA40 (ctor
// sub_180EF54C8(&global, "nervousness_flag") from static-init sub_1801B02A0;
// base ctor sub_180EF4F30; table name @0x183F06428, group "rpg" @DB+0x70; row
// loader sub_18113EBDC, XML xpath "//nervousness_flag/@name" @0x183F06440).
// Rows = the nervousness tiers (noNervousness/low/mid/high/alerted at
// 0/0.3/0.6/0.9 + behavior thresholds like crime_investigateSound 0.06),
// checked by the BT node CheckNervousnessInterval / concept node
// HasNervousnessFlag via S_NervousnessFlagDefinitionID.

namespace wh::rpgmodule {

class C_NamedInterval;            // row type -- not RE'd (named [min;max) interval)
struct S_NamedIntervalDBData;     // per-row DB payload -- not RE'd

class C_NamedIntervalDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTableDatabase<C_NamedInterval, S_NamedIntervalDBData>, uint32_t /*Key [U exact]*/>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NamedIntervalDatabase;
    // Global static instance @ RVA 0x532DA40 ("nervousness_flag", group "rpg").
};

}  // namespace wh::rpgmodule
