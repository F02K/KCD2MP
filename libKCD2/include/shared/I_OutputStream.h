#pragma once

// -----------------------------------------------
// wh::shared::I_OutputStream -- byte-output stream interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183A530D0. Root of the output chain I_OutputStream -> I_SeekableOutputStream
// -> I_MemoryOutputStream implemented by the memory streams. Write-slot set not enumerated.

namespace wh::shared {

class I_OutputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_OutputStream;
    virtual ~I_OutputStream();   // [0]; further slots not enumerated
};
static_assert(sizeof(I_OutputStream) == 0x08, "I_OutputStream is a vptr-only interface");

}  // namespace wh::shared
