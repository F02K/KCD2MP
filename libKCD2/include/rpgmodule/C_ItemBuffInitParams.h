#pragma once
#include <cstdint>
#include <cstddef>
#include "C_BuffInitParamsDefault.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemBuffInitParams : C_BuffInitParamsDefault -- KCD2 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI .?AVC_ItemBuffInitParams@rpgmodule@wh@@  make sub_182CDE7AC (alloc 16; vtable +
// `v5[1] = *a2` -- copies exactly one 8-byte param). Carries the source-item identity into
// item-driven buff implementations (on-equip / consumed-item buffs). The 8B payload is very
// likely the item WUID (tag 2) but that was NOT traced -- typed as raw uint64_t (INFERRED).
// Subclass: C_ItemHealthCheckingBuffInitParams (adds fields; layout not measured).

namespace wh::rpgmodule {

class C_ItemBuffInitParams : public C_BuffInitParamsDefault {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemBuffInitParams;
    uint64_t m_itemParam;   // +0x08  source-item id (likely item WUID -- UNVERIFIED)
};
static_assert(sizeof(C_ItemBuffInitParams) == 0x10, "C_ItemBuffInitParams must be 0x10");
static_assert(offsetof(C_ItemBuffInitParams, m_itemParam) == 0x08, "param at 0x08");

}  // namespace wh::rpgmodule
