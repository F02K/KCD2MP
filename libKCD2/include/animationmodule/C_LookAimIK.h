#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::animationmodule::C_LookAimIK -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x188.
// -----------------------------------------------
// Look/aim IK parameters + 6 runtime blend channels. Embedded at C_Actor+0x288 (boundary-proven:
// next ctor at +0x410) and at SLuaCache_ActorGameParams+0x10. ctor sub_180F2CEB4; XML loader
// sub_180F2D87C (field names below are its literal attribute strings; degree attributes are stored
// as radians). Channel stride is 0x28 (40 decimal -- the earlier "0x40" was a dec/hex mixup).

namespace wh::animationmodule {

// One runtime look/aim blend channel (0x28; zero-init except the 1.0 weight; the other 9 float
// slots have no located reader -- placeholder names, likely one channel per aim pose).
struct S_LookAimChannel {
    float f00, f04;      // +0x00
    float weight;        // +0x08  init 1.0
    float f0C;           // +0x0C
    float f10, f14, f18, f1C, f20, f24;   // +0x10..+0x24
};
static_assert(sizeof(S_LookAimChannel) == 0x28, "S_LookAimChannel must be 0x28");

class C_LookAimIK {
public:
    CryStringT<char> idleLeftArmAimPose;    // +0x000
    CryStringT<char> idleRightArmAimPose;   // +0x008
    CryStringT<char> idleBothArmsAimPose;   // +0x010
    CryStringT<char> runLeftArmAimPose;     // +0x018
    CryStringT<char> runRightArmAimPose;    // +0x020
    CryStringT<char> runBothArmsAimPose;    // +0x028
    CryStringT<char> unk30;                 // +0x030  empty-init in ctor (sub_1804FD80C()+12); pose/name string, no located reader, loader never writes it
    CryStringT<char> lookAtSimpleHeadBone;  // +0x038  default "Bip01 Head"
    float   weight40;                       // +0x040  init 1.0 (loader-untouched; name inferred)
    float   weight44;                       // +0x044  init 1.0
    S_LookAimChannel channels[6];           // +0x048  (0xF0)
    float   lookFOV;                        // +0x138  rad; init 78.75deg
    float   aimFOV;                         // +0x13C  rad; init 180deg
    int32_t cornerSmoother;                 // +0x140  init 1
    int32_t aimIKLayer;                     // +0x144  init 14
    int32_t lookIKLayer;                    // +0x148  init 15
    float   bothArmsAimFOV;                 // +0x14C  rad half-angle; init 10deg
    float   bothArmsAimPitchFactor;         // +0x150  init 1.0
    bool    flag154;                        // +0x154  init 1 (loader-untouched)
    bool    flag155;                        // +0x155  init 1
    bool    canUseComplexLookIK;            // +0x156  init 1
    bool    hasAnyAimPose;                  // +0x157  OR of the 6 aim poses being non-empty
    float   turnThresholdAngle;             // +0x158  rad; init 5deg
    float   minimumAngleForTurnWithoutDelay; // +0x15C rad; init 30deg
    float   turnThresholdTime;              // +0x160  init 1.0
    float   stepThresholdDistance;          // +0x164  (no ctor default; allocator-zeroed)
    float   stepThresholdTime;              // +0x168  (no ctor default)
    float   maxDeltaAngleRateNormal;        // +0x16C  rad/s; init 180deg
    float   maxDeltaAngleRateAnimTarget;    // +0x170  rad/s; init 720deg
    float   maxDeltaAngleMultiplayer;       // +0x174  rad/s; init 3600deg
    float   maxDeltaAngleRateJukeTurn;      // +0x178  rad/s; init 720deg
    float   animatedCharacterTurnSpeedSmoothingTime;   // +0x17C  init 0.4
    bool    smoothedZTurning;               // +0x180  init 0
    bool    flag181;                        // +0x181  init 1 (loader-untouched)
    bool    flag182;                        // +0x182  init 1
    bool    flag183;                        // +0x183  init 1
    uint8_t _pad184[4];                     // +0x184
};
static_assert(sizeof(C_LookAimIK) == 0x188, "C_LookAimIK must be 0x188");

}  // namespace wh::animationmodule
