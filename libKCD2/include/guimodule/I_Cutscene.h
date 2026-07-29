#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"
#include "E_CutsceneType.h"

// -----------------------------------------------
// wh::guimodule::I_Cutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 -- DATA-BEARING base.
// -----------------------------------------------
// RTTI .?AVI_Cutscene@guimodule@wh@@ (TD 0x184CB0720, COL 0x4138CB8). vtable 0x183B2E298
// (12 slots). Base ctor sub_1811CFD84 (chained by every cutscene leaf). NOT a pure
// vtable-only interface: carries the shared cutscene identity/config block below --
// m_name @+0x08 is independently corroborated by C_CutsceneDatabase's console
// autocompleter (GetValue returns element+0x08).
//
// Virtual contract: [0] dtor · [1..7] pure · [8] defaulted (nullsub; only
// C_RenderedCutscene overrides) · [9..11] rttr trio. Method names [1..7] are
// BEHAVIOR-COINED from the C_TrackViewCutscene bodies over m_pSequence (dossier
// cutscene_fader.md §B0): [1] 0x182B3878C plays the sequence, [2] 0x182B3772C stops it,
// [3] 0x182B37F18 per-frame tick, [4] 0x182B380B4 resets, [6] returns a distinct small
// int per leaf (TrackView `return 5`), [7] 0x182B377FC `return m_pSequence != 0`.
// Signatures beyond the coined shapes UNVERIFIED.

namespace wh::guimodule {

class I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Cutscene;
    virtual ~I_Cutscene();                                   // [0]
    virtual void Play() = 0;                                 // [1] name coined (start playback)
    virtual void Stop() = 0;                                 // [2] name coined (abort playback)
    virtual void Update() = 0;                               // [3] name coined (per-frame tick; dt arg UNVERIFIED)
    virtual void Reset() = 0;                                // [4] name coined (drop sequence / on-finished)
    virtual void _vf5() = 0;                                 // [5] getter; often shared `return 0` 0x180838AE0 [role UNVERIFIED]
    virtual E_CutsceneType::Type GetType() const = 0;        // [6] name coined; per-leaf constant (E_CutsceneType)
    virtual bool IsActive() const = 0;                       // [7] name coined (`return m_pSequence != 0`)
    virtual void _vf8();                                     // [8] defaulted nullsub; role UNVERIFIED (Skip/CanSkip?)
    // RTTR trio [9..11]: get_type 0x182B3EC24, get_ptr 0x1805F5DA0, get_derived_info
    // 0x182B3E710. Registered BASELESS (sizeof imm 0x60). No clash with the game's own
    // GetType [6] -- different identifier (was previously mis-modeled as get_type_rttr).
    RTTR_ENABLE()

    CryStringT<char> m_name;          // +0x08  cutscene name (db key; autocompleter source)
    uint8_t          m_flag10;        // +0x10  ctor 0 [role UNVERIFIED]
    uint8_t          m_flag11;        // +0x11  ctor 1 [role UNVERIFIED]
    uint8_t          m_flag12;        // +0x12  ctor 1 [role UNVERIFIED]
    uint8_t          _pad13;          // +0x13
    float            m_duration14;    // +0x14  ctor 15.0f [default duration; name coined]
    CryStringT<char> m_str18;         // +0x18  ctor empty [role UNVERIFIED]
    uint64_t         m_reserved[8];   // +0x20..+0x60  ctor-zeroed block [roles UNVERIFIED]
};
static_assert(sizeof(I_Cutscene) == 0x60, "I_Cutscene must be 0x60 (base ctor sub_1811CFD84)");
static_assert(offsetof(I_Cutscene, m_name) == 0x08, "name at 0x08 (autocompleter-corroborated)");

}  // namespace wh::guimodule
