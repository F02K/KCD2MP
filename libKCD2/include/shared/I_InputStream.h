#pragma once

// -----------------------------------------------
// wh::shared::I_InputStream -- byte-input stream interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183A48768. Second base of C_MemoryIOStream / C_FragmentedMemoryIOStream
// (subobject @+0x08). Read-slot set not enumerated -- do not call through this interface yet.

namespace wh::shared {

class I_InputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InputStream;
    virtual ~I_InputStream();   // [0]; further slots not enumerated
};
static_assert(sizeof(I_InputStream) == 0x08, "I_InputStream is a vptr-only interface");

}  // namespace wh::shared
