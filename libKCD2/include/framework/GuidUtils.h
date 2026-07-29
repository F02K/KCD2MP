#pragma once
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh GUID text utilities (KCD2 1.5.6, utem).
// -----------------------------------------------
// ParseGuid = sub_180719B1C: sscanf("%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx")
// with the out-pointers aimed at out+0(u32)/+4(u16)/+6(u16)/+8..15(u8x8) -- i.e. the
// WINDOWS-GUID field layout, NOT hipart=<first 16 hex digits> packing (the numeric
// hipart/lopart of the result do not equal the textual halves).  Accepts an optional
// {}-wrapper; the literal "0" parses as the null guid.  Out is zero-initialized first,
// so failure and "0" both leave {0,0}.  This is the game's own parser (the alchemy DSL
// functors run it on condition string args) -- use it whenever the result must compare
// equal to game-parsed GUIDs (item ids, recipe rows, brew records).

namespace wh {

// Parse "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx" into the game's GUID byte layout.
bool ParseGuid(const char* text, CryGUID& out);

}  // namespace wh
