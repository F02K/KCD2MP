#pragma once
#include "C_PortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_TypedArrayPortRef<T> -- typed array port handle
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x40 (adds NOTHING over C_PortRef).
// -----------------------------------------------
// Instantiation seen: <std::vector<rpgmodule::I_Soul*>> (target-souls port @+0x88 of
// rpgmodule::C_BuffEffect / C_TemporaryFactionEffect).  Same 0x40 embedded stride and
// ctor behavior as C_TypedPortRef; the +0x38 qword formerly modeled here is
// C_PortRef::m_cachedPort.  Array values travel as rttr::variant holding a registered
// std::vector<T> alias (Floats/Bools/Ints/ConceptPaths/Strings/Souls).

namespace wh::conceptmodule {

template <typename T>
class C_TypedArrayPortRef : public C_PortRef {
public:
    RTTR_ENABLE(C_PortRef)   // per-instantiation trio
};

}  // namespace wh::conceptmodule
