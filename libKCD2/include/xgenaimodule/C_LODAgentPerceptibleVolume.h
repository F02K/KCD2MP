#pragma once
#include <cstdint>
#include "C_LODAgentPassiveGrid.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LODAgentPerceptibleVolume : C_LODAgentPassiveGrid :
// C_LODAgent -- the LOD/streaming proxy embedded by value at
// C_PerceptibleVolume+0x90 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x40 (ctor
// sub_180D45D80 writes only through the backptr at +0x38; the host's next field
// starts at volume+0xD0).  Lets far/inactive volumes be culled from active
// perception without destroying them.
// -----------------------------------------------
// RTTI TD rva 0x4F49060; concrete vtable 0x183A98800 (12 slots, same count as
// C_LODAgent -- no new virtuals).  Implements base purecalls [1] GetOwner
// (sub_180602360, returns m_ownerVolume; IDA shows a COMDAT-folded
// __ExceptionPtr name -- NOT an exception fn), [2] sub_180838AE0 stub,
// [4] sub_18041A6A0 return-1, [9] OnLODEvent sub_1811E2830 (dispatch when
// ev==2 via sub_1811E283C).  Overrides [0] dtor sub_1832A8774 (frees base
// +0x18 vector, reverts to C_LODAgent vtbl), [7] GetLODDistanceSq sub_1832A94DC
// (dist^2(pt, ownerVolume->pos) biased by dword_18492F10C), [10] OnDeactivate
// sub_1803ED868 (C_RegularGrid::Remove(globalGrid, GetOwner()) via
// sub_1805CE304), [11] OnActivate sub_180D47000 (grid add of GetOwner()).
// Registered with the process LOD scheduler by the host ctor
// (sub_1803EE368(sub_1823D3CE8(), this, 0) @0x180d45d40).

namespace wh::xgenaimodule {

class C_PerceptibleVolume;

class C_LODAgentPerceptibleVolume : public C_LODAgentPassiveGrid {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LODAgentPerceptibleVolume;
    ~C_LODAgentPerceptibleVolume() override;          // [0]  sub_1832A8774
    void* GetOwner() override;                        // [1]  sub_180602360 -- returns m_ownerVolume
    void _vf2() override;                             // [2]  sub_180838AE0 stub [U role]
    void _vf4() override;                             // [4]  sub_18041A6A0 -- return 1 [U role]
    float GetLODDistanceSq(const Vec3& pt) override;  // [7]  sub_1832A94DC -- dist^2 to owner volume pos, biased
    void OnLODEvent(int ev) override;                 // [9]  sub_1811E2830 -- dispatches on ev==2
    void OnDeactivate() override;                     // [10] sub_1803ED868 -- remove owner from global grid
    void OnActivate() override;                       // [11] sub_180D47000 -- add owner to global grid

    uint64_t             m_gridCellHandle;   // +0x30  init 0x8000000000000000 sentinel (@0x180d45daa)
    C_PerceptibleVolume* m_ownerVolume;      // +0x38  backptr to embedding volume (@0x180d45dc1)
};
static_assert(sizeof(C_LODAgentPerceptibleVolume) == 0x40,
              "C_LODAgentPerceptibleVolume must be 0x40 (volume+0x90..+0xD0)");

}  // namespace wh::xgenaimodule
