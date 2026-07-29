#pragma once
#include <cstdint>
#include "C_Actor.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::entitymodule::C_Human : C_Actor  (KCD2 WHGame.dll 1.5.6, kd7u).  own region ends 0xA60.
// -----------------------------------------------
// RTTI .?AVC_Human@entitymodule@wh@@  primary vtable 0x183A3B398 (283 slots)
// ctor 0x1804CE118  dtor 0x1804CE9B4.  Own members occupy +0x9C0..+0xA60 (0xA0); base is C_Actor.
// SIZEOF NOTE (verify pass): 0xA60 is the layout-boundary; the reachable heap allocation
// sub_1804CE06C news 0xA78 and constructs the C_NPCActor SUBCLASS (own 0x18 extension begins
// exactly at 0xA60). No standalone new(0xA60) C_Human site was located.
//
// [FUNDAMENTAL vs KCD1] Own region shrank 0xC0 -> 0xA0 and diverged: KCD1's m_humanSignal,
//   m_humanStanceParams and m_pGlobalTableRef have no KCD2 own-region counterpart. Two earlier
//   KCD2 readings are REFUTED by the round-3 deep map (verify-confirmed): +0x9C0 is NOT a
//   CryStringT (it is a wh::shared::C_Signal -- the "two uint16 = 0xFFFF @+0x9C8" were its emit
//   cursors) and +0x9EC is NOT a {begin,end,cap} container (it is S_TrackedFloat[3], whose 4-byte
//   alignment is exactly why it can legally sit at the 4-aligned offset 0x9EC).
//
// The +0xA08..+0xA50 fields form one embedded CryMannequin fragment/aim sub-controller (teardown
// sub_1805D1ABC, runtime driver sub_1805D16D8): two fragment entries (-3 = invalid FragmentID,
// dtor parks them at -2), owner back-ptr, and an owning mannequin action handle created/released
// through the manager global qword_18492D830 (vtbl+136 / vtbl+152; IActionController vs IMannequin
// identity UNRESOLVED -- the global is null in static data).

struct IMannequinUserParams;   // CryEngine; +0xA50 points at the global wh SMannequinHumanParams instance (IS-A)
class IAction;                 // ICryMannequin action (class per SDK)

namespace wh::entitymodule {

class C_HumanHandHelper;

// {float value; dirty flag} tracked parameter. Built by sub_1809CF028 (zeroes 24B for [3]);
// assignment op sub_1809CEF38 walks 3 elements as {float@+0; if (new != old) byte@+4 = 1}.
struct S_TrackedFloat {
    float   value;      // +0x00
    bool    changed;    // +0x04  dirty flag, set on assign-if-different
    uint8_t _pad05[3];  // +0x05
};
static_assert(sizeof(S_TrackedFloat) == 0x8, "S_TrackedFloat must be 0x8");

class C_Human : public C_Actor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Human;
    wh::shared::C_Signal<> m_signal9C0;                // +0x9C0  VERIFIED C_Signal (shared default sentinel unk_185665430,
                                                       //         teardown sub_1808E19EC); args/purpose UNRESOLVED
    void*    m_pHandItemController;                    // +0x9D0  raw NON-owning; pointee back-refs C_Human @+56 and iterates
                                                       //         2 hand slots (sub_1809E503C) -- class + name tentative
    uint32_t m_state9D8;                               // +0x9D8  (reset sub_1809E5178 zeroes as DWORD; role unresolved)
    uint32_t m_registrationHandle;                     // +0x9DC  packed {u16 index+1, u16 salt}; unregistered by dtor
                                                       //         sub_1823CA568 via global manager(+64); reader sub_1803ECF9C
                                                       //         indexes a 648-byte-stride table
    uint32_t m_unk9E0;                                 // +0x9E0  width-4 confirmed; int-vs-float + meaning UNVERIFIED
    uint32_t m_unk9E4;                                 // +0x9E4
    uint32_t m_unk9E8;                                 // +0x9E8
    S_TrackedFloat m_trackedParams[3];                 // +0x9EC  (0x18, 4-aligned; ctor sub_1809CF028, assign sub_1809CEF38)
    int32_t  m_unkA04;                                 // +0xA04  NOT ctor-initialized; type/meaning UNVERIFIED
    // --- embedded mannequin fragment/aim sub-controller [0xA08..0xA50) ---
    int32_t  m_fragmentId0;                            // +0xA08  FragmentID, init -3 (invalid); dtor sets -2
    int32_t  m_fragParam0;                             // +0xA0C  NOT ctor-initialized (entry field, role unverified)
    uint16_t m_fragTag0;                               // +0xA10  init 0xFFFF
    uint8_t  m_fragFlag0;                              // +0xA12  (ctor 0)
    uint8_t  _padA13;                                  // +0xA13
    int32_t  m_fragmentId1;                            // +0xA14  FragmentID, init -3; dtor sets -2
    int32_t  m_fragParam1;                             // +0xA18  NOT ctor-initialized
    uint16_t m_fragTag1;                               // +0xA1C  init 0xFFFF
    uint8_t  m_fragFlag1;                              // +0xA1E  (ctor 0)
    uint8_t  _padA1F;                                  // +0xA1F
    C_Human* m_pMannequinOwner;                        // +0xA20  = this  VERIFIED (driver reads owner+0x38 / owner+0x668)
    uint8_t  m_mannequinMode;                          // +0xA28  init 2 (mode/priority, tentative)
    uint8_t  _padA29[7];                               // +0xA29
    IAction* m_pMannequinAction;                       // +0xA30  OWNING mannequin action (created qword_18492D830->vtbl[136],
                                                       //         released ->vtbl[152] in dtor)
    uint16_t m_mannequinFlags;                         // +0xA38  (reset sub_1809E5178 zeroes as WORD)
    uint8_t  m_mannequinFlagA3A;                       // +0xA3A  init 1 (tentative)
    uint8_t  _padA3B;                                  // +0xA3B
    int32_t  m_mannequinFieldA3C;                      // +0xA3C  (ctor 0)
    uint64_t m_mannequinFieldA40;                      // +0xA40  NOT ctor-initialized (ptr or CTimeValue, UNVERIFIED)
    void*    m_mannequinFieldA48;                      // +0xA48  (ctor 0)
    // --- end embedded record ---
    IMannequinUserParams* m_pMannequinUserParams;      // +0xA50  = &off_18492A0F0 global SMannequinHumanParams  VERIFIED
    C_HumanHandHelper* m_pHandHelper;                  // +0xA58  (0xB0 heap, ctor sub_1804CE428)  VERIFIED
};
static_assert(sizeof(C_Human) == 0xA60, "C_Human must be 0xA60");

}  // namespace wh::entitymodule
