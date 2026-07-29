#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::C_CompanionManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC0 (INFERRED).
// -----------------------------------------------
// RTTI-named (ctor sub_181049CAC). Embedded inside C_Soul's RPG-stats aggregate @aggregate+0x08
// (abs C_Soul+0x4A0). SIZE is inferred from the aggregate tiling (next anchor at aggregate+0xC8),
// NOT from an allocation. Interior NOT walked (honest gap) -- flagged qwords only.

namespace wh::rpgmodule {

class C_CompanionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CompanionManager;
    virtual ~C_CompanionManager() = default;   // slot order UNVERIFIED
    uint64_t m_unk08[23];                      // +0x08..+0xC0  unresolved interior
};
static_assert(sizeof(C_CompanionManager) == 0xC0, "C_CompanionManager must be 0xC0");

}  // namespace wh::rpgmodule
