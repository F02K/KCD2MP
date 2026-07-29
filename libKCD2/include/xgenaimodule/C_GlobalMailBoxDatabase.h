#pragma once
#include "C_MailboxDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_GlobalMailBoxDatabase : C_MailboxDatabase -- the runtime
// type of the mailbox-DB heap singleton at qword_185493110 (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x78 (identical to the base; the create-site
// sub_181125078 runs the base ctor then stamps
// &C_GlobalMailBoxDatabase::vftable).
// -----------------------------------------------
// Adds NO members and (beyond the vftable identity) no observed behavior of
// its own.

namespace wh::xgenaimodule {

class C_GlobalMailBoxDatabase : public C_MailboxDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GlobalMailBoxDatabase;
    ~C_GlobalMailBoxDatabase() override;   // [0] (own vftable stamped at create-site)
};
static_assert(sizeof(C_GlobalMailBoxDatabase) == 0x78, "trivial subclass: same 0x78");

}  // namespace wh::xgenaimodule
