#pragma once
#include <cstdint>
#include "../framework/I_EEFunction.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogFunctionExpression -- compiled dialogue expression
// (KCD2 1.5.6, kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI TD 0x184B4B6D0; vtable 0x183A518F8 (8 slots); ctor sub_180782A3C; deleting-dtor
// [0] 0x1827FBF90. The general dialogue expression evaluator -- the fallback when a
// branch condition is not one of the 5 named intrinsics. GetName returns the source
// string; Evaluate (sub_1827FCC00) runs the compiled expression (built by sub_180782828,
// freed by sub_1803C14C4) over the context -> float. Rows come from the
// "dialogue_functions" table (C_DialogueFunctionsDatabase).

namespace wh::dialogmodule {

class C_DialogFunctionExpression : public framework::I_EEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogFunctionExpression;
    CryStringT<char> m_exprSource;   // +0x08  expression source text
    void* m_pCompiledExpr;           // +0x10  owned* -> 0x38 compiled-expr {CryStringT compiled@+0, CryStringT src@+8, int status@+0x10, std::vector@+0x18, u8@+0x30}; built sub_180782828/sub_180781820, freed sub_1803C14C4
    bool  m_isValid;                 // +0x18  compile succeeded
    uint8_t _pad19[7];               // +0x19
};
static_assert(sizeof(C_DialogFunctionExpression) == 0x20, "C_DialogFunctionExpression must be 0x20");

}  // namespace wh::dialogmodule
