#pragma once
#include "C_DataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_OutputDataPort -- data-producing pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30 (adds no data), vtable
// 0x183E25288 (32 slots).
// -----------------------------------------------
// Ctor/creator 0x1806B2444 (alloc 0x30 @0x1806B2492).  Stores NO links: data edges
// live on the consuming C_InputDataPort, and value pull ends here at the owning
// node.  Direction Out (0x181A72480), role OutputData -- neither edge source nor
// target in Connect (the CONSUMER records the link).  Overrides: [16] shared
// GetValue, [31] GetValueImpl 0x180693458 = owner C_Node::GetPortValue only
// (no upstream fallback).

namespace wh::conceptmodule {

class C_OutputDataPort : public C_DataPort {
public:
    RTTR_ENABLE(C_DataPort)   // [5] 0x18061B81C, [7] 0x18265357C
};
static_assert(sizeof(C_OutputDataPort) == 0x30, "C_OutputDataPort adds no data");

}  // namespace wh::conceptmodule
