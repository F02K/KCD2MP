#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionDodgeParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionDodge -- the dodge action (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xF8 (PROVEN: factory sub_181482FAC allocates 248 bytes).
// -----------------------------------------------
// *** KCD2-NEW: no KCD1 counterpart. ***
// RTTI: .?AVC_CombatActorActionDodge@combatmodule@wh@@  (TD 0x184AF9E58; primary vtable 0x183B92428).
// ctor sub_18148306C; base ctor sub_181483190; factory sub_181482FAC; deleting dtor sub_181483128.
//
// Plain animated action -- NO helper-owner / collision bases.  RTTI BaseClassArray (mdisp),
// most-derived first:
//   C_CombatActorActionDodge
//   C_CombatActorAnimatedAction<S_CombatActorActionDodgeParams,1,I_CombatActorActionPrivate>     @ +0x00
//     -> ... -> C_Action @ +0x00 ; I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x80
// AnimatedAction<...,1> ends at +0xA0, so the leaf tail begins at +0xA0.  Default priority 3.  The
// 0x30 params (@ +0x50) are bulk-copied from a caller argument by the factory (three OWORD stores).
//
// The leaf embeds a 0x20 "motion/state" sub-struct at +0xA0 (ctor sub_18090B2A4) -- the SAME sub-struct
// type is embedded inside S_CombatActorActionDodgeParams at its +0x10.

namespace wh::combatmodule {

class C_CombatActor;   // owner back-reference (forward-declared)

class C_CombatActorActionDodge
    : public C_CombatActorAnimatedAction<S_CombatActorActionDodgeParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionDodge;
    // ---- own members (leaf tail begins at +0xA0, after C_CombatActorAnimatedAction<...,1>) ----
    // 0x20 motion/state sub-struct (ctor sub_18090B2A4): qword + 3 dwords + 2 dwords + byte.
    uint64_t       m_stateQ;        // +0xA0  init 0
    uint32_t       m_stateA[3];     // +0xA8  init 0  (UNVERIFIED -- possibly Vec3)
    uint32_t       m_stateB[2];     // +0xB4  init 0  (UNVERIFIED -- possibly Vec2)
    uint8_t        m_stateFlag;     // +0xBC  init 0
    uint8_t        _padBD[3];       // +0xBD
    C_CombatActor* m_pOwner;        // +0xC0  copy of the C_CombatActorObject owner pointer
    uint64_t       m_fieldC8;       // +0xC8  init 0
    uint64_t       m_fieldD0;       // +0xD0  init 0
    uint64_t       m_fieldD8;       // +0xD8  init 0
    uint64_t       m_fieldE0;       // +0xE0  init 0
    uint64_t       m_fieldE8;       // +0xE8  init 0
    bool           m_fieldF0;       // +0xF0  OnEnter sub_181648A28 @0x181648BC8 stores (cfg dword>0) -> bool
    uint8_t        _padF1[7];       // +0xF1
};
static_assert(sizeof(C_CombatActorActionDodge) == 0xF8, "C_CombatActorActionDodge must be 0xF8");

}  // namespace wh::combatmodule
