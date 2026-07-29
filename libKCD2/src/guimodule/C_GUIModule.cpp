#include "guimodule/C_GUIModule.h"
#include "game/S_GameContext.h"
#include "Offsets/Offsets.h"
#include <utility>   // std::move

// C_GUIModule accessors -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// KCD1 PORT NOTE: this file replaces KCD1's CUIManager.cpp. KCD1's
// wh::guimodule::CUIManager singleton (global qword_182F77888, REL::ID(869)) has
// NO KCD2 equivalent -- the class is gone (no TD) and its element registry moved
// onto C_GUIModule::m_uiElements (full story in the header banner). The module
// pointer itself is held by S_GameContext @+0xE8.

namespace wh::guimodule {

C_GUIModule* C_GUIModule::GetInstance()
{
    auto* ctx = wh::game::S_GameContext::GetInstance();
    return ctx ? ctx->m_pGUIModule : nullptr;
}

C_UICompass* C_GUIModule::GetUICompass()
{
    // sub_180C4E098 -- rttr scan of m_uiElements for the C_UICompass-typed screen,
    // cached process-wide in qword_18548B068 (cleared with the UI). Used by the
    // quest-log marker producer sub_180DC5F24 and the checkpoint/POI paths.
    using Fn = C_UICompass*(__fastcall*)(C_GUIModule*);
    static REL::Relocation<Fn> fn{ REL::ID(34) };
    return fn(this);
}

std::map<std::string, std::shared_ptr<C_UIBase>> C_GUIModule::GetUIElementsByName() const
{
    // sub_182B8D8D8 -- the rttr "UIElementsByName" read-only property getter:
    // iterates m_uiElements and keys each element on elem->get_type() name
    // (vf[7] -> type_data+0x18 string).  It CONSTRUCTS the map into the out-slot
    // (zeroes it, then installs a fresh sentinel from sub_182B7FA70), so it must
    // receive RAW storage -- handing it a live map would leak our sentinel node.
    // Cross-heap safety: nodes/strings are game-allocated; both WHGame.dll and
    // this /MD module resolve malloc/free to the shared ucrtbase heap (see
    // CrySDKStubs.cpp), so destroying the map on our side is fine.
    using Map = std::map<std::string, std::shared_ptr<C_UIBase>>;
    using Fn = Map*(__fastcall*)(const C_GUIModule*, Map*);
    static REL::Relocation<Fn> fn{ REL::ID(89) };

    alignas(Map) unsigned char storage[sizeof(Map)];
    Map* built = fn(this, reinterpret_cast<Map*>(storage));
    Map result(std::move(*built));
    built->~Map();
    return result;
}

}  // namespace wh::guimodule
