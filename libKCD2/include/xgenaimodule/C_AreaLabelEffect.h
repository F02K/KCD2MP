#pragma once
#include <cstdint>
#include <vector>
#include "../conceptmodule/C_Effect.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../conceptmodule/C_TypedArrayPortRef.h"
#include "I_Area.h"
#include "E_AddRemove.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AreaLabelEffect : conceptmodule::C_Effect -- concept-graph
// node that adds/removes labels on areas (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x180 (create-site proven: sub_181331F34 @0x181331f63: operator
// new(384) + memset(384) -> ctor sub_181331FEC; C_Effect base ctor
// sub_1806B19C0 fills +0x00..+0x88).
// -----------------------------------------------
// RTTI TD rva 0x4F29208, COL 0x40E3498.  Vtable rva 0x3B623D8 (46 slots;
// [0] deleting dtor sub_181460F84; [1..3],[16],[30],[42],[45] nullsub;
// [6] sub_1805F5DA0 shared RTTR stub).  Overrides vs the C_Effect chain NOT
// diffed [U] -- only the dtor declared.  Three embedded conceptmodule port
// refs (0x40 each): areas array-port @+0x88, label-strings port @+0xC8,
// add/remove selector port @+0x108.

namespace wh::xgenaimodule {

class C_AreaLabelEffect : public conceptmodule::C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaLabelEffect;
    ~C_AreaLabelEffect();   // [0] deleting dtor sub_181460F84

    conceptmodule::C_TypedArrayPortRef<std::vector<I_Area*>>       m_areasPort;     // +0x88
    conceptmodule::C_TypedPortRef<std::vector<CryStringT<char>>>   m_labelsPort;    // +0xC8
    conceptmodule::C_TypedPortRef<E_AddRemove::Type>               m_addRemovePort; // +0x108
    std::vector<I_Area*>          m_appliedAreas;   // +0x148  {first,last,end}; writer sub_180CC2378 <- m_areasPort resolve
    std::vector<CryStringT<char>> m_appliedLabels;  // +0x160  {first,last,end@+0x170}; writer sub_180CC2378 <- m_labelsPort
    bool     m_flag178;       // +0x178  ctor: 0 [U role]
    uint8_t  _pad179[7];      // +0x179
};
static_assert(sizeof(C_AreaLabelEffect) == 0x180, "C_AreaLabelEffect must be 0x180 (new 384 at sub_181331F34)");

}  // namespace wh::xgenaimodule
