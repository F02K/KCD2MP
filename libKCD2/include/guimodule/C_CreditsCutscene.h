#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_CreditsCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB04F8; vtable 0x183B2DE00. Simple I_Cutscene leaf (credits roll);
// creator sub_1811CF334 (alloc 0x68 @0x1811cf361, ctor inline). Own slots: [0]
// 0x182B35FCC, [1] Play 0x182B382F4, [2] Stop 0x182B375EC, [3] Update 0x182B37C9C,
// [4] Reset nullsub, [5] 0x180838AE0, [6] GetType 0x181A7F810, [7] IsActive 0x181A72E80.

namespace wh::guimodule {

class C_CreditsCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CreditsCutscene;
    void Play() override;            // [1] 0x182B382F4
    void Stop() override;            // [2] 0x182B375EC
    void Update() override;          // [3] 0x182B37C9C
    void Reset() override;           // [4] nullsub
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A7F810  Credits(7)
    bool IsActive() const override;  // [7] 0x181A72E80

    bool    m_flag60;    // +0x60  ctor 0 [role UNVERIFIED]
    uint8_t _pad61[7];   // +0x61
};
static_assert(sizeof(C_CreditsCutscene) == 0x68, "C_CreditsCutscene must be 0x68 (creator sub_1811CF334)");

}  // namespace wh::guimodule
