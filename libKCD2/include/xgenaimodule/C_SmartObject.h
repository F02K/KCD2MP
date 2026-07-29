#pragma once
#include <cstdint>
#include "C_InteractiveSmartEntity.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObject : C_InteractiveSmartEntity -- smart-object
// base (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x1A0 (adds NO data over ISE;
// variants embed C_Ownership at +0x1A0).  ctor sub_180A2B1FC (also constructs
// the inlined ISE level).  Abstract-ish base for the concrete variants
// C_SmartObjectDefault / C_NavigationSmartObject.
// -----------------------------------------------
// RTTI TD rva 0x4F5A4B8.  Primary vtable rva 0x3A6BE10 (41 slots -- ADDS slot
// [40] vs the 40-slot SmartEntity family tables).  Overrides vs ISE:
// [0]dtor 0x32D6C1C, [1]0x5D29E0, [16]0x32DC400, [18]0x32DC190, [24]0x7F9AC4,
// [31]0x1AA5B20, [32]0x413150, [33]0x1AA5B00, [34]0x1AA5AE0, [39]0x1A72480;
// NEW [40]0x66CD10 (return 0).  Secondary vtables +0x20 0x3A6BC18 /
// +0xA8 0x3A6BC38 / +0xB0 0x3A6BC90.

namespace wh::xgenaimodule {

class C_SmartObject : public C_InteractiveSmartEntity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObject;
    ~C_SmartObject() override;        // [0]  deleting dtor 0x32D6C1C
    uint32_t GetTypeMask() override;  // [1]  0x5D29E0
    void _vf16() override;            // [16] 0x32DC400
    void _vf18() override;            // [18] 0x32DC190
    void _vf24() override;            // [24] 0x7F9AC4 (base: nullsub)
    void _vf31() override;            // [31] 0x1AA5B20
    void _vf32() override;            // [32] 0x413150
    void _vf33() override;            // [33] 0x1AA5B00
    void _vf34() override;            // [34] 0x1AA5AE0
    void _vf39() override;            // [39] 0x1A72480
    virtual void* _vf40();            // [40] NEW 0x66CD10 (return 0) [U role]
};
static_assert(sizeof(C_SmartObject) == 0x1A0, "C_SmartObject adds no data over C_InteractiveSmartEntity");

}  // namespace wh::xgenaimodule
