#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "I_EEFunction.h"

// -----------------------------------------------
// wh::framework::C_EEExpression -- compiled expression-engine unit (KCD2 1.5.6, kd7u/utem).
// sizeof 0x38 (alloc(56) in the Condition setter sub_1807806A0).
// -----------------------------------------------
// [SYNTHETIC NAME] -- no RTTI (non-polymorphic, refcount-freed via sub_1803C14C4).
// Built by sub_180781820(node, exprText, debugName); compiled by sub_180781598 -> tokenizer
// sub_180781204 (shunting-yard) with identifiers resolved by sub_1807809A0 against the global
// engine (qword_185168B30; also *(S_GameContext+0x60)): +0x418 string pool (char*[]; '...'
// literals appended at compile, referenced by index), +0x430 function/const manager, +0x438
// gvars, +0x440 local vars, +0x448 expression registry.  Constants fold to literals at compile.
//
// COMPILED PROGRAM (installer sub_18078201C, decoded from the RPN walker sub_1803BC690 and the
// operand decoder sub_1803BCC98): ONE co-allocated buffer holding the 16-byte OPERAND entries
// first, then the int32 OPCODE stream.  Opcodes 1..4 consume the next operand entry in order;
// everything else operates on the value stack.  Function-call entries carry the RESOLVED
// I_EEFunction* -- its GetArgCount() stack values (pushed before the call, in source order) are
// the arguments; string arguments are FloatLiteral entries holding the engine string-pool INDEX.
// Evaluate ctx frame (built at each call, sub_1803BCC98 v49..v52): {engine*, valueStack*,
// ctxSources (5-qword entries), ctxCount}.
// Wrapper sub_1803BC4D4 gates on m_status==Resolved, runs the walker on a 256-float stack, pops
// the final value.  Alchemy instance: one node per C_AlchemyRecipeStep::Condition (step+0x20).

namespace wh::framework {

// m_status values (writers: sub_180781820 / sub_180781598)
enum class E_EEExpressionStatus : int32_t {
    Empty        = 0,
    NeedsCompile = 2,
    Resolved     = 3,   // ok -- evaluatable
    ParseError   = 5,
};

class C_EEExpression {
public:
    // One compiled operand (16 bytes; consumed in order by Push opcodes).
    struct S_Operand {
        enum E_Type : int32_t {
            FloatLiteral     = 0,   // m_value; string args are the float INDEX into engine+0x418
            Call             = 1,   // m_pFunction; args = GetArgCount() prior stack values
            CallContextIndex = 2,   // m_pFunction; +1 leading arg = context-source index
            CallNamedContext = 3,   // m_pFunction; +1 leading arg = string-pool index naming an
                                    //   entity/soul context source
            GlobalVariable   = 4,   // m_id -> engine+0x438 gvar table (sub_180614870)
            LocalVariable    = 5,   // m_id -> engine+0x440 registry, scoped by ctx (sub_1813CF184)
        };
        E_Type   m_type;            // +0x00
        uint32_t _pad04;            // +0x04
        union {                     // +0x08
            float         m_value;      // FloatLiteral
            I_EEFunction* m_pFunction;  // Call / CallContextIndex / CallNamedContext
            uint32_t      m_id;         // GlobalVariable / LocalVariable
        };
    };

    // Opcode stream values (walker sub_1803BC690 dispatch).  1..4 are push variants consuming
    // the next S_Operand; their individual distinction is untraced beyond 2 = push+negate.
    // 11..16 are the comparison family (exact <,<=,>,>=,==,!= assignment UNVERIFIED -- the
    // decompiler drops the setcc); 6 is the tolerance-equality helper sub_1803BD264.
    enum E_Op : int32_t {
        End        = 0,    // terminate; validates the stack sentinel
        Push       = 1,
        PushNeg    = 2,
        Push3      = 3,
        Push4      = 4,
        Negate     = 5,
        FuzzyEq    = 6,
        Add        = 7,
        Sub        = 8,
        Mul        = 9,
        Div        = 10,   // "Division by zero" guard
        CmpFirst   = 11,
        CmpLast    = 16,
        LogicalOr  = 17,
        LogicalAnd = 18,
        LogicalXor = 19,
        Mod        = 20,   // fmodf, "Modulo by zero" guard
    };

    CryStringT<char>     m_debugName;    // +0x00  e.g. "RecipeStep %d %s" (id + UIText)
    CryStringT<char>     m_exprString;   // +0x08  the raw expression text
    E_EEExpressionStatus m_status;       // +0x10  Resolved (3) = evaluatable
    uint32_t             _pad14;         // +0x14
    S_Operand*           m_operands;     // +0x18  operand entries (start of the shared buffer)
    int32_t*             m_opcodes;      // +0x20  opcode stream (follows the operands in-buffer)
    int32_t              m_operandCount; // +0x28
    int32_t              m_opcodeCount;  // +0x2C
    bool                 m_hasError;     // +0x30
    uint8_t              _pad31[7];      // +0x31
};
static_assert(sizeof(C_EEExpression) == 0x38, "C_EEExpression must be 0x38");
static_assert(sizeof(C_EEExpression::S_Operand) == 0x10, "S_Operand must be 0x10");
static_assert(offsetof(C_EEExpression, m_operands) == 0x18, "operands at 0x18");
static_assert(offsetof(C_EEExpression, m_opcodeCount) == 0x2C, "opcode count at 0x2C");

}  // namespace wh::framework
