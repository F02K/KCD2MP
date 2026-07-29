#pragma once
#include <cstddef>
#include "CBrush.h"

// -----------------------------------------------
// COwnedBrush : CBrush -- WH-ADDED brush subclass, KCD2 WHGame.dll 1.5.6 (e4cp). sizeof 0x108.
// -----------------------------------------------
// Not present in stock CryEngine. RTTI .?AVCOwnedBrush@@; vtable 0x183A6F148 (stored
// @0x180A796C0). Ctor 0x180A796A0: runs CBrush::CBrush, zeroes the single new member at
// +0x100, installs its own vtable. Created by C3DEngine::CreateRenderNode(eERType_OwnedBrush
// = 26/0x1A): alloc `sub_1804F75C0(0x108)` + memset @0x180A795AF..0x180A795C5.
// The +0x100 member's PURPOSE IS UNVERIFIED (only offset/size/zero-init proven); "owner"
// reading comes from the class name only.

class COwnedBrush : public CBrush {
public:
    void* m_unk100 = nullptr;   // +0x100  pointer-shaped, zero-init; purpose unknown
};
static_assert(sizeof(COwnedBrush) == 0x108, "alloc+memset 0x108 @0x180A795AF");
static_assert(offsetof(COwnedBrush, m_unk100) == 0x100, "single new member @+0x100");
