#pragma once
#include <cstdint>
#include "guimodule/I_Cutscene.h"

// -----------------------------------------------
// wh::guimodule::C_TextCutscene -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB06E8; vtable 0x183B2DE68. Simple I_Cutscene leaf (full-screen text
// card); creator sub_1811CF40C (alloc 0x78 @0x1811cf439). Own slots: [0] 0x182B361F0,
// [1] 0x182B38644, [2] 0x182B376FC, [3] 0x182B37C9C, [4] nullsub, [5] 0x180838AE0,
// [6] 0x181A78EC0, [7] 0x180A2A858.

namespace wh::guimodule {

class C_TextCutscene : public I_Cutscene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TextCutscene;
    void Play() override;            // [1] 0x182B38644
    void Stop() override;            // [2] 0x182B376FC
    void Update() override;          // [3] 0x182B37C9C
    void Reset() override;           // [4] nullsub
    void _vf5() override;            // [5] 0x180838AE0
    E_CutsceneType::Type GetType() const override;   // [6] 0x181A78EC0  Text(6)
    bool IsActive() const override;  // [7] 0x180A2A858

    CryStringT<char> m_text;     // +0x60  [name coined]
    uint64_t         m_unk68;    // +0x68  ctor 0 [role UNVERIFIED]
    uint64_t         m_unk70;    // +0x70  ctor 0 [role UNVERIFIED]
};
static_assert(sizeof(C_TextCutscene) == 0x78, "C_TextCutscene must be 0x78 (creator sub_1811CF40C)");

}  // namespace wh::guimodule
