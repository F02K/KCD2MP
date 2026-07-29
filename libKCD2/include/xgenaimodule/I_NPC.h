#pragma once
#include <cstdint>
#include "C_IntelligentObject.h"
#include "I_XGenAINPC.h"

// -----------------------------------------------
// wh::xgenaimodule::I_NPC : C_IntelligentObject + wh::I_XGenAINPC(@+0xA8) -- the
// abstract NPC contract level of the spine (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xB0 (C_IntelligentObject 0xA8 + I_XGenAINPC subobject @+0xA8).
// -----------------------------------------------
// RTTI TD rva 0x4F6E3D0; primary vtable 0x183A83228, 64 slots -- adds [27..63], ALL
// _purecall at this level (the NPC-specific virtual contract; C_NPC implements them).
// Subobject vtables: I_RWLocked @+0x20 -> 0x183A83680, I_XGenAINPC @+0xA8 ->
// 0x183A83668. The "I_" prefix is misleading: RTTI is unambiguous that the concrete
// C_IntelligentObject is a BASE of I_NPC (its CHD lists the whole spine at mdisp 0).
// Real ctor sub_180BDCBF0(this, const WUID*, I_AIPuppet*): chains the spine ctors and
// writes the vptr triple +0x00/+0x20/+0xA8; NO own data members.

namespace wh::xgenaimodule {

class I_NPC : public C_IntelligentObject, public ::wh::I_XGenAINPC {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_NPC;
    ~I_NPC() override;                // [0] deleting dtor sub_183320E08
    uint32_t GetTypeMask() override;  // [1] sub_181AA6B70
    // [27..63] the NPC contract -- all _purecall here; impl addresses on C_NPC.
    virtual void _vf27() = 0;
    virtual void _vf28() = 0;
    virtual void _vf29() = 0;
    virtual void _vf30() = 0;
    virtual void _vf31() = 0;
    virtual void _vf32() = 0;
    virtual void _vf33() = 0;
    virtual void _vf34() = 0;
    virtual void _vf35() = 0;
    virtual void _vf36() = 0;
    virtual void _vf37() = 0;
    virtual void _vf38() = 0;
    virtual void _vf39() = 0;
    virtual void _vf40() = 0;
    virtual void _vf41() = 0;
    virtual void _vf42() = 0;
    virtual void _vf43() = 0;
    virtual void _vf44() = 0;
    virtual void _vf45() = 0;
    virtual void _vf46() = 0;
    virtual void _vf47() = 0;
    virtual void _vf48() = 0;
    virtual void _vf49() = 0;
    virtual void _vf50() = 0;
    virtual void _vf51() = 0;
    virtual void _vf52() = 0;
    virtual void _vf53() = 0;
    virtual void _vf54() = 0;
    virtual void _vf55() = 0;
    virtual void _vf56() = 0;
    virtual void _vf57() = 0;
    virtual void _vf58() = 0;
    virtual void _vf59() = 0;
    virtual void _vf60() = 0;
    virtual void _vf61() = 0;
    virtual void _vf62() = 0;
    virtual void _vf63() = 0;
};
static_assert(sizeof(I_NPC) == 0xB0, "I_NPC must be 0xB0 (C_IntelligentObject 0xA8 + I_XGenAINPC @+0xA8)");

}  // namespace wh::xgenaimodule
