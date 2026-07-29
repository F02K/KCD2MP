#pragma once
#include <cstdint>
#include <cstddef>
#include "../Offsets/vtables/IItem.h"
#include "../framework/CryDeferrable.h"

// -----------------------------------------------
// wh::entitymodule::C_PickableItem -- the base framework item extension (KCD2 WHGame.dll 1.5.6,
// utem).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_PickableItem@entitymodule@wh@@; vftables 0x183B466F0 (@+0x00, the IItem lineage via
// CGameObjectExtensionHelper<C_PickableItem, IItem>) and 0x183B466B8 (@+0x40, coloff 0x40 =
// IGameObjectProfileManager).  UnsafeOp::CryDeferrable<0> is an EMPTY base at +0x48 (zero-size
// here: preceded by a non-empty base, followed by data -- same MSVC folding as C_Actor's; the
// ctor writes NO vptr for it, and the pointer stored at +0x48 targets an UNINITIALIZED static
// block, not a vftable).  Base of C_DocumentItem (and the simple pickable world items); one of
// exactly THREE CItemCreator-registered item classes.  Ctor sub_1803F08EC pins every offset
// below (it inlines the extension-chain zero-init that C_Actor gets from sub_181354AB4);
// members not written there are runtime state.

namespace wh::entitymodule {

class C_PickableItem
    : public Offsets::IItem                        // +0x00  vptr + extension storage (0x40)
    , public Offsets::IGameObjectProfileManager    // +0x40  vptr
    , public UnsafeOp::CryDeferrable<0> {          // +0x48  empty (zero-size, see banner)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickableItem;

    void*    _unk48;       // +0x48  ctor = &unk_185667118 (static BSS block) [role unresolved]
    int16_t  _unk50;       // +0x50  ctor -1
    int16_t  _unk52;       // +0x52  ctor -1
    uint32_t _pad54;       // +0x54
    void*    _unk58;       // +0x58  ctor 0
    uint64_t _unk60;       // +0x60  ctor 16 [role unresolved]
    uint64_t _unk68;       // +0x68  ctor 0
    uint64_t _unk70;       // +0x70  ctor 2 [role unresolved]
    uint64_t _unk78;       // +0x78  ctor 1 [role unresolved]
    uint8_t  _unk80[0x38]; // +0x80  not touched by the ctor (runtime state)
    uint64_t _unkB8;       // +0xB8  ctor 0
    uint32_t _unkC0;       // +0xC0  ctor 0
    uint32_t _padC4;       // +0xC4
    void*    _unkC8;       // +0xC8  ctor = TLS-guarded static (empty-string block)
    uint64_t _unkD0;       // +0xD0  ctor 0
};
static_assert(sizeof(C_PickableItem) == 0xD8, "C_PickableItem must be 0xD8 (ctor 0x1803F08EC)");
static_assert(offsetof(C_PickableItem, _unk48) == 0x48, "own data starts at 0x48");

}  // namespace wh::entitymodule
