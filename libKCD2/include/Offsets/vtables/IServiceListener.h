#pragma once
#include <cstdint>

// -----------------------------------------------
// Cry::GamePlatform::IServiceListener -- KCD2 binary vtable shape (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AUIServiceListener@GamePlatform@Cry@@.
// CE5-era GamePlatform plugin listener (NEW vs KCD1 -- no GamePlatform module there).
//
// Shape evidence: C_GUIModule's third vtable (subobject +0x18, COL 0x4376A90,
// vtable 0x183EEB798) = 9 slots: [0] sub_181F79670, [1..8] nullsub_1 defaults.
// C_GUIModule::Init (sub_180EE52B8 @0x180ee546c..8c) registers this subobject with the
// platform service resolved from global qword_18492D978 via sub_180D9DBB0 -> vf[+0x90]
// -> sub_181F783C0(service, this+0x18).
//
// Slot NAMES below are UNVERIFIED placeholders (only [0]'s implementation is non-null
// in the C_GUIModule vtable and its exact event identity has not been traced); the
// 9-slot COUNT and all-default bodies are the verified facts. Kept in namespace
// Offsets like the other engine-side vtable replicas; the true namespace is
// Cry::GamePlatform.

namespace Offsets {

struct IServiceListener {
    virtual void _vf0() = 0;   // [0]  0x181F79670 in C_GUIModule (only overridden slot)
    virtual void _vf1() = 0;   // [1]  nullsub default
    virtual void _vf2() = 0;   // [2]  nullsub default
    virtual void _vf3() = 0;   // [3]  nullsub default
    virtual void _vf4() = 0;   // [4]  nullsub default
    virtual void _vf5() = 0;   // [5]  nullsub default
    virtual void _vf6() = 0;   // [6]  nullsub default
    virtual void _vf7() = 0;   // [7]  nullsub default
    virtual void _vf8() = 0;   // [8]  nullsub default
};
static_assert(sizeof(IServiceListener) == 8);

}  // namespace Offsets
