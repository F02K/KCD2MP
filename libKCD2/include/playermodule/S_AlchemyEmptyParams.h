#pragma once

// -----------------------------------------------
// wh::playermodule::S_EmptyParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  Empty action-param block.
// -----------------------------------------------
// The no-payload TParams of C_AlchemyActionBase<S_EmptyParams, ...> (RTTI template arg
// US_EmptyParams@playermodule; POD, no own TypeDescriptor).  As a C_ParamAction member it still
// occupies the +0x50 slot (1 byte, padded to 8 by the following C_Alchemy* alignment), matching
// the binary: S_EmptyParams leaves hold m_pAlchemy at +0x58.
// FILE NAME NOTE: header named S_AlchemyEmptyParams.h to avoid clashing with the distinct
// wh::entitymodule S_EmptyParams (RTTI .?AUS_EmptyParams@entitymodule@wh@@); the CLASS name
// matches the binary (wh::playermodule::S_EmptyParams).

namespace wh::playermodule {

struct S_EmptyParams {
};
static_assert(sizeof(S_EmptyParams) == 1, "empty params (padded to 8 inside the action)");

}  // namespace wh::playermodule
