#pragma once
#include <cstdint>
#include "C_SmartObject.h"
#include "C_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObjectDefault : C_SmartObject -- the default
// concrete smart object (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof ~0x1D0
// (INFERRED from ctor writes: C_Ownership @+0x1A0 spans to +0x1D0; no direct
// operator-new constant traced) -- NO static_assert.  ctor sub_180A2B168,
// create-site sub_180A2A5C4.
// -----------------------------------------------
// RTTI TD (see rtti_map).  Own vtables at +0x0/+0x20/+0xA8/+0xB0 (RVAs not
// captured in the G6F pass [U]); overrides beyond the dtor NOT diffed [U].
// C_Ownership defaults at +0x1A8/+0x1B0 (qword_185338430), zeros +0x1B8..0x1C8
// -- these are the C_Ownership interior, covered by the embedded member.

namespace wh::xgenaimodule {

class C_SmartObjectDefault : public C_SmartObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObjectDefault;
    ~C_SmartObjectDefault() override;   // [0] (own deleting dtor; RVA not captured [U])

    C_Ownership m_ownership;   // +0x1A0  embedded (0x30)
};
// sizeof 0x1D0 INFERRED (members end at +0x1D0; alloc constant not traced) -- no static_assert.

}  // namespace wh::xgenaimodule
