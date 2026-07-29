#pragma once
#include "C_OutputDataPort.h"
#include "rttr/type.h"

// -----------------------------------------------
// wh::conceptmodule::C_AssetPort -- world-asset reference pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x40, vtable 0x183E25180 (32 slots).
// -----------------------------------------------
// Created by CreatePorts (0x1806A03D4) for <Asset .../> entries: ctor
// 0x1806B1D74(name, value) keeps the asset NAME text (alloc 0x40 @0x1806B1DCA).
// GetValue [16] = 0x18087C988 forwards (this, out, &m_assetName, &m_assetType) to
// the world-object lookup 0x18087C9A8 -- the value is resolved from the live world
// each read.  Resets IsPassThrough [10]/[11] to false.  Direction Out, role
// OutputData.

namespace wh::conceptmodule {

class C_AssetPort : public C_OutputDataPort {
public:
    RTTR_ENABLE(C_OutputDataPort)      // [5] 0x1806B18AC, [7] 0x18265349C
    rttr::variant GetValue() override; // [16] 0x18087C988 world lookup by name+type

    CryStringT<char> m_assetName;      // +0x30  asset identifier text from XML
    rttr::type m_assetType;            // +0x38  magic-static default type (sub_180C0CDA4 -> qword_18549D930) [exact meaning U]
};
static_assert(sizeof(C_AssetPort) == 0x40, "C_AssetPort must be 0x40");

}  // namespace wh::conceptmodule
