#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::I_ItemAttachmentListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x08.
// -----------------------------------------------
// Pure interface for item attach/detach notifications (vtable pointer only, no data).
// RTTI .?AVI_ItemAttachmentListener@entitymodule@wh@@  primary vtable 0x183A3C570 (3 slots)
// scalar-deleting dtor 0x1829FD7E4 (frees 8). Standalone leaf; no bases.
//
// [FUNDAMENTAL vs KCD1] The set of implementers changed. In KCD1 C_CombatActor (@+0x08) and
// C_Player (@+0xA00) implemented it; in KCD2 neither does. The KCD2 implementers are
// C_HumanHandHelper (entitymodule, sub-object @+0x08) and C_BattleArchery (playermodule, @+0x70).
// The interface shape (3 pure slots, size 8) is unchanged.
//
// [UNVERIFIED] The KCD1 argument signature is kept, but a KCD2 implementer
// (C_HumanHandHelper::OnItemDetached) dereferences arg2 as an entity POINTER, so the real argument
// shape may differ. Confirm at a call site before relying on the parameter types.

namespace wh::entitymodule {

class I_ItemAttachmentListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ItemAttachmentListener;
    virtual ~I_ItemAttachmentListener() = default;                              // [0]  0x1829FD7E4
    virtual void OnItemAttached(uint64_t entityId, unsigned int handSlot) = 0;  // [1]  (attach; order inferred from KCD1)
    virtual void OnItemDetached(uint64_t entityId, unsigned int handSlot) = 0;  // [2]  (detach)
};
static_assert(sizeof(I_ItemAttachmentListener) == 0x08);

}  // namespace wh::entitymodule
