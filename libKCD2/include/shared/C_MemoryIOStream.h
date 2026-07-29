#pragma once
#include <cstdint>
#include "I_InputStream.h"
#include "I_MemoryOutputStream.h"

// -----------------------------------------------
// wh::shared::C_MemoryIOStream -- contiguous in-memory IO stream (KCD2 1.5.6, kd7u).
// sizeof 0x38.
// -----------------------------------------------
// Dual-vtable: output chain (I_MemoryOutputStream) @+0x00 vtable 0x183A530F0, I_InputStream
// @+0x08 vtable 0x183A530B0. dtor sub_180794FB4 frees the +0x10 buffer via the shared-heap
// free thunk qword_18549D358. sizeof 0x38 VERIFIED from the stack instance in sub_1825BD378
// (_QWORD v17[7]) and the by-value embedding in framework::C_CryOutputSerializer @+0x10.
// The three size-like qwords are individually UNPINNED among {size, capacity, position} --
// pin from a write-slot decompile before relying on any single one.

namespace wh::shared {

class C_MemoryIOStream : public I_MemoryOutputStream, public I_InputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MemoryIOStream;
    uint8_t* m_pBuffer;    // +0x10  heap buffer (freed via qword_18549D358)
    uint64_t m_size;       // +0x18  [size/cap/pos split UNPINNED -- see header note]
    uint64_t m_capacity;   // +0x20  [ditto]
    uint64_t m_position;   // +0x28  [ditto]
    uint64_t m_readPos;    // +0x30  read cursor (Read 0x180793170 via input thunk 0x182138710; GetRemaining 0x181A71B40 = size - m_readPos)
};
static_assert(sizeof(C_MemoryIOStream) == 0x38, "C_MemoryIOStream must be 0x38 (stack instance sub_1825BD378)");

}  // namespace wh::shared
