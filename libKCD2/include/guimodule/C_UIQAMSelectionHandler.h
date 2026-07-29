#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIQAMSelectionHandler -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// RTTI .?AVC_UIQAMSelectionHandler@guimodule@wh@@ (TD 0x184CD0F50, COL 0x4136418).
// Base: C_UIBase @+0x00. vtable 0x183C12550 (10 slots, pure C_UIBase shape -- every
// slot is the C_UIBase default impl; behavior added through the signals, not virtuals).
// Built by factory sub_1817895D4 (alloc 0x38) in the C_GUIModule ctor, owned at
// C_GUIModule+0x88 (module-owned like C_UIBuffsManager, not in the screen vector).
//
// QAM = Quick Action Menu (the radial weapon/item selector). Selection changes are
// published through the two C_Signal heads. NEW vs KCD1. Signal argument signatures
// and the two int roles are UNVERIFIED (writers live in the QAM flash leaf, outside
// the G2 spine scope).

namespace wh::guimodule {

class C_UIQAMSelectionHandler : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIQAMSelectionHandler;
    wh::shared::C_Signal<> m_signalA;   // +0x10  empty-sentinel unk_185665430 (signature UNVERIFIED)
    wh::shared::C_Signal<> m_signalB;   // +0x20  empty-sentinel unk_185665430 (signature UNVERIFIED)
    int32_t m_30;                       // +0x30  ctor 0 (UNVERIFIED; ctor zeroes 0x2C..0x33 as one qword)
    int32_t m_34;                       // +0x34  ctor 0 (UNVERIFIED)
};
static_assert(sizeof(C_UIQAMSelectionHandler) == 0x38, "C_UIQAMSelectionHandler must be 0x38");

}  // namespace wh::guimodule
