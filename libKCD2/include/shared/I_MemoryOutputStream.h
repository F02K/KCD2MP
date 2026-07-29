#pragma once
#include "I_SeekableOutputStream.h"

// -----------------------------------------------
// wh::shared::I_MemoryOutputStream -- in-memory output stream (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Intermediate interface (TD only, no own vtable in the plain-TD dump): buffer-access slots
// over I_SeekableOutputStream, no data. Primary base of the two concrete memory streams.

namespace wh::shared {

class I_MemoryOutputStream : public I_SeekableOutputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_MemoryOutputStream;
};
static_assert(sizeof(I_MemoryOutputStream) == 0x08, "interface adds no data");

}  // namespace wh::shared
