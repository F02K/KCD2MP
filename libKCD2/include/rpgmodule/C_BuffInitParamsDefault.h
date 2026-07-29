#pragma once
#include "I_BuffInitParams.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffInitParamsDefault : I_BuffInitParams -- KCD2 1.5.6 (kd7u).  sizeof 0x8.
// -----------------------------------------------
// RTTI .?AVC_BuffInitParamsDefault@rpgmodule@wh@@  vtable 0x183A43B88 -- concrete default
// implementations of the 3 interface virtuals ([1] sub_181A730F0, [2] sub_18066CD10,
// [3] sub_181A82500). Used as an 8-byte stack functor in the buff-impl instantiation path
// (sub_1804C80E0 / sub_18063ACE4); no separate heap alloc site measured (vtable ptr only).
//
// Concrete param-carrying subclasses present in KCD2 RTTI (all single-inherit this class):
//   0x10 one-param {vtable, one 8B field}: C_ItemBuffInitParams (own header),
//     C_CaffeineBuffInitParams, C_CombatStaminaBuffInitParams, C_ConsumedItemBuffInitParams,
//     C_ConsumedPoisonBuffInitParams, C_JailRecoveryBuffInitParams, C_MoraleContextBuffInitParams,
//     C_OverreadBuffInitParams, C_SharpeningPressureBuffInitParams,
//     C_ShortTermNutritionFoodBuffInitParams, C_SleepBuffInitParams.
//   0x18 two-param {vtable, two 8B fields}: C_HangoverBuffInitParams, C_InjuredBuffInitParams,
//     C_UnconsciousBuffInitParams.
//   Also present (layouts not measured): C_AdjustableFadingBuffInitParams,
//     C_CarryingLoadBuffInitParams, C_RiddenHorseBuffInitParams, C_ReadingQualityInitParams,
//     C_AlcoholCravingInitParams; C_ItemHealthCheckingBuffInitParams : C_ItemBuffInitParams.
// Only C_ItemBuffInitParams (field decoded) gets its own header; the rest are all the same
// {vtable + N raw qword params} shape with UNRESOLVED param meanings.

namespace wh::rpgmodule {

class C_BuffInitParamsDefault : public I_BuffInitParams {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffInitParamsDefault;
    void vf1() override;   // [1] = sub_181A730F0
    void vf2() override;   // [2] = sub_18066CD10
    void vf3() override;   // [3] = sub_181A82500
};
static_assert(sizeof(C_BuffInitParamsDefault) == 0x8, "C_BuffInitParamsDefault must be 0x8");

}  // namespace wh::rpgmodule
