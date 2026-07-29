#pragma once
#include "../shared/C_TransformManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AITransformManager : shared::C_TransformManager<C_AIObject>
// -- AI-object transform manager SINGLETON at qword_1854960C0 (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x98 (tracked getter sub_1834509C4 @0x183450a5c:
// qword_18549D378(152,...), += 0x98; untracked variant sub_18197B554:
// sub_1804F75C0(152); in-place ctor sub_181177E7C).  Both getters register a
// functor (sub_180C5BD8C/sub_180874FC4, cleanup sub_18041C37C) with a
// subsystem.
// -----------------------------------------------
// RTTI TD rva 0x4FFB4C0.  Vtable rva 0x3B20CC0 (1 slot: [0] dtor
// sub_1803EC188) -- nearly non-virtual beyond the dtor.  Adds NO own data
// (all fields live in the flat-modeled template base).

namespace wh::xgenaimodule {

class C_AIObject;

class C_AITransformManager : public shared::C_TransformManager<C_AIObject> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AITransformManager;
    ~C_AITransformManager() override;   // [0] sub_1803EC188
};
static_assert(sizeof(C_AITransformManager) == 0x98, "must be 0x98 (alloc 152 at sub_1834509C4)");

}  // namespace wh::xgenaimodule
