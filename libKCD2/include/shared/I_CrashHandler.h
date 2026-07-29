#pragma once

// -----------------------------------------------
// wh::shared::I_CrashHandler -- crash-handler callback interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// TD 0x184A42378; no COL with an own vtable resolves in the plain-TD dump (pure interface,
// only implementers carry vtables). Slot set not enumerated -- enumerate from an implementer
// before calling through it.

namespace wh::shared {

class I_CrashHandler {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CrashHandler;
    virtual ~I_CrashHandler();   // [0]; further slots not enumerated
};
static_assert(sizeof(I_CrashHandler) == 0x08, "I_CrashHandler is a vptr-only interface");

}  // namespace wh::shared
