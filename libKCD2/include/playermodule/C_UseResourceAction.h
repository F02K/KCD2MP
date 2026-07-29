#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_UseResourceAction -- pour a base liquid into the cauldron (KCD2 1.5.6, kd7u).
// sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_UseResourceAction@playermodule@wh@@ (TD 0x184DD5280); vtable 0x183A5C670; ctor
// sub_1808D381C; factory sub_1808D3770 for verbs 13..16 (gate: pot bucket empty).  Which liquid is
// NOT a param: the anim-event dispatch sub_1806C29D8, on the "Use" event, reads the LAST VERB from
// C_Alchemy+0x304 (13 Spiritus / 14 Oil / 15 Wine / 16 Water) and builds the base
// C_AlchemyResource via sub_1806C2B08(rec, verb) -> AddResource(kind 6).  "Attach"/"Detach"
// hand-attach/detach the carafe prop to entity_left.  Plays AlchemyPutBasis.

namespace wh::playermodule {

class C_UseResourceAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UseResourceAction;

    uint32_t m_evAttach;   // +0x68  ~CRC32("attach")
    uint32_t m_evDetach;   // +0x6C  ~CRC32("detach")
    uint32_t m_evUse;      // +0x70  ~CRC32("use") -> creates the base C_AlchemyResource
    uint32_t _pad74;       // +0x74
    uint64_t _pad78;       // +0x78  tail [contents UNVERIFIED]
};
static_assert(sizeof(C_UseResourceAction) == 0x80, "C_UseResourceAction must be 0x80");

}  // namespace wh::playermodule
