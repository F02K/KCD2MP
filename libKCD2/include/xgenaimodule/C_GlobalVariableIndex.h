#pragma once
#include <cstdint>
#include "C_VariableIndex.h"

// -----------------------------------------------
// wh::xgenaimodule::C_GlobalVariableIndex : C_VariableIndex -- THE process-wide
// root variable index; singleton qword_185496678, the default parent of every
// C_VariableIndex (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x40 (create-site proven:
// operator new(64) in sub_18188E050, which stores the result INTO qword_185496678).
// -----------------------------------------------
// RTTI TD rva 0x4FFD1C8; COL rva 0x45CC6B8; vtable 0x184018B60, 21 slots.
// Overrides: [0] dtor sub_18345EA58 (unregisters from a global registry),
// [9] GetParent -> return 0 (no parent), [11], [19] sub_183460D4C
// (*(this+0x38) != arg).  ctor sub_18188E0B8: zero members, register self into two
// global registries (sub_18188E144, sub_180BC9924).

namespace wh::xgenaimodule {

class C_GlobalVariableIndex : public C_VariableIndex {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GlobalVariableIndex;
    ~C_GlobalVariableIndex() override;        // [0]  deleting dtor sub_18345EA58 (registry unregister)
    I_VariableIndex* GetParent() override;    // [9]  return 0 -- the root has no parent
    void _vf11() override;                    // [11] [U role]
    bool _vf19() override;                    // [19] sub_183460D4C: *(this+0x38) != arg [U role]

    uint64_t m_registration[3];   // +0x28..+0x3F  registry pos/callback state; [19] reads +0x38 [U roles]
};
static_assert(sizeof(C_GlobalVariableIndex) == 0x40, "C_GlobalVariableIndex must be 0x40 (operator new(64))");

}  // namespace wh::xgenaimodule
