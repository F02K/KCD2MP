#pragma once
#include <cstdint>
#include <cstddef>
#include "../framework/C_Action.h"
#include "../framework/C_ParamAction.h"
#include "I_AlchemyAction.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyActionBase<TParams> -- alchemy action base (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI: .?AV?$C_AlchemyActionBase@US_EmptyParams@playermodule@wh@@V?$C_Action@VI_AlchemyAction...
// (instantiated for S_EmptyParams / S_IntParams / S_HandItemParams / S_InventoryParams /
// S_SwitchPotParams).  Binary base chain: C_ParamAction<TParams, C_Action<I_AlchemyAction>>;
// mirrored here (the second template arg is fixed for alchemy).
//
// Base ctors: sub_1809CC378(this, director) = C_Action<I_AlchemyAction> init (fields per
// framework/C_Action.h; +0x40 m_pOwnerSubsystem = the director), then
// sub_1809CC290(this, alchemy, director):
//   - m_actionSequenceId (+0x3C) = 1  -- the DIRECTOR SLOT (C_SwitchPotHalterAction ctor sets 4)
//   - m_pAlchemy  = arg1 (disasm-proven: mov r9,rdx -> [rbx+58h] for the S_EmptyParams shape)
//   - m_pManager  = *(*(S_GameContext::Instance()+0xE0)+0xC8)  (same value as C_Alchemy+0x680)
// For fat params the tail shifts by sizeof(TParams): S_SwitchPotParams (0x10) puts m_pAlchemy at
// +0x60 / m_pManager at +0x68 (leaf 0x80).
//
// Adds 2 own vtable slots [39],[40] after C_Action<I_AlchemyAction>'s [0..38] (the base vtable
// 0x183A67978 ends at 38; its slot-39 position holds the next COL).  Roles UNVERIFIED
// (bellows: [39]=nullsub, [40]=0x181A98DF0).

namespace wh::playermodule {

class C_Alchemy;

template<typename TParams = S_EmptyParams>
class C_AlchemyActionBase
    : public wh::framework::C_ParamAction<TParams, wh::framework::C_Action<I_AlchemyAction>> {
public:
    virtual void _vfAlch39() = 0;   // [39] +0x138  bellows: nullsub  [role UNVERIFIED]
    virtual void _vfAlch40() = 0;   // [40] +0x140  bellows: 0x181A98DF0  [role UNVERIFIED]

    C_Alchemy* m_pAlchemy;   // +0x50+sizeof(TParams) aligned8 (S_EmptyParams shape: +0x58)
    void*      m_pManager;   // +8 after m_pAlchemy (S_EmptyParams shape: +0x60) -- subsystem mgr
};
static_assert(sizeof(C_AlchemyActionBase<S_EmptyParams>) == 0x68,
              "C_AlchemyActionBase<S_EmptyParams> must be 0x68");
static_assert(offsetof(C_AlchemyActionBase<S_EmptyParams>, m_pAlchemy) == 0x58,
              "m_pAlchemy at 0x58 for empty params");

}  // namespace wh::playermodule
