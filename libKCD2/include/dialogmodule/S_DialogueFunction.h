#pragma once
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::S_DialogueFunction -- "dialogue_functions" table row
// (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// Row/element of C_DialogueFunctionsDatabase (stored by value in the base +0x30 rows
// vector). From ParseRows sub_1815B07C0: two CryStrings -- the named function key and
// its expression body (compiled into C_DialogFunctionExpression instances).

namespace wh::dialogmodule {

struct S_DialogueFunction {
    CryStringT<char> m_name;         // +0x00  function name (key)
    CryStringT<char> m_expression;   // +0x08  expression source
};
static_assert(sizeof(S_DialogueFunction) == 0x10, "S_DialogueFunction must be 0x10");

}  // namespace wh::dialogmodule
