#pragma once
#include <cstdint>
#include "I_RPGPerception.h"
#include "S_RpgParams.h"

// -----------------------------------------------
// wh::rpgmodule::C_RPGPerception : I_RPGPerception -- THE recognition/perception
// evaluator service (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (init
// sub_1819DD880 writes exactly vptr + m_pConstants + m_rngState; extent-proven,
// no alloc site -- process-global singleton @ 0x1855E63B0, stored at
// C_PerceptionManager+0x1A8 as the FSM evaluator).
// -----------------------------------------------
// RTTI TD rva 0x4D5FEE8; COL 0x184128958; vtable 0x3D18E68 (27 slots 0..26 --
// vtable ends at the C_RPGMovement COL; single-inheritance, interface merged).
// Per-slot impl EAs:
//  [0] sub_1805D05E4 GetViewConeByWuid (default {119, 19.5, 10})
//  [1] sub_1805D0634 GetViewConeOfSoul {hFov prop10 def120, vFov prop11 def20, inner}
//  [2] sub_1812EF170  [3] sub_1805D06C4  [4] sub_1805D0718 (rec[+24] = sub_18047164C)
//  [5] sub_180471B4C ComputePerception -- time = (P + K(+-)*consp) * bias(dist)
//      * visMod * (1 + stealth*StealthSkillToRecogTime) [* NonPlayerRecognitionTimeMod]
//  [6] sub_180584B70 (RecognitionSpeedNotVisible 0x858)
//  [7] sub_180B42E68 (DefaultVisVolume 0x9E0)  [8] sub_180B42D0C  [9] sub_180B42C20
//  [10] sub_18121BD10  [11] sub_18123DCCC crowd dilution (0x85C)
//  [12] sub_180585384  [13] sub_180874418  [14] sub_182CF057C  [15] sub_182CEF9A0
//  [16] sub_182CF0B58 ComputeStealVolumeVisConsp (lerp 0x9E8..0x9F4) -- the
//       steal-volume vis/consp setter
//  [17] sub_182CF0AB4  [18] sub_181FD3DF0 BuildPerceptionInformation (reads the
//       appearance stats @ soul+0xB50)  [19] sub_1805BA874  [20] sub_180C5BE90
//  [21] sub_182CF458C  [22] sub_182CF45DC  [23] sub_180C5C114 EvalStealthSneak
//       (sneak XP; virtual-only callers)  [24] sub_182CEEFC8
//  [25] sub_182CEEED8 probabilistic check (LCG 214013*s+2531011 on m_rngState)
//  [26] sub_1805853C8 inner-cone worker (nervousness/InnerConeNervousnessLimit)

namespace wh::rpgmodule {

class C_RPGPerception : public I_RPGPerception {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGPerception;

    S_Constants* m_pConstants;   // +0x08  cached &S_Constants (0x1855E63D0); init sub_1819DD880
    uint64_t     m_rngState;     // +0x10  LCG state (seed qword_18492D7E0>>16); advanced by slot [25]
};
static_assert(sizeof(C_RPGPerception) == 0x18, "C_RPGPerception must be 0x18 (init write extent)");

}  // namespace wh::rpgmodule
