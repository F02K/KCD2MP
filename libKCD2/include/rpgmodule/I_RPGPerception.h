#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::I_RPGPerception -- interface of the RPG perception evaluator
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface, 27 slots, merged into the
// C_RPGPerception primary vtable (no standalone COL).
// -----------------------------------------------
// RTTI TD rva 0x4D603D8.  Sole impl: C_RPGPerception (singleton 0x1855E63B0).
// This is the evaluator the xgenai recognition FSM calls virtually
// (eval->vt[6/10/11] in sub_182086400 / sub_180585498).  Slot names derive from
// the impl behavior (see C_RPGPerception.h for per-slot EAs); [U]-tagged names
// are behavioral placeholders.  Adjacent view-cone getters DE-OVERLOADED
// (MSVC reverses same-name adjacent overloads).

namespace wh::rpgmodule {

class I_RPGPerception {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGPerception;
    virtual void  GetViewConeByWuid(void* outCone, const void* wuid) = 0;      // [0]
    virtual void  GetViewConeOfSoul(void* outCone, void* soul) = 0;            // [1] {hFov, vFov, innerCone}
    virtual void  EvalPairwiseRecognition(void* soulA, void* soulB) = 0;       // [2] [U name]
    virtual void  ResolveSoulOp3(void* soul) = 0;                              // [3] [U name]
    virtual void  UpdateDetectionPriors(void* records) = 0;                    // [4] batch: rec[+24] = prior score
    virtual float ComputePerception(float* out2, const void* observerWuid, const void* targetWuid, float dist) = 0;  // [5] THE recognition-speed formula
    virtual float GetDefaultPerception() = 0;                                  // [6] RecognitionSpeedNotVisible
    virtual void  GetVisVolumeParams(void* out, void* a2) = 0;                 // [7] DefaultVisVolume/MaxModVisibility [U name]
    virtual void  EvalRecognitionSkill(void* a2, void* a3) = 0;                // [8] [U name]
    virtual float GetItemVisibilityLifetime(void* item) = 0;                   // [9] BaseItemDisappearingTime family
    virtual float ScaleByCount(float v, int n) = 0;                            // [10] returns v * n [U name]
    virtual float ApplyCrowdDilution(float v, int crowdCount) = 0;             // [11] v * max(1 - crowd/CrowdMembers, 0)
    virtual float GetInnerConeSize(void* soul) = 0;                            // [12] slot26 or InnerConeMinRelSize default
    virtual float GetDerivedStatRatio13(void* soul) = 0;                       // [13] clamp(derived21/prop35, 0, prop50) [U name]
    virtual void  EvalSoulPairOp14(void* soulA, void* soulB) = 0;              // [14] [U role]
    virtual float GetTypedPerceptionCoef(const void* typeCrc) = 0;             // [15] CRC-keyed lerp of params 0xA00/0xA04 [U name]
    virtual void  ComputeStealVolumeVisConsp(float* outVisConsp, float ratio, void* soul) = 0;  // [16] lerp StealVisVolume Min/Max pairs
    virtual int   GetDurationMs17(void* a2) = 0;                               // [17] [U role]
    virtual void  BuildPerceptionInformation(void* soul, void* infoWriter) = 0;// [18] packs weapon/armory/appearance tags
    virtual void  BuildDistanceGatedInfo(void* soul, void* infoWriter) = 0;    // [19] person/item distance gate [U name]
    virtual float GetSneakCheckRadius() = 0;                                   // [20] StealthSneakCheckRadius (0x8A4)
    virtual float GetVerticalFov(void* soul) = 0;                              // [21] soul prop 11
    virtual float EvalViewCalc22(void* soul) = 0;                              // [22] props 11/14 + MaxViewRadius [U role]
    virtual void  EvalStealthSneak(void* a2, void* a3, void* a4, void* a5) = 0;// [23] sneak-XP award (sub_180C5C114 impl)
    virtual float EvalPlayerDistancePerception(void* soul) = 0;                // [24] [U role]
    virtual bool  EvalProbabilisticCheck(void* soul) = 0;                      // [25] prop(165) vs LCG rand
    virtual float ComputeInnerConeRelSize(void* soul) = 0;                     // [26] nervousness -> inner-cone lerp worker
};
static_assert(sizeof(I_RPGPerception) == 0x08, "I_RPGPerception is vptr-only");

}  // namespace wh::rpgmodule
