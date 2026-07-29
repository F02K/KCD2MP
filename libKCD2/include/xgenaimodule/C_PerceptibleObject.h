#pragma once
#include <vector>
#include "C_PerceptionObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptibleObject : C_PerceptionObject -- a perceivable
// entity record (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30 (create-site proven:
// qword_18549D378(48) in the factory sub_1803EB888 else-branch @0x1803eb939;
// created e.g. for world-inventory items by C_PerceptionItemListener OnItemAdded).
// -----------------------------------------------
// RTTI TD rva 0x4F49E80; COL 0x1840E0130; vtable 0x183A332E8, 6 slots (adds
// [2..5]): [0] sub_1819D2600 dtor (body sub_1803ECB44), [1] sub_18066CD10
// (return 0), [2] nullsub_1, [3] nullsub_1, [4] sub_1832A93E4, [5] nullsub_1.
// ctor = inline path of sub_1803EB888 (@0x1803eb95d..0x1803eb98d).  The dtor
// deletes each m_refPoints element via its vslot0 (@0x1803ecb64) -> element
// type proven C_PerceptibleRefPoint*.

namespace wh::xgenaimodule {

class C_PerceptibleRefPoint;

class C_PerceptibleObject : public C_PerceptionObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptibleObject;
    ~C_PerceptibleObject() override;   // [0] sub_1819D2600 (body sub_1803ECB44)
    void _vf1() override;              // [1] sub_18066CD10 -- return 0 [U role]
    virtual void _vf2();               // [2] nullsub_1 [U role]
    virtual void _vf3();               // [3] nullsub_1 [U role]
    virtual void _vf4();               // [4] sub_1832A93E4 [U role]
    virtual void _vf5();               // [5] nullsub_1 [U role]

    std::vector<C_PerceptibleRefPoint*> m_refPoints;   // +0x18  owned observation ref points (dtor deletes elements @0x1803ecb64)
};
static_assert(sizeof(C_PerceptibleObject) == 0x30, "C_PerceptibleObject must be 0x30 (alloc 48)");

}  // namespace wh::xgenaimodule
