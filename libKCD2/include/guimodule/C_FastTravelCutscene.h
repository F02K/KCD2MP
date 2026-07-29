#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_FastTravelCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0568; vtable 0x183B2DED0. Simple I_Cutscene leaf (the fast-travel
// transition screen; playermodule C_FastTravel drives it); creator sub_1811CF500
// (alloc 0x70 @0x1811cf52d). Own slots: [0] 0x182B36154, [1] 0x182B38364,
// [2] 0x182B37678, [3] 0x182B37CA4, [4] 0x181A8CA50, [5] 0x180838AE0, [6] 0x18041A6A0,
// [7] 0x182B3777C.

namespace wh::guimodule {

class C_FastTravelCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FastTravelCutscene;
    void Play() override;            // [1] 0x182B38364
    void Stop() override;            // [2] 0x182B37678
    void Update() override;          // [3] 0x182B37CA4
    void Reset() override;           // [4] 0x181A8CA50
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x18041A6A0  FastTravel(1)
    bool IsActive() const override;  // [7] 0x182B3777C

    Vec3    m_destPos;     // +0x60  FT destination; Update(0x182B37CA4) writes 3 floats from a pos-node; Play(0x182B38364) passes &m_destPos to StartFastTravel(0x182DE2CC0); IsActive(0x182B3777C) tests !=(0,0,0)
    uint8_t m_travelIcon;  // +0x6C  enum candidate {Player=0,Group=1,Woman=2,Couple=3} (attr "esFastTravelIcon"; writer 0x182B38278; default 0)
    uint8_t m_eventPolicy; // +0x6D  enum candidate {None=0,NonGuaranteed=1,Generic=2,str@0x183AE1158=3} (attr "esDisableRandomEvents"; writer 0x182B381FC; default 3)
    bool    m_flag6E;      // +0x6E  'stopped' flag: creator/Play(0x182B383C6) set 0, Stop(0x182B37698) sets 1
    uint8_t _pad6F;        // +0x6F
};
static_assert(sizeof(C_FastTravelCutscene) == 0x70, "C_FastTravelCutscene must be 0x70 (creator sub_1811CF500)");

}  // namespace wh::guimodule
