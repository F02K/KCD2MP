#pragma once
#include "C_LogBase.h"

// -----------------------------------------------
// wh::questmodule::C_Log -- plain quest log entry (KCD2 1.5.6, kd7u).  sizeof 0x30 (alloc 48).
// -----------------------------------------------
// Own vtable 0x183ED62B8; ctor sub_180F6F91C (memset + C_LogBase ctor + own vftable, NO new
// members); registered via sub_182AEFA40.

namespace wh::questmodule {

class C_Log : public C_LogBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Log;
};
static_assert(sizeof(C_Log) == 0x30, "C_Log adds no data (alloc 48)");

}  // namespace wh::questmodule
