#pragma once
#include "C_SUBBBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_EmptySUBB : C_SUBBBase -- the do-nothing subbrain (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0xC8 (== base; NO new members; create-site
// proven: factory sub_18041A4DC "Default"/"" branch operator new(200) ->
// memset -> base ctor sub_18041414C -> vptr swap).
// -----------------------------------------------
// RTTI TD rva 0x4EFDEF0; COL rva 0x451DC30; vtable 0x183FD7710, 37 slots.
// Fills ONLY the 5 base purecalls: [22]sub_180838AE0 (return 0)
// [23]sub_18041A6A0 (return 1) [27]sub_181A74280 (type const)
// [35]sub_18075EDB8 [36]sub_1807F8484.  Every other slot inherited unchanged.

namespace wh::xgenaimodule {

class C_EmptySUBB : public C_SUBBBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EmptySUBB;
    void  Step() override;              // [22] sub_180838AE0 (return 0)
    bool  IsRunning() override;         // [23] sub_18041A6A0 (return 1)
    void* GetSubbrainType() override;   // [27] sub_181A74280
    void  RequestOwnerTick() override;  // [35] sub_18075EDB8 (common body)
    void  CancelOwnerTick() override;   // [36] sub_1807F8484 (common body)
};
static_assert(sizeof(C_EmptySUBB) == 0xC8, "C_EmptySUBB must be 0xC8 (operator new(200))");

}  // namespace wh::xgenaimodule
