#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// wh::entitymodule::C_AlchemyTable -- the alchemy-table entity component (KCD2 1.5.6, kd7u).
// sizeof 0x48.
// -----------------------------------------------
// RTTI .?AVC_AlchemyTable@entitymodule@wh@@ (TD 0x184B9D868); vtable 0x183BDCB10; ctor
// sub_18163F150 (alloc 72, zeroes +0x08..+0x40, wraps in a holder via sub_18163F1F0); a
// CGameObjectExtensionHelper<C_AlchemyTable, IGameObjectExtension, 0x40> extension of the
// AlchemyTable entity (Scripts/Entities/WH/Minigames/AlchemyTable.lua).
//
// Vtable highlights (validated slots): [3] GetKind -> 7 (sub_181A74280); [5] getter this+0x38
// (sub_180602360); [6] clone thunk (sub_1828C4AD8); [7] AttachLuaComponent (sub_181781F20 /
// sub_181781F64 -- registers the per-entity "alchemyTable" Lua component, methods from
// bind+0x48).  SetGameObject stores the game object at +0x28, entityId at +0x30, entity ptr at
// +0x38, and registers with the table registry (S_GameContext+224 -> +144 -> +32).
// The helper/slot names (alchemyPot/Bellow/Sandglass/...) resolve to SKELETON JOINT indices of
// the table's ICharacterInstance (entity+0x18) via CRC32 (sub_18078BBF8); cache built by
// sub_180D7FC88/sub_180D82BB0 (~32 named resolvers; "AlchemyUse" anchor at cache+464).
//
// ENABLED FLAG RESOLVED (table_enable_and_verbs.md §1): m_disabled at +0x40 (uint32, 0 = enabled).
// Lua alchemyTable:IsEnabled() = sub_1814E37FC: entity -> QueryExtension("AlchemyTable") ->
// returns *(u32)(this+0x40) == 0 (the interaction/use gate).  Writer = C_DisableAlchemyTable
// Activate -> sub_1804F4BB0 generic RTTR property_wrapper::set over its AlchemyTables port (no
// per-class mov -- reflected write).  Remaining fields +0x08..+0x20 are ctor-zeroed only
// [roles UNVERIFIED].

namespace wh::entitymodule {

class C_AlchemyTable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyTable;
    virtual ~C_AlchemyTable() = default;   // slot [0]; see banner for the validated slot map

    uint64_t _unk08;        // +0x08  zeroed by ctor [role UNVERIFIED]
    uint64_t _unk10;        // +0x10  zeroed [role UNVERIFIED]
    uint64_t _unk18;        // +0x18  zeroed [role UNVERIFIED]
    uint64_t _unk20;        // +0x20  zeroed [role UNVERIFIED]
    void*    m_pGameObject; // +0x28  SetGameObject arg
    uint32_t m_entityId;    // +0x30  *(gameObject+8)
    uint32_t _pad34;        // +0x34
    void*    m_pEntity;     // +0x38  IEntity* (also returned by vtable slot [5])
    uint32_t m_disabled;    // +0x40  0 = enabled; set via the RTTR-reflected disable path
    uint32_t _pad44;        // +0x44
};
static_assert(sizeof(C_AlchemyTable) == 0x48, "C_AlchemyTable must be 0x48");
static_assert(offsetof(C_AlchemyTable, m_pGameObject) == 0x28, "game object at 0x28");

}  // namespace wh::entitymodule
