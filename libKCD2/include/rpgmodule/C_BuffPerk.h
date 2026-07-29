#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffPerk -- perk that applies a buff (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_BuffPerk@rpgmodule@wh@@ (TD 0x184D53D78)  vtable 0x183A391B8.
// sizeof 0x48 (PROVEN: factory sub_180471308 operator new(72)).
// Extra fields init: +0x30 = 16-byte constant xmmword_183A391A0 (invalid buff id), +0x40 = 0.

namespace wh::rpgmodule {

class C_BuffPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffPerk;
    uint64_t m_buffId[2];   // +0x30  16-byte buff id; ctor = the invalid constant
                            //        (buff GUID candidate -- interior typing UNVERIFIED)
    void*    m_pActiveBuff;      // +0x40  ctor 0; activation worker sub_180470F98 stores a buff-instance pointer here via sub_1804712D0, back-links it (*(ptr+0x20)=this) and validity-checks it via sub_18039B210 [pointer to the applied buff instance; pointee not RE'd]. Name inferred.
};
static_assert(sizeof(C_BuffPerk) == 0x48, "C_BuffPerk must be 0x48 (operator new(72))");

}  // namespace wh::rpgmodule
