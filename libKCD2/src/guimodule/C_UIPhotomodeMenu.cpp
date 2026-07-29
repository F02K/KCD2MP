#include "guimodule/C_UIPhotomodeMenu.h"
#include "Offsets/Offsets.h"

// C_UIPhotomodeMenu forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::guimodule {

void C_UIPhotomodeMenu::BuildCvarWidgets()
{
    // sub_181F8DDA0: one widget per entry of the global photomode cvar list
    // @0x185593560 (sub_181F8B780 adder; special cases wh_ui_photomodeVignetteIntensity
    // sub_181F8E0F0 and wh_ui_photomodeLogoType sub_181F8DEE0). The page must already
    // be prepared -- C_UISettings::ShowRootPhotomodePage is the canonical caller.
    using Fn = void(__fastcall*)(C_UIPhotomodeMenu*);
    static REL::Relocation<Fn> fn{ REL::ID(76) };
    fn(this);
}

}  // namespace wh::guimodule
