#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_MessageDispatcher -- AI message dispatcher contract (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F5EC68; standalone vtable 0x183FEE240, 3 slots:
// [0] dtor sub_1832F56C0, [1] nullsub_1, [2] _purecall (the Dispatch contract).

namespace wh::xgenaimodule {

class I_MessageDispatcher {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_MessageDispatcher;
    virtual ~I_MessageDispatcher();          // [0] sub_1832F56C0
    virtual void _vf1();                     // [1] nullsub_1 [U role]
    virtual bool Dispatch(void* msg) = 0;    // [2] _purecall; impl returns (result != 3) [U exact signature]
};
static_assert(sizeof(I_MessageDispatcher) == 0x08, "I_MessageDispatcher is vptr-only");

}  // namespace wh::xgenaimodule
