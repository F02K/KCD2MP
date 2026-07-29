#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_LinkableObjectExtension -- the IGameObjectExtension that
// attaches a linkable to a game object/entity (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof NOT create-site proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F46AA0; primary vtable 0x183A651E8, 29 slots.  Real base chain
// (RTTI CHD): CGameObjectExtensionHelper<C_LinkableObjectExtension,
// IGameObjectExtension, 64> -> IGameObjectExtension -> IComponent @+0, with
// std::enable_shared_from_this<IComponent> @+0x08.  Modeled FLAT here: the stock
// CryCommon IGameObject.h/IComponent.h cannot compile in this build env (boost
// dependency; same policy as crysystem/IEntitySystemSink.h), and IGameObjectExtension
// is interfuscator-shuffled, so slot indices below are BINARY VTABLE ORDER, not SDK
// declaration order -- layout mirror only, do not call by declaration index.
// enable_shared_from_this base is mirrored as a 2-qword member @+0x08 (weak_ptr
// {ctrl, ptr}).

namespace wh::xgenaimodule {

class C_LinkableObjectExtension {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkableObjectExtension;
    virtual ~C_LinkableObjectExtension();  // [0]  deleting dtor sub_18096D3F0
    virtual void _vf1();                   // [1]  nullsub_1
    virtual void _vf2();                   // [2]  sub_180838AE0 (shared stub)
    virtual void _vf3();                   // [3]  sub_181A74280
    virtual void _vf4();                   // [4]  nullsub_1
    virtual void _vf5();                   // [5]  sub_180602360
    virtual void _vf6();                   // [6]  nullsub_1
    virtual void _vf7();                   // [7]  sub_18096E264
    virtual void _vf8();                   // [8]  nullsub_1
    virtual void _vf9();                   // [9]  nullsub_1
    virtual void _vf10();                  // [10] nullsub_1
    virtual void _vf11();                  // [11] sub_1832F36CC
    virtual void _vf12();                  // [12] nullsub_1
    virtual void _vf13();                  // [13] sub_1832F3410
    virtual void _vf14();                  // [14] sub_18096D5FC
    virtual void _vf15();                  // [15] nullsub_1
    virtual void _vf16();                  // [16] sub_180838AE0
    virtual void _vf17();                  // [17] sub_180838AE0
    virtual void _vf18();                  // [18] sub_181A72600
    virtual void _vf19();                  // [19] nullsub_1
    virtual void _vf20();                  // [20] nullsub_1
    virtual void _vf21();                  // [21] sub_1823CA690
    virtual void _vf22();                  // [22] nullsub_1
    virtual void _vf23();                  // [23] nullsub_1
    virtual void _vf24();                  // [24] nullsub_1
    virtual void _vf25();                  // [25] nullsub_1
    virtual void _vf26();                  // [26] sub_181AA49F0
    virtual void _vf27();                  // [27] nullsub_1
    virtual void _vf28();                  // [28] nullsub_1

    uint64_t m_weakSelf[2];   // +0x08  std::enable_shared_from_this<IComponent> base mirrored flat (weak_ptr {ptr, ctrl}) [layout mirror]
};
// extension data past +0x18 + sizeof unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
