#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::xgenaimodule::C_SEConditionDatabase -- hand-written situation-condition
// table DB, EMBEDDED at C_SituationManager+0x38 (NOT a template DB, NOT heap
// allocated) (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x68 (embedded extent
// proven: ctor sub_1805FFC9C(mgr+0x38); next manager member at +0xA0).
// -----------------------------------------------
// RTTI TD rva 0x4F52030, COL 0x40DF320 -- the polymorphic part is a 3-slot
// reload-listener vtable with the vptr at DB+0x50 (vtable rva 0x3A40A40:
// [0]sub_180C78760 [1]sub_180C776FC [2]sub_180C7706C).  The RTTI implies a
// non-polymorphic column-handle first base + a poly listener second base, but
// BOTH base identities are UNVERIFIED -- modeled FLAT with a raw vptr member
// at +0x50 (pragmatic escape hatch) [U].  Column handles come from the
// reflection registry (sub_1809155C8()+328, vfunc +96), with registered row
// sizes noted below.

namespace wh::xgenaimodule {

class C_SEConditionDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SEConditionDatabase;

    void*              m_colSituation;              // +0x00  I_DatabaseModule::GetTableMetadata handle "situation" (row stride 72)
    void*              m_colSituationRole;          // +0x08  I_DatabaseModule::GetTableMetadata handle "situation_role" (stride 64)
    void*              m_colSituationVariant;       // +0x10  I_DatabaseModule::GetTableMetadata handle "situation_variant" (stride 48)
    void*              m_colSituationRoleBehavior;  // +0x18  I_DatabaseModule::GetTableMetadata handle "situation_role_behavior" (stride 40)
    void*              m_colSituationFrequency;     // +0x20  I_DatabaseModule::GetTableMetadata handle "situation_frequency" (stride 48)
    void*              m_colSituationGlobalCondition; // +0x28 I_DatabaseModule::GetTableMetadata handle "situation_global_condition" (stride 56)
    void*              m_colSituationRoleCondition; // +0x30  I_DatabaseModule::GetTableMetadata handle "situation_role_condition" (stride 40); also cached @+0x58
    std::vector<void*> m_colHandles;                // +0x38  the 7 column handles
    const void*        m_reloadListenerVptr;        // +0x50  vptr of the class's 2nd (polymorphic) base -- 3-slot query iface over the cached cols (vtable rva 0x3A40A40: [0]sub_180C78760 [1]sub_180C776FC [2]sub_180C7706C); SHOULD be virtual-via-inheritance, base identity [U]
    uint64_t           m_cachedSituationCol;        // +0x58  cached copy of m_colSituation
    void*              m_colSeConditionType;        // +0x60  I_DatabaseModule::GetTableMetadata handle "se_condition_type" (stride 16); 2nd-base data member
};
static_assert(sizeof(C_SEConditionDatabase) == 0x68, "C_SEConditionDatabase must be 0x68 (SituationManager +0x38..+0xA0)");

}  // namespace wh::xgenaimodule
