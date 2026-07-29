#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptionObject -- root of the perception-object hierarchy
// (perceptor/perceptible records tracked by C_PerceptionManager).  KCD2 WHGame.dll
// 1.5.6, kd7u.  Abstract; base subobject size 0x18 (every derived ctor writes the
// {+8 source, +16 handle} pair while the base vtable is installed; no standalone
// base ctor exists, so the base-vs-derived boundary of +8/+16 is modeled, not
// ctor-proven [U]).
// -----------------------------------------------
// RTTI TD rva 0x4F49E40; COL 0x1840DFC80; vtable 0x183A332A8, 2 slots:
// [0] sub_1832AE9D0 scalar-deleting dtor, [1] _purecall.  ROOT class (CHD
// numBase 1).  Derived: C_PerceptibleObject (-> C_StatePerceptibleObject) and
// C_PerceptorObject -- all single inheritance, all subobjects at mdisp 0.

namespace wh::xgenaimodule {

class C_PerceptionObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptionObject;
    virtual ~C_PerceptionObject();   // [0] sub_1832AE9D0
    virtual void _vf1() = 0;         // [1] _purecall here [U role]

    void*    m_source;        // +0x08  owner/source object (every derived ctor: = a2)
    uint64_t m_sourceHandle;  // +0x10  handle/id copied from the source (= *(a2+8))
};
static_assert(sizeof(C_PerceptionObject) == 0x18, "C_PerceptionObject base subobject is 0x18");

}  // namespace wh::xgenaimodule
