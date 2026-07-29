#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_RenderedCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB0750; vtable 0x183EE1DB8. The pre-rendered VIDEO cutscene leaf.
// Bases: I_Cutscene@+0x00 + empty CryDeferrable<0> @+0x61 (CHD; not modeled -- covered
// by the +0x60 pad below). ctor sub_1811CF7E8; creator sub_1811CF6AC (alloc 0xD8
// @0x1811cf6da). The ONLY leaf overriding the defaulted [8] (0x181F58CE0). Own slots:
// [0] 0x182B361BC, [1] 0x182B384C8, [2] 0x1819E09B0, [3] 0x182B37E4C, [4] nullsub,
// [5] 0x182B38F00, [6] 0x181A78EB0, [7] 0x182B377AC, [8] 0x181F58CE0.

namespace wh::guimodule {

struct S_RenderedCutsceneFile;        // [SYNTHETIC] per-file entry of m_files78
class I_RenderedCutsceneResource;     // [SYNTHETIC] refcounted video resource
class I_RenderedCutsceneObject;       // [SYNTHETIC] owned polymorphic object @+0xB8
class I_RenderedCutscenePlayback;     // [SYNTHETIC] Bink playback stream

class C_RenderedCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RenderedCutscene;
    void Play() override;            // [1] 0x182B384C8
    void Stop() override;            // [2] 0x1819E09B0
    void Update() override;          // [3] 0x182B37E4C
    void Reset() override;           // [4] nullsub
    void _vf5() override;            // [5] 0x182B38F00
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A78EB0  Rendered(3)
    bool IsActive() const override;  // [7] 0x182B377AC
    void _vf8() override;            // [8] 0x181F58CE0 -- only leaf overriding the default [role UNVERIFIED]

    uint8_t          _pad60[8];      // +0x60  UnsafeOp::CryDeferrable<0> empty base @+0x61 (no in-obj data) + align
    CryStringT<char> m_str68;        // +0x68  ctor magic-static [role UNVERIFIED -- video path?]
    CryStringT<char> m_str70;        // +0x70  ctor [role UNVERIFIED]
    std::vector<S_RenderedCutsceneFile> m_files78;  // +0x78  {first,last,end}; per-entry filename queried by sub_180799608 [element name SYNTHETIC]
    uint8_t          m_byte90;       // +0x90  ctor 1
    uint8_t          m_byte91;       // +0x91  ctor 1
    uint8_t          m_byte92;       // +0x92  ctor 0
    uint8_t          _pad93[5];      // +0x93
    std::vector<uint32_t> m_ids98;                 // +0x98  {first,last,end}; filled with dword ids from base+0x30 vector (sub_1819D0C80)
    _smart_ptr<I_RenderedCutsceneResource> m_pResourceB0;  // +0xB0  built from m_str68 video path [pointee SYNTHETIC]
    I_RenderedCutsceneObject* m_pObjectB8;         // +0xB8  owning; dtor calls virtual deleting-dtor [pointee SYNTHETIC]
    int32_t          m_intC0;        // +0xC0  ctor 0
    uint8_t          _padC4[4];      // +0xC4
    std::shared_ptr<I_RenderedCutscenePlayback> m_playbackC8;  // +0xC8 ptr / +0xD0 ctrl; Bink video stream from manager qword_18492D990 [pointee SYNTHETIC]
};
static_assert(sizeof(C_RenderedCutscene) == 0xD8, "C_RenderedCutscene must be 0xD8 (creator sub_1811CF6AC)");

}  // namespace wh::guimodule
