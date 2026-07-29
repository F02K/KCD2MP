#pragma once
#include "C_SUBBBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEntitySubbrain : C_SUBBBase -- ABSTRACT intermediate
// shared by the two Smart* subbrains (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xD0
// (= base 0xC8 + one ptr @+0xC8; never operator-new'd directly -- ctor
// sub_180414120 is called only from the SmartObject/SmartArea subbrain ctors).
// -----------------------------------------------
// RTTI TD rva 0x4EFF340; COL rva 0x40E7278; vtable 0x183A34140, 37 slots --
// overrides ONLY [1] (deleting dtor sub_18320B5B8); slots 22,23,27,35,36 remain
// _purecall => still abstract.

namespace wh::xgenaimodule {

class C_SmartEntity;

class C_SmartEntitySubbrain : public C_SUBBBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEntitySubbrain;
    ~C_SmartEntitySubbrain() override;   // [1] deleting dtor sub_18320B5B8 (only override)

    C_SmartEntity* m_smartEntity;   // +0xC8  bound smart entity (ctor: 0) [U pointee identity by role]
};
static_assert(sizeof(C_SmartEntitySubbrain) == 0xD0, "C_SmartEntitySubbrain must be 0xD0");

}  // namespace wh::xgenaimodule
