#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleVolumeManager -- spatial registry of perceptible
// volumes, embedded at +0x120 of C_PerceptionManager (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x60 (ctor-proven: sub_180FB7764(PM+0x120), spans PM+0x120..
// +0x180).
// -----------------------------------------------
// RTTI TD rva 0x4F4C7F0; vtable 0x183FE6D98, 2 slots.  ROOT class (CHD numBase
// 1).  Vtable referenced only by ctor/dtor -- the perception side uses the
// manager by direct field access, not virtual dispatch.
// Non-virtual API (entry points, not wired as members here):
//   RegisterVolume  = sub_180D45EB4(mgr, vol): push vol into m_volumes; wrap in
//     a 40-byte C_RegularGridObjectWrapper<C_PerceptibleVolume,
//     C_RegularGridCellDummy> {vtbl @0x180d45f41, [1]=vol}; push into
//     m_wrappers; vol->GetBounds() (@0x180d45f91) -> C_RegularGrid::Insert
//     (sub_1805CE350 @0x180d45fa6).  Callers = the 6 volume spawners.
//   FindVolumeByWuid = sub_180D45BA4(mgr, &wuid): linear scan of m_volumes
//     matching *(vol+8) == wuid (11 callers; THE query gameplay/perception uses).
//   Spatial enumeration goes through m_grid (Insert sub_1805CE350 / Remove
//     sub_1805CE304); the by-radius grid-walk entry not isolated [UNVERIFIED].

namespace wh::shared { template <typename T> class C_RegularGrid; class C_RegularGridCellDummy; class C_RegularGridObjectWrapperBase; }

namespace wh::xgenaimodule {

class C_PerceptibleVolume;

class C_PerceptibleVolumeManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleVolumeManager;
    virtual ~C_PerceptibleVolumeManager();   // [0] sub_1832AE970 (deleting dtor -> body sub_1832AE1FC)
    virtual C_PerceptibleVolume* FindVolumeOfType(const void* wuid);  // [1] sub_1832AEFA0 -- FindVolumeByWuid + dynamic-type check vs unk_183FE6D88

    wh::shared::C_RegularGrid<wh::shared::C_RegularGridCellDummy*>* m_grid;  // +0x08  40x40 spatial hash (ctor sub_180FB7E5C, alloc 376), heap-owned [V]
    std::vector<C_PerceptibleVolume*> m_volumes;   // +0x10  all registered volumes (insert push @0x180d45ee5; FindVolumeByWuid scans this)
    std::vector<wh::shared::C_RegularGridObjectWrapperBase*> m_wrappers;  // +0x28  40-byte grid wrappers, parallel to m_volumes (insert push @0x180d45f6d)
    uint64_t           m_field40;  // +0x40  standalone qword; ctor 0, NOT freed by dtor [U role]
    std::vector<void*> m_vec48;    // +0x48  std::vector<8B-elem> (dtor frees a1[9]); element type [U]
};
static_assert(sizeof(C_PerceptibleVolumeManager) == 0x60, "C_PerceptibleVolumeManager must be 0x60 (PM+0x120..+0x180)");

}  // namespace wh::xgenaimodule
