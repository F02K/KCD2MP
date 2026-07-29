#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_CutsceneType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// I_Cutscene::GetType() [6] per-leaf constant (all 8 stubs byte-decoded:
// "xor al,al / mov al,imm; ret"). Contiguous and complete for 1.5.6. Enum and
// member names [COINED] from the leaf RTTI class names (no binary name table;
// no RTTR registration exists for this enum).

namespace wh::guimodule {

struct E_CutsceneType {
    enum Type : uint8_t {
        Fader      = 0,  // C_FaderCutscene      (stub 0x180838AE0)
        FastTravel = 1,  // C_FastTravelCutscene (stub 0x18041A6A0)
        Ingame     = 2,  // C_IngameCutscene     (stub 0x181A74A40)
        Rendered   = 3,  // C_RenderedCutscene   (stub 0x181A78EB0)
        SkipTime   = 4,  // C_SkipTimeCutscene   (stub 0x181A78EA0)
        TrackView  = 5,  // C_TrackViewCutscene  (stub 0x181A78ED0)
        Text       = 6,  // C_TextCutscene       (stub 0x181A78EC0)
        Credits    = 7,  // C_CreditsCutscene    (stub 0x181A7F810)
    };
};

}  // namespace wh::guimodule
