#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"
#include "game/I_SkipEventProvider.h"
#include "Offsets/vtables/ITrackEventListener.h"
#include "Offsets/vtables/IMovieListener.h"

// -----------------------------------------------
// wh::guimodule::C_TrackViewBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  ABSTRACT, sizeof 0xC0.
// -----------------------------------------------
// RTTI TD 0x184CB05D8. Base of the trackview cutscene family (C_TrackViewCutscene /
// C_IngameCutscene siblings). Bases: I_Cutscene@+0x00 (0x60) +
// wh::game::I_SkipEventProvider@+0x60 + ITrackEventListener@+0x68 (engine) +
// IMovieListener@+0x70 (engine) + empty CryDeferrable<0>@+0x78 (CHD; covered by the
// member block). vtables: primary 0x183B2E228 / skip 0x183B2E1D8 / track 0x183B2E208 /
// movie 0x183B2E1F0. ctor sub_1811CFD04 (chains the I_Cutscene base ctor sub_1811CFD84).
// Own primary overrides: [0] dtor 0x182B36224, [2] Stop 0x182B3772C, [9]/[11] trio --
// [1]/[3..8] stay pure/defaulted for the subclasses (abstract here).
// Every playback slot keys off m_pSequence (@+0xB8).

struct IAnimSequence;

namespace wh::guimodule {

class C_TrackViewBase : public I_Cutscene,                    // +0x00 (0x60)
                        public wh::game::I_SkipEventProvider, // +0x60
                        public Offsets::ITrackEventListener,  // +0x68
                        public Offsets::IMovieListener {      // +0x70
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TrackViewBase;
    void Stop() override;   // [2] 0x182B3772C  StopSequence if active

    // Interface impls (subobject vtables).
    void _vf1() override;                                       // skip 0x183B2E1D8[1] 0x182B36D6C
    void OnTrackEvent(void* pTrackEvent) override;              // track 0x183B2E208[1] 0x182B38134
    void OnMovieEvent(int movieEvent, void* pAnimSequence) override;  // movie 0x183B2E1F0[1] 0x182B37F88

    uint8_t        m_unk78[0x40];   // +0x78..+0xB8  ctor-zeroed 8-field block [roles UNVERIFIED]
    IAnimSequence* m_pSequence;     // +0xB8  the playing trackview/movie sequence (null = idle)
};
static_assert(sizeof(C_TrackViewBase) == 0xC0, "C_TrackViewBase must be 0xC0 (subclass extents pin it)");
static_assert(offsetof(C_TrackViewBase, m_pSequence) == 0xB8, "sequence at 0xB8");

}  // namespace wh::guimodule
