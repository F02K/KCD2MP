#pragma once
#include "I_OutputStream.h"

// -----------------------------------------------
// wh::shared::I_SeekableOutputStream -- seekable output stream (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Intermediate interface (TD only, no own vtable in the plain-TD dump): adds seek slots to
// I_OutputStream, no data. Slot set not enumerated.

namespace wh::shared {

class I_SeekableOutputStream : public I_OutputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SeekableOutputStream;
};
static_assert(sizeof(I_SeekableOutputStream) == 0x08, "interface adds no data");

}  // namespace wh::shared
