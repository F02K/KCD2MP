#pragma once
#include "C_InterfaceDataPort.h"
#include "rttr/type.h"

// -----------------------------------------------
// wh::conceptmodule::C_AssetInterfacePort -- world-asset reference on a
// module-boundary pin (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x68, vtable
// 0x183E25498 (32 slots).
// -----------------------------------------------
// CreatePorts picks this over C_AssetPort when the definition's IsInterface
// predicate is true.  Named ctor sub_182732AA4(name, value); alloc 0x68
// @0x18272EBA2.  GetValue [16] = 0x1826508C0; IsPassThrough [10] = 0x1806B3C0C.

namespace wh::conceptmodule {

class C_AssetInterfacePort : public C_InterfaceDataPort {
public:
    RTTR_ENABLE(C_InterfaceDataPort)   // [5] 0x1813F66D4, [7] 0x18126E7DC
    rttr::variant GetValue() override; // [16] 0x1826508C0

    CryStringT<char> m_assetName;      // +0x58  asset identifier text
    rttr::type m_assetType;            // +0x60  magic-static default type (sub_180C0CDA4) [exact meaning U]
};
static_assert(sizeof(C_AssetInterfacePort) == 0x68, "C_AssetInterfacePort must be 0x68");

}  // namespace wh::conceptmodule
