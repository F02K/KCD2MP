#pragma once
#include <cstdint>
#include "C_AIBrain.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIBrainSingleSubb : C_AIBrain -- concrete brain driving ONE
// subbrain (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x108 (create-site proven:
// sub_1804F75C0(264) @0x180419B34 in factory sub_180419AA8, a4!=0 path).
// -----------------------------------------------
// RTTI TD rva 0x4EFDE38; COL rva 0x40E7390; vtable 0x183A37B50, 35 slots (adds
// NONE; overrides 22: 0,5,6,7,10..14,17,21,23,24,26..32 -- fills all 15 pure ->
// CONCRETE).  No standalone ctor: built inline in the factory (base ctor
// sub_180418CF4, then vptr swap + init +0xF8/+0x100).  dtor sub_180413884
// (deletes m_subbrain via its vtbl[1]); deleting dtor = slot [0] sub_180413840.
// Override bodies (g4a_dump_vtables.txt): [5]sub_18041AF88 [6]sub_180419488
// [7]sub_1807F942C [10]sub_18320A028 [11]sub_183209810 [12]sub_183209CDC
// [13]sub_18320A71C [14]sub_181AA36B0 [17]sub_1832096F0 [21]sub_181A834F0
// [23]sub_183209668 [24]nullsub_1 [26]sub_180418C48 [27]sub_180838AE0
// [28]sub_1811D12E8 [29]sub_183209628 [30]sub_18320957C [31]sub_1807F9A84
// [32]sub_1807F8854.

namespace wh::xgenaimodule {

class C_SUBBBase;

class C_AIBrainSingleSubb : public C_AIBrain {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIBrainSingleSubb;
    void _vf7() override;    // [7]  fills pure, sub_1807F942C [U role]
    void _vf12() override;   // [12] sub_183209CDC [U role]
    void _vf13() override;   // [13] sub_18320A71C [U role]
    void _vf14() override;   // [14] sub_181AA36B0 [U role]
    void _vf17() override;   // [17] sub_1832096F0 [U role]
    void _vf21() override;   // [21] sub_181A834F0 [U role]
    void _vf23() override;   // [23] sub_183209668 [U role]
    void _vf24() override;   // [24] nullsub_1
    void _vf26() override;   // [26] sub_180418C48 [U role]
    void _vf27() override;   // [27] sub_180838AE0 (return false)
    void _vf28() override;   // [28] sub_1811D12E8 [U role]
    void _vf29() override;   // [29] sub_183209628 [U role]
    void _vf30() override;   // [30] sub_18320957C [U role]
    void _vf31() override;   // [31] sub_1807F9A84 [U role]
    void _vf32() override;   // [32] sub_1807F8854 [U role]

    C_SUBBBase* m_subbrain;   // +0xF8  the single subbrain (deleted in dtor via vtbl[1]);
                              //        G4A called the pointee "C_AISubbrain" -- corrected to the
                              //        real SUBB family root C_SUBBBase (G5E) [U original name]
    bool        m_flag100;    // +0x100  factory: 0 [U name/meaning]
    uint8_t     _pad101[7];   // +0x101
};
static_assert(sizeof(C_AIBrainSingleSubb) == 0x108, "C_AIBrainSingleSubb must be 0x108 (operator new(264))");

}  // namespace wh::xgenaimodule
