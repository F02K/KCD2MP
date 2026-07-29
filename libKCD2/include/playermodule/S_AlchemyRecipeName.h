#pragma once
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::S_AlchemyRecipeName -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI .?AUS_AlchemyRecipeName@playermodule@wh@@ (TD 0x184DCE1D0).  A strong-typedef wrapper for
// a recipe name string: RTTR registers bidirectional type_converters S_AlchemyRecipeName <->
// CryStringT<char>, and a reflected bool(I_Soul*, S_AlchemyRecipeName) method uses it as the
// "has learned recipe" parameter type.
// [Layout: single CryStringT member INFERRED from the converter pair -- no other field appears in
// any converter/registration; UNVERIFIED against a ctor.]

namespace wh::playermodule {

struct S_AlchemyRecipeName {
    CryStringT<char> m_name;   // +0x00
};
static_assert(sizeof(S_AlchemyRecipeName) == 0x08, "S_AlchemyRecipeName is one CryStringT");

}  // namespace wh::playermodule
