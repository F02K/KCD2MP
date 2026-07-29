#pragma once
#include <cstdint>
#include "C_PerceptibleVolumeManager.h"
#include "C_RecognizingThresholdManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PerceptionManager -- THE sight/perception hub: cvar block,
// perceptor dispatch tables, perceptor/perceptible lists, embedded volume +
// threshold managers (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x228 (create-site
// proven: alloc 552 in the lazy-init getter sub_180418170 -> qword_185492EA0).
// -----------------------------------------------
// RTTI TD rva 0x4EE1508; vtable 0x183FD06B0, 1 slot ([0] sub_1831CA2E4).  ROOT
// class.  ctor sub_180FB74DC.  The cvar block +0x98..+0xD4 is registered
// POST-construct by sub_180F66F0C(this) through the registrar qword_18492D8A8
// (vf+64 RegisterInt, vf+72 RegisterFloat) -- registration order/binding per field
// from that function [U, Section D item 5].
// CHEAT LEVERS: m_perceptionEnabled (+0x98) = 0 kills sight/perception
// recognition; raise m_recognizingThreshold (+0xD0) so NPCs never reach
// recognition.  (Hearing is separate: C_HearingSystem+0x08.)

namespace wh::xgenaimodule {

class C_PerceptionItemListener;

class C_PerceptionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PerceptionManager;
    virtual ~C_PerceptionManager();   // [0] sub_1831CA2E4 [U -- single slot; dtor role presumed]

    uint8_t  m_perceptorDispatch[0x90];   // +0x008..+0x097  9 x {handler_table*, i16 = -1, i16 = -1} stride-16 perceptor-category dispatch entries (&unk_18566B860 x6, &unk_185669B18, &unk_18566B870, &unk_18566B880) [U interior]
    // ---- cvar block (defaults in parens; registered post-construct, see banner) ----
    int32_t  m_perceptionEnabled;              // +0x098  wh_ai_perception_enabled (1) -- MASTER perception toggle [V]
    int32_t  m_bypassRpg;                      // +0x09C  wh_ai_perception_bypass_rpg (0) -- "recognition speed 0.5 for everyone"
    int32_t  m_lodEnabled;                     // +0x0A0  wh_ai_perception_lod_enabled (1)
    int32_t  m_jobsPerFrame;                   // +0x0A4  wh_ai_perception_jobs_per_frame (7)
    int32_t  m_perceivedStatesParallelUpdate;  // +0x0A8  wh_ai_perception_perceived_states_parallel_update (1)
    int32_t  m_parallelUpdateSize;             // +0x0AC  ..._parallel_update_size (15)
    float    m_ignoreFovRadius;                // +0x0B0  wh_ai_perception_ignore_fov_radius
    float    m_ignoreFovCylinderHeight;        // +0x0B4  ..._ignore_fov_cylinder_height
    float    m_ignoreFovCylinderBackSectionDeg;// +0x0B8  ..._ignore_fov_cylinder_back_section_deg
    float    m_ignoreFovCylinderWeightDir;     // +0x0BC  ..._ignore_fov_cylinder_weight_direction
    int32_t  m_coneBiasEnable;                 // +0x0C0  wh_ai_perception_cone_bias_enable (1)
    float    m_coneBiasLogBase;                // +0x0C4  ..._cone_bias_log_base
    float    m_coneHysteresis;                 // +0x0C8  ..._cone_hysteresis
    int32_t  m_targetFilteringEnable;          // +0x0CC  wh_ai_perception_target_filtering_enable (1)
    float    m_recognizingThreshold;           // +0x0D0  wh_ai_perception_recognizing_threshold -- DETECTION THRESHOLD (read by RTM slot [1] as PM+0xD0) [V]
    uint8_t  _padD4[4];                        // +0x0D4
    // ----
    uint64_t m_listHeads[8];                   // +0x0D8/+0xE8/+0xF8/+0x108  four intrusive circular-list heads (perceptor/perceptible lists; 40B nodes, {self, self, 0x0101-pair}); list[0] @+0xD8 is THE perceptibles container the recognition loop sub_180584B88 iterates (via *(sensingCtx+216))
    C_PerceptionItemListener* m_itemListener;  // +0x118  16-byte world-inventory listener {vptr, +8=this}; created+registered by sub_180F661D0 @0x180f66399 (list at *(gameCtx+224)+232) [V]
    C_PerceptibleVolumeManager m_volumeManager;        // +0x120 (0x60) embedded (ctor sub_180FB7764) [V]
    uint64_t m_listHead180[3];                 // +0x180..+0x197  intrusive list head (56B nodes) [U +0x190 role]
    C_RecognizingThresholdManager m_thresholdManager;  // +0x198 (0x90) embedded [V]
};
static_assert(sizeof(C_PerceptionManager) == 0x228, "C_PerceptionManager must be 0x228 (552)");

}  // namespace wh::xgenaimodule
