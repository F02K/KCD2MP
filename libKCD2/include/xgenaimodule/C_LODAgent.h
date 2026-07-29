#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::xgenaimodule::C_LODAgent -- abstract LOD agent base (NOT a singleton)
// (KCD2 WHGame.dll 1.5.6, kd7u).  Size folded into the derived classes
// (C_LODAgentNPC TD 0x4F6E490, C_PerceptibleVolume TD 0x4F4C560) -- NO
// static_assert.  Base ctor sub_180BDA08C(this, char initFlag) writes only the
// vptr (+ mem tracking if initFlag & 1).
// -----------------------------------------------
// RTTI TD rva 0x4EFF2D0 (root, numBase=1).  Vtable rva 0x3A82FE8, COL
// 0x40DC5F8 -- 12 slots: [0] dtor sub_183320CB4, [1][2][4][9] _purecall,
// [3]sub_181A74A40, [5][8]sub_180838AE0 (shared body), [6][10][11] nullsub,
// [7]sub_1833220F0.  Member hint from dtor sub_1832A8774: std::vector @+0x18
// (elem stride 0x10); +0x08..+0x18 not attributed [U].

namespace wh::xgenaimodule {

class C_LODAgent {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LODAgent;
    virtual ~C_LODAgent();     // [0]  dtor sub_183320CB4
    virtual void* GetOwner() = 0;                 // [1]  role proven by C_LODAgentPerceptibleVolume impl (returns owner object)
    virtual void _vf2() = 0;   // [2]  [U role]
    virtual void _vf3();       // [3]  sub_181A74A40 [U role]
    virtual void _vf4() = 0;   // [4]  [U role]
    virtual void _vf5();       // [5]  sub_180838AE0 (shared) [U role]
    virtual void _vf6();       // [6]  nullsub [U role]
    virtual float GetLODDistanceSq(const Vec3& pt);  // [7]  base sub_1833220F0 [U base impl]; role proven by PerceptibleVolume override (dist^2 to owner)
    virtual void _vf8();       // [8]  sub_180838AE0 (shared) [U role]
    virtual void OnLODEvent(int ev) = 0;          // [9]  role proven by PerceptibleVolume impl (LOD scheduler event dispatch)
    virtual void OnDeactivate();                  // [10] base nullsub; PerceptibleVolume override removes owner from grid
    virtual void OnActivate();                    // [11] base nullsub; PerceptibleVolume override adds owner to grid

    uint8_t            _unk08;         // +0x08  (init byte 0; flag/enum, bool candidate)
    uint8_t            _pad09[3];      // +0x09  align to +0x0C
    float              m_field0C;      // +0x0C  (init FLT_MAX 0x7F7FFFFF)
    float              m_field10;      // +0x10  (init FLT_MAX 0x7F7FFFFF)
    uint8_t            _unk14;         // +0x14  (init byte 0; flag, bool candidate)
    uint8_t            m_flags15;      // +0x15  (ctor AND 0xFC -> bits 0-1 are bitfield bools)
    uint8_t            _pad16[2];      // +0x16  align to +0x18 (vector)
    std::vector<void*> m_vec18;       // +0x18  elem stride 0x10 in dtor free [U elem type]
};
// abstract; full size folded into derived -- no static_assert.

}  // namespace wh::xgenaimodule
