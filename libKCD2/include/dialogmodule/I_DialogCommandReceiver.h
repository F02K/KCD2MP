#pragma once

// -----------------------------------------------
// wh::dialogmodule::I_DialogCommandReceiver -- command-execution sink (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_DialogCommandReceiver@dialogmodule@wh@@ (TD 0x184B4B7F0, abstract, no COL).
// Implemented by C_DialogInstance @+0x10 (vtable 0x183E63B00, 11 slots; `this` = full+0x10,
// full+328 = scene, full+688 = service). The callbacks data::C_*Command handlers invoke to
// perform playback actions. Observed impl bodies (names UNVERIFIED):
//   [1] sub_1827FD564  resolve + dispatch node/port
//   [2] sub_1827FD088  resolve actor, then execute an actor-targeted command
//   [5] sub_1814B2C3C  build play/anim command
//   [3]/[4]/[6]/[7]/[8]/[9]/[10]  camera/prop/script/show-actor executors
// Mirrored for layout/slot count only.

namespace wh::dialogmodule {

class I_DialogCommandReceiver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DialogCommandReceiver;
    virtual ~I_DialogCommandReceiver();   // [0]
    virtual void _vf1() = 0;              // [1] resolve+dispatch node/port
    virtual void _vf2() = 0;              // [2] resolve actor + execute command
    virtual void _vf3() = 0;              // [3]
    virtual void _vf4() = 0;              // [4]
    virtual void _vf5() = 0;              // [5] build play/anim command
    virtual void _vf6() = 0;              // [6]
    virtual void _vf7() = 0;              // [7]
    virtual void _vf8() = 0;              // [8]
    virtual void _vf9() = 0;              // [9]
    virtual void _vf10() = 0;             // [10]
};
static_assert(sizeof(I_DialogCommandReceiver) == 0x08);

}  // namespace wh::dialogmodule
