#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::E_DocumentViewMode -- KCD2 WHGame.dll 1.5.6 (utem).  Enum wrapper,
// Type : uint8 (the C_DocumentItem field is one byte).
// -----------------------------------------------
// [SYNTHETIC NAME] -- the C_DocumentItem+0x170 byte.  Written ONLY at book open/bind
// (0x180A96A64 / 0x180A96C3C: copied from the document's gfx-asset registry row byte +0x40,
// looked up by the entity's "GfxName" script property in the registry @0x1853257F0) and
// zeroed by the ctor -- i.e. it is the document's UI VIEW KIND, static per asset, not a
// per-page state (page state = C_DocumentItem::m_herbViewActive + m_lastOpenRecipeId).
// Only the alchemy recipe-book value is enumerated: the GetOpenRecipeId/autocook gate
// (0x1808C3C68) requires == 2.  Other values are asset-driven and unenumerated.

namespace wh::entitymodule {

struct E_DocumentViewMode {
    enum Type : uint8_t {
        RecipeDetail = 2,   // alchemy recipe book detail view (the autocook gate value)
    };
};
static_assert(sizeof(E_DocumentViewMode::Type) == 1, "stored as one byte at C_DocumentItem+0x170");

}  // namespace wh::entitymodule
