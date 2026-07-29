#pragma once
#include <cstdint>
#include <vector>
#include "C_LinkableObject.h"
#include "C_LODAgentPerceptibleVolume.h"
#include "../framework/T_TimeoutTimer.h"
#include "../crysystem/EntityEventListenerWithCleanup.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleVolume : C_LinkableObject + IEntityEventListener
// (@+0x60) -- a perceivable volume in the linkable graph (registered in
// C_PerceptibleVolumeManager's spatial grid); carries the visibility/
// conspicuousness pair the perception side reads.  KCD2 WHGame.dll 1.5.6, kd7u.
// ABSTRACT shape base (slots [19][20][23][24] _purecall).  sizeof 0x110 (base
// ctor sub_180D45C38 fills through the embedded timer +0xE8..+0x110; concrete
// cylinder adds radius@+0x110 with alloc 0x118).
// -----------------------------------------------
// RTTI TD rva 0x4F4C560; CHD numBase 5: C_LinkableObject@0, C_AIObject@0,
// I_RWLocked@+0x20, IEntityEventListener@+0x60.  Primary vtable 0x183FE6CC0,
// 25 slots; I_RWLocked subobject vtable @+0x20 = 0x183FE6CA0 (3x nullsub);
// IEntityEventListener subobject @+0x60, vtable 0x183FE6C88 ([0] dtor thunk
// sub_18213D520, [1] OnEntityEvent sub_1832AF6BC).  IEntityEventListener comes
// from crysystem/EntityEventListenerWithCleanup.h (stock IEntitySystem.h cannot
// compile here) -- do NOT include the stock header in the same TU.
// Spine overrides: [0] deleting dtor sub_1832AE93C (body sub_1811FBF50),
// [1] GetTypeMask sub_181AA5780 (returns dword_18533AEBC), [6] sub_181A75250 =
// GetInformationContainer, returns &m_informationIds (+0xD0).
// Host ctor sub_180D45C38 also creates the paired 16-byte
// C_PerceptibleVolumePuppet (C_AIObject+0x18 = the puppet; not a field here).
// Debug console schema (SpawnPerceptibleVolume, sub_1819C386C):
// pos,radius,height,visibility,conspicuousness,label,timer,worldTime.

namespace wh::rpgmodule { struct T_GameWorldTimePolicy; }

namespace wh::xgenaimodule {

class C_PerceptibleRefPoint;

class C_PerceptibleVolume : public C_LinkableObject, public IEntityEventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleVolume;
    ~C_PerceptibleVolume() override;       // [0]  deleting dtor sub_1832AE93C
    uint32_t GetTypeMask() override;       // [1]  sub_181AA5780
    void* _vf6() override;                 // [6]  sub_181A75250 -- GetInformationContainer: returns &m_informationIds [V]
    // shape interface (impls in C_PerceptibleVolumeCylinder)
    virtual void CollectObservationPoints(std::vector<C_PerceptibleRefPoint*>& out, void* ctx) = 0;  // [19] surface sample points for LOS tests
    virtual void GetBounds(float* outAABB6) = 0;                                                     // [20] writes AABB min/max (6 floats); feeds grid insert @0x180d45f91
    virtual void Save(void* ar);                                                                     // [21] sub_1832AF960 -- streams consp/vis/pos/source-id/flags/ids
    virtual void Load(void* ar);                                                                     // [22] sub_1832AEFEC
    virtual bool IntersectSegment2D(const Vec3* a, const Vec3* b, const Vec3* c, float margin) = 0;  // [23] 2D segment-vs-shape test (arg roles [U])
    virtual bool ContainsPoint2D(const Vec3& pt, float margin) = 0;                                  // [24] 2D point-in-shape test
    // IEntityEventListener impl
    void OnEntityEvent(IEntity* pEntity, SEntityEvent& event) override;   // subobject vt [1] sub_1832AF6BC

    Vec3     m_pos;                // +0x68  world position (ctor @0x180d45ca7; Save 12B)
    float    m_visibility;         // +0x78  perceived-visibility value, init -1.0 sentinel (ctor @0x180d45ccc)
    float    m_conspicuousness;    // +0x7C  conspicuousness value, init -1.0 sentinel (ctor @0x180d45ccf)
    void*    m_source;             // +0x80  registered owner obj; Save streams (*src->vt[2])(src) 8B id; dtor unregisters (pointee type [U])
    uint8_t  m_losCheckEnabled;    // +0x88  gate for the observation-point raycast filter (sub_180D463FC skips if 0)
    uint8_t  m_flag89;             // +0x89  ctor arg a5 [U role]
    uint8_t  _pad8A[6];            // +0x8A  align to +0x90
    C_LODAgentPerceptibleVolume m_lodAgent;   // +0x90  streaming/LOD proxy (ctor sub_180D45D80)
    uint64_t m_informationIds[3];  // +0xD0  embedded id container; Save/Load stream a set<u32>; slot[6] returns &this; dtor sub_180BDB8B8 (exact type [U])
    wh::framework::T_TimeoutTimer<wh::rpgmodule::T_GameWorldTimePolicy> m_timer;  // +0xE8  world-time expiry timer (timer/worldTime schema args)
};
static_assert(sizeof(C_PerceptibleVolume) == 0x110, "C_PerceptibleVolume must be 0x110 (ctor write extent; cylinder alloc 0x118 adds +0x110..+0x118)");

}  // namespace wh::xgenaimodule
