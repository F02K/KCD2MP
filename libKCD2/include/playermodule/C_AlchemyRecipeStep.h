#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "../framework/C_EEExpression.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyRecipeStep -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// RTTI .?AVC_AlchemyRecipeStep@playermodule@wh@@ (TD 0x184DE1750; 7-slot vtable 0x183A51708).
// One <AlchemyRecipeStep> row.  RTTR-reflected by sub_18021C010.  Size stride-VERIFIED: the DSL
// eval driver sub_182E2BD10 iterates recipe.Steps with `i += 0x38`.  Every 8-byte slot accounted.
//
// Condition is a getter/setter reflected property: the raw string lives at +0x10 (getter
// sub_181A72EC0 = this+0x10), and the SETTER sub_1807806A0 additionally compiles it into the
// framework::C_EEExpression tree stored at +0x20 (released via sub_1803C14C4 on re-set).
// StepType is a CryStringT NAME ("Trivial"/"Easy"/"Hard") referencing an AlchemyRecipeStepType
// row -- there is NO binary enum for it (no RTTR enum registration exists).

namespace wh::playermodule {

class C_AlchemyRecipeStep {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyRecipeStep;
    virtual ~C_AlchemyRecipeStep() = default;

    int32_t                    m_stepId;             // +0x08  <AlchemyRecipeStep StepId>
    uint32_t                   _pad0C;               // +0x0C
    CryStringT<char>           m_condition;          // +0x10  raw Condition DSL text
    CryStringT<char>           m_uiText;             // +0x18  loc key shown in the recipe book
    framework::C_EEExpression* m_pCompiledCondition; // +0x20  compiled functor tree (status 3 = ok)
    int32_t                    m_feedbackPriority;   // +0x28  highest-priority failing step wins the bark
    uint32_t                   _pad2C;               // +0x2C
    CryStringT<char>           m_stepType;           // +0x30  StepType row name (Trivial/Easy/Hard)
};
static_assert(sizeof(C_AlchemyRecipeStep) == 0x38, "C_AlchemyRecipeStep must be 0x38");
static_assert(offsetof(C_AlchemyRecipeStep, m_pCompiledCondition) == 0x20, "tree at 0x20");
static_assert(offsetof(C_AlchemyRecipeStep, m_stepType) == 0x30, "stepType at 0x30");

}  // namespace wh::playermodule
