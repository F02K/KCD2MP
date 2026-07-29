#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_PositioningManager -- positioning manager interface;
// PRIMARY base of C_PositioningManager (@+0x0) (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_PositioningManager@xgenaimodule@wh@@.  Shape (15 slots)
// read from the C_PositioningManager primary vtable rva 0x3FE8858: [0] dtor
// sub_180B443E0, then getters sub_18101AF00/sub_18101AFB8 and a run of
// sub_1832B79xx/sub_1832BDxxx positioning queries.  Non-dtor slot roles NOT
// decompiled [U].

namespace wh::xgenaimodule {

class I_PositioningManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_PositioningManager;
    virtual ~I_PositioningManager();  // [0]
    virtual void PosVf1()  = 0;   // [1]  [U role]
    virtual void PosVf2()  = 0;   // [2]  [U role]
    virtual void PosVf3()  = 0;   // [3]  [U role]
    virtual void PosVf4()  = 0;   // [4]  [U role]
    virtual void PosVf5()  = 0;   // [5]  [U role]
    virtual void PosVf6()  = 0;   // [6]  [U role]
    virtual void PosVf7()  = 0;   // [7]  [U role]
    virtual void PosVf8()  = 0;   // [8]  [U role]
    virtual void PosVf9()  = 0;   // [9]  [U role]
    virtual void PosVf10() = 0;   // [10] [U role]
    virtual void PosVf11() = 0;   // [11] [U role]
    virtual void PosVf12() = 0;   // [12] [U role]
    virtual void PosVf13() = 0;   // [13] [U role]
    virtual void PosVf14() = 0;   // [14] [U role]
};
static_assert(sizeof(I_PositioningManager) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
