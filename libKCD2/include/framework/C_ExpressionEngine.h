#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

// -----------------------------------------------
// wh::framework::C_ExpressionEngine -- expression-engine core (KCD2 1.5.6, utem).
// sizeof 0x478 (alloc(1144) by the bootstrap sub_180C103AC).
// -----------------------------------------------
// [SYNTHETIC NAME] -- non-polymorphic (ctor sub_180C10248 sets no vtable), no RTTI.
// Singleton: ctor tail writes qword_185168B30; the SAME pointer is stored at
// S_GameContext+0x60 by the bootstrap (mov [v1+0x60], engine @0x180C104EC) -- access it
// via S_GameContext::GetInstance()->m_pExpressionEngine (no extra REL::ID needed).
// gvars/locals (+0x438/+0x440) are EXTERNAL objects passed into the ctor
// (qword_185168B38 / +24); the engine does not own them.
//
// The string pool (+0x418) holds the '...' literals of every compiled expression:
// the tokenizer's literal handler sub_180780DF8 interns via sub_180781F10 (backwards
// linear search, CryMalloc copy on miss, returns the ELEMENT INDEX) and emits a
// FloatLiteral operand whose float value is that index (see C_EEExpression::S_Operand).
// Container IS std::vector layout ({first,last,end} POINTER triple: the appender computes
// size as (last-first)>>3 and bumps last by 8 -- not boost's {ptr,count,count}).

namespace wh::framework {

class C_EEFunctionManager;   // 0x38-byte manager @+0x430 (vtable 0x183A86520) [not yet RE'd]

class C_ExpressionEngine {
public:
    // RPN evaluation stack frame (walker sub_1803BC690; top-address helper sub_1803BCC7C).
    // m_freeCount == 256 means empty and it decrements per push (top value = m_pWrite - 1);
    // the End opcode (0) demands m_freeCount == 255 -- exactly the single result left.
    // The wrapper sub_1803BC4D4 targets either the engine's embedded frame or, when one is
    // already active (reentrant eval), a caller-stack local laid out identically.
    struct S_ValueStack {
        float    m_values[256];   // +0x000
        float*   m_pWrite;        // +0x400  next free slot (init = m_values)
        int32_t  m_freeCount;     // +0x408  free slots remaining (256 = empty)
    };
    static_assert(sizeof(S_ValueStack) == 0x410, "S_ValueStack pads to 0x410");

    S_ValueStack*         m_pActiveStack;    // +0x000  null = idle (wrapper saves/restores)
    S_ValueStack          m_stack;           // +0x008  embedded evaluation stack
    std::vector<char*>    m_stringPool;      // +0x418  interned string literals (pool-owned)
    C_EEFunctionManager*  m_pFunctions;      // +0x430  function/constant resolver (owns TRUE/FALSE)
    void*                 m_pGlobalVars;     // +0x438  external gvar table (ctor arg1)
    void*                 m_pLocalVars;      // +0x440  external local-var table (ctor arg2)
    uint8_t               _pad448[0x30];     // +0x448  expression registry (+0x448, nodes
                                             //         self-register) + tail [not yet RE'd]

    // Interned literal by compiled operand index (bounds-checked; eval reads are begin[i]).
    [[nodiscard]] const char* GetPoolString(int32_t index) const
    {
        return (index >= 0 && index < static_cast<int32_t>(m_stringPool.size()))
            ? m_stringPool[index] : nullptr;
    }
};
static_assert(sizeof(C_ExpressionEngine) == 0x478, "C_ExpressionEngine must be 0x478");
static_assert(offsetof(C_ExpressionEngine, m_stack) == 0x008, "embedded stack at 0x08");
static_assert(offsetof(C_ExpressionEngine, m_stringPool) == 0x418, "string pool at 0x418");
static_assert(offsetof(C_ExpressionEngine, m_pFunctions) == 0x430, "function manager at 0x430");

}  // namespace wh::framework
