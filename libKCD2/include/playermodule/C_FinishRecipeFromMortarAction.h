#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_FinishRecipeFromMortarAction -- gunpowder finish: bag the milled result
// (KCD2 1.5.6, kd7u).  sizeof 0xA0.
// -----------------------------------------------
// RTTI .?AVC_FinishRecipeFromMortarAction@playermodule@wh@@ (TD 0x184DD4D90); vtable 0x183A5C900;
// ctor sub_1808D3A08.  The IsResultMilled() recipe family (gunpowder/ammo): after distilling, the
// result is milled in the mortar (MortarFinishingModel) and SACKED instead of poured -- plays
// AlchemyGunpowderFinish.  Nine completion hashes cover the sack/pot/mortar/pestle prop swaps.

namespace wh::playermodule {

class C_FinishRecipeFromMortarAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FinishRecipeFromMortarAction;

    uint32_t m_evAttachSack;    // +0x68  ~CRC32("attachsack")
    uint32_t m_evDetachSack;    // +0x6C  ~CRC32("detachsack")
    uint32_t m_evAttachPot;     // +0x70  ~CRC32("attachpot")
    uint32_t m_evDetachPot;     // +0x74  ~CRC32("detachpot")
    uint32_t m_evAttachMortar;  // +0x78  ~CRC32("attachmortar")
    uint32_t m_evDetachMortar;  // +0x7C  ~CRC32("detachmortar")
    uint32_t m_evAttachPestle;  // +0x80  ~CRC32("attachpestle")
    uint32_t m_evDetachPestle;  // +0x84  ~CRC32("detachpestle")
    uint32_t m_evUse;           // +0x88  ~CRC32("use")
    uint32_t _pad8C;            // +0x8C
    uint64_t _pad90;            // +0x90
    void*    m_pIndexer;        // +0x98  item indexer (sub_1808D3DC4)
};
static_assert(sizeof(C_FinishRecipeFromMortarAction) == 0xA0,
              "C_FinishRecipeFromMortarAction must be 0xA0");

}  // namespace wh::playermodule
