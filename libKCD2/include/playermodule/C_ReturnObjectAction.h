#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_HandItemParams.h"

// -----------------------------------------------
// wh::playermodule::C_ReturnObjectAction -- return the held object to the table (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_ReturnObjectAction@playermodule@wh@@ (TD 0x184DD5240); vtable 0x183C23010; ctor
// sub_1817F7AF4 (base-ctor path also used by the S_HandItemParams factory sub_1817F7A38 /
// sub_1806C6A00, which builds the params by brew state: {0,0x0100} / {0,0x0101} / {1,0x0001}).
// Ctor defaults m_params = {2, 0}.  Plays the AlchemyReturn fragment; completes on "Detach".

namespace wh::playermodule {

class C_ReturnObjectAction : public C_AlchemyActionBase<S_HandItemParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReturnObjectAction;

    uint32_t m_evDetach;   // +0x68  ~CRC32("detach")
    uint32_t _pad6C;       // +0x6C
    uint64_t m_state70;    // +0x70  zeroed by ctor [role UNVERIFIED]
};
static_assert(sizeof(C_ReturnObjectAction) == 0x78, "C_ReturnObjectAction must be 0x78");

}  // namespace wh::playermodule
