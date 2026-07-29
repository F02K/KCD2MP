#pragma once
#include <cstdint>

// -----------------------------------------------
// IStreamable -- streamable-resource base, KCD2 binary slot order + data layout
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct (RTTI .?AUIStreamable@@), second base of IStatObj (mdisp +0x08 inside CStatObj).
// Standalone default vtable 0x183A2F788 = exactly 7 slots (dtor + 6 _purecall). Carries
// 0x20 bytes of own data (CStatObj-relative 0x10..0x2F): only the streaming-state byte is
// behaviourally resolved; the rest is zero-initialised by the ctor with no observed
// reader/writer in the ~110-function CStatObj corpus (likely touched by CObjManager's
// streaming manager, which holds a pointer to THIS subobject, not to CStatObj --
// sub_180611A88(objmgr, this+8) @0x1806119E8).
// Slot names from CStatObj impl behaviour; evidence: cstatobj.md §3.

namespace Offsets {

struct IStreamable {
    virtual ~IStreamable() = default;                                    // [0] impls are adjustor thunks into the owner dtor
    virtual void StartStreaming(bool bFinishNow, void** ppStream) = 0;   // [1] issues the read on m_szFileName, registers the owner's IStreamCallback, state->1 (0x1806B6DB0)
    virtual int GetStreamableContentMemoryUsage(bool bJustForDebug) = 0; // [2] cached per-slot sizes (owner +0x188/+0x18C/+0x190)
    virtual void ReleaseStreamableContent() = 0;                         // [3] drops render meshes across LODs/subobjects, state->0 (0x18126DC08)
    virtual void GetStreamableName(CryStringT<char>& sName) = 0;         // [4] "<filepath> - <geomname>" (0x181202810)
    virtual uint32_t GetLastDrawMainFrameId() = 0;                       // [5] owner +0x54
    virtual bool IsUnloadable() = 0;                                     // [6] owner +0x154

    uint64_t m_unk08 = 0;             // +0x08  zero-init; no observed access
    uint64_t m_unk10 = 0;             // +0x10  zero-init; no observed access
    uint64_t m_unk18 = 0;             // +0x18  zero-init; no observed access
    uint32_t m_unk20 = 0;             // +0x20  zero-init; no observed access
    uint8_t  m_eStreamingStatus = 0;  // +0x24  0 = not loaded, 1 = read in flight, 2 = loaded (writers 0x1806B6F69/0x18105DFFE/0x18126DEAF)
    uint8_t  m_unk25 = 0;             // +0x25  zero-init; no observed access
    // +0x26..0x27 tail padding -> the next vptr in CStatObj lands at +0x30
};
static_assert(sizeof(IStreamable) == 0x28, "vptr + 0x20 data (puts CStatObj's IStreamCallback at +0x30)");

}  // namespace Offsets
