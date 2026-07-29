#pragma once
#include <cstdint>

// -----------------------------------------------
// IGameFrameworkListener -- CryAction framework listener, KCD2 slot order (from broadcast sites).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). LISTENER interface: game code implements it, CCryAction
// calls it. Registry: delete-safe set at CCryAction+0x758 (stride 0x68, listener @+0x00,
// active-bitmask @+0x768). Slot indices read from the broadcast loops (evidence table in
// KCD2/analysis/vtables_reslot/IInput_IActionMap_IGFListener.md).
//
// [vs KCD1] NO reshuffle -- every recovered event keeps its KCD1 slot. Only [8]
// OnForceLoadingWithFlash has no broadcast site in this build (slot inferred from SDK order).
// Note: OnPreRender and OnPostUpdate are broadcast from ONE merged frame fn (0x1820E1050;
// pre-render loop first, post-update loop after the render work).

namespace Offsets {

struct IGameFrameworkListener {
    virtual void Dtor() = 0;  // [0] standard dtor slot
    virtual void OnPostUpdate(float fDeltaTime) = 0;  // [1] broadcast 0x1820E1050 2nd loop; dt = CCryAction+0x18
    virtual void OnSaveGame(void* pSaveGame) = 0;  // [2] broadcast 0x1835766A0 (save serializer sub_183714DA8)
    virtual void OnLoadGame(void* pLoadGame) = 0;  // [3] broadcast 0x18357657C (load deserialize stage)
    virtual void OnLevelEnd(const char* nextLevel) = 0;  // [4] broadcast 0x1812750D4 ("CCryAction:LevelEnd")
    virtual void OnActionEvent(const void* event) = 0;  // [5] broadcast 0x180668D48 (event codes 21/22/27)
    virtual void OnPreRender() = 0;  // [6] broadcast 0x1820E1050 1st loop
    virtual void OnSavegameFileLoadedInMemory(const char* levelName) = 0;  // [7] broadcast 0x1835768E0 (early load path)
    virtual void OnForceLoadingWithFlash() = 0;  // [8] NO broadcast site found in 1.5.6 -- slot per KCD1/SDK order [UNVERIFIED]
};

}  // namespace Offsets
