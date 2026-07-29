#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::C_StateProxy<TBase> -- KCD2 WHGame.dll 1.5.6 (kd7u).  Adds 1 byte over TBase.
// -----------------------------------------------
// RTTI-proven template (instantiation seen: C_StateProxy<C_Effect>, sizeof 0x90 =
// C_Effect 0x88 + flag byte + pad). Concept-graph "state proxy" intermediate: overrides
// the node-table slot [12] with sub_1816A4050 (reads a bool port, returns an rttr
// variant) and adds a slot [28] override -- not modeled as C++ virtuals per module
// convention. Base of C_InfiniteHoldInteractionTrigger.

namespace wh::conceptmodule {

template <typename TBase>
class C_StateProxy : public TBase {
public:
    uint8_t m_stateFlag;      // +sizeof(TBase)  ctor 0 [role UNVERIFIED]
    uint8_t _padProxy[7];     // alignment to 8
};

}  // namespace wh::conceptmodule
