#pragma once
#include <cstdint>
#include <memory>
#include "C_FactionBase.h"
#include "C_Nervousness.h"

// -----------------------------------------------
// wh::rpgmodule::C_NPCFactionNode : C_FactionBase -- the per-NPC faction
// membership node (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xB8 (make_shared ctor
// sub_1803F21DC write extent: control block @a1, node @a1+0x10; last member =
// embedded C_Nervousness +0x90..+0xB8) [UNPROVEN exact tail].
// Always heap-allocated via std::make_shared (_Ref_count_obj2 control block);
// C_Faction "Souls" holds vector<weak_ptr<C_NPCFactionNode>>.
// -----------------------------------------------
// RTTI TD rva 0x4D4E540; COL 0x18412A410; node vtable 0x3A33630 (not fully
// walked -- out of stealth scope); _Ref_count_obj2 ctrl vtable 0x3A33450.
// CHD bases: C_FactionBase@0 (0x78, header exists) + std::enable_shared_from_
// this<C_NPCFactionNode>@0x78.  RTTR: CryStringT<char> read-only getter
// (property_wrapper 0x183AFABE8); shared_ptr converters C_FactionBase <->
// C_NPCFactionNode (0x183AFAB98/0x183AFABC0).

namespace wh::rpgmodule {

class C_NPCFactionNode : public C_FactionBase,
                         public std::enable_shared_from_this<C_NPCFactionNode> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NPCFactionNode;

    uint64_t      _unk88;         // +0x88  ctor writes rdx [U role] (@0x1803f2230)
    C_Nervousness m_nervousness;  // +0x90  embedded nervousness value (0x28, init @0x1803f224e)
};
static_assert(sizeof(C_NPCFactionNode) == 0xB8, "C_NPCFactionNode must be 0xB8 (ctor write extent)");

}  // namespace wh::rpgmodule
