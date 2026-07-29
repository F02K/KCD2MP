#pragma once
#include "../conceptmodule/C_ModuleBase.h"

// -----------------------------------------------
// wh::questmodule::C_Gameplay -- gameplay concept node (KCD2 1.5.6, kd7u).
// sizeof 0xA0 (alloc 160).
// -----------------------------------------------
// Own vtable 0x183B1DBF0; ctor sub_181163DB0 (memset 0xA0 + C_ModuleBase ctor sub_1806B193C
// + own vftable + addref sub_181E38F20, NO new members -- this allocation is the proof of
// conceptmodule::C_ModuleBase's 0xA0 size); registered via sub_182AEA154.

namespace wh::questmodule {

class C_Gameplay : public conceptmodule::C_ModuleBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Gameplay;
};
static_assert(sizeof(C_Gameplay) == 0xA0, "C_Gameplay adds no data (alloc 160)");

}  // namespace wh::questmodule
