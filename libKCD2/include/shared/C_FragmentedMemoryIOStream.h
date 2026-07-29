#pragma once
#include <cstdint>
#include "I_InputStream.h"
#include "I_MemoryOutputStream.h"

// -----------------------------------------------
// wh::shared::C_FragmentedMemoryIOStream -- chunked in-memory IO stream (KCD2 1.5.6, kd7u).
// sizeof 0x38.
// -----------------------------------------------
// Dual-vtable sibling of C_MemoryIOStream: output chain @+0x00 vtable 0x183A531A0,
// I_InputStream @+0x08 vtable 0x183A53138. Ctor sub_180794E64(this, chunkSize, totalSize);
// dtor sub_180794EF0. Instead of one contiguous buffer it keeps a heap-allocated chunk
// vector ({begin,end,cap} triple) behind the +0x10 pointer.

namespace wh::shared {

class C_FragmentedMemoryIOStream : public I_MemoryOutputStream, public I_InputStream {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FragmentedMemoryIOStream;
    std::vector<uint8_t*>* m_pChunkVec;  // +0x10  heap-alloc'd {first,last,end} vector of chunk byte-ptrs (ctor sub_180794E64 allocs 24B; dtor sub_180794EF0)
    uint64_t m_chunkSize;   // +0x18  ctor arg2
    uint64_t m_size;        // +0x20  readable byte count / high-water (Read limit 0x180F84B14; GetRemaining 0x181A71B40 = m_size - m_readPos)
    uint64_t m_position;    // +0x28  write/seek cursor (SetPosition 0x181A71B50, GetPosition 0x18113CE50; advanced by Write 0x180B5890C)
    uint64_t m_readPos;     // +0x30  read cursor (advanced by Read 0x180F84B14; GetRemaining 0x181A71B40 = m_size - m_readPos)
};
static_assert(sizeof(C_FragmentedMemoryIOStream) == 0x38, "C_FragmentedMemoryIOStream must be 0x38");

}  // namespace wh::shared
