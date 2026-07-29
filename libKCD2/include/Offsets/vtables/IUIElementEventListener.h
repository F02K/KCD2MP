#pragma once
#include <cstdint>

// -----------------------------------------------
// IUIElementEventListener -- KCD2 binary vtable order (NOT interfuscated)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashUI.h
// (GLOBAL-namespace struct, untagged -> declaration order).
// RTTI: .?AUIUIElementEventListener@@.
//
// The PURE base interface vtable @ 0x183ED9418 (8 slots, this_off 0). Slots [0]-[6]
// are the SDK's empty default virtual impls (all folded onto nullsub_1
// 0x1803B6E80), slot [7] is the base scalar-deleting dtor (sub_182AFDA44, restores
// &IUIElementEventListener::vftable then frees if flags&1). 8 binary slots == the 8
// SDK declarations -- same layout and order as KCD1 (no inserted slot).
//
// Concrete implementers (e.g. the guimodule element listeners) override individual
// slots; the enumeration below is the interface contract. The KCD1 anchor for slot
// [0] OnUIEvent (the main Flash->C++ dispatch entry) and slot [6] OnInstanceDestroyed
// still hold; KCD2 debug strings are stripped so per-implementer bodies are matched
// elsewhere.
//
// [G4c VERIFIED] Slot [0] is a WARHORSE-MODIFIED signature: 4 formal params, NOT the
// stock CryEngine 3. Both CFlashUIElement dispatchers stage a 5th machine arg at
// [rsp+0x20] before `call [vtable+0]`: the return-value dispatch sub_1805579A4 passes
// a real buffer (call site 0x180557B16), the fire-and-forget dispatch sub_181179C9C
// passes literal 0 (call site 0x181179E40) -- hence a NULLABLE pointer. pResult points
// at a 16-byte TUIData the handler may write: C_UICommonEventHandler's "GetButtonId"
// (sub_180557F3C @0x180558017) stores { int32 type=4 @+0x00, int32 value @+0x08 }
// through it with NO null check. Return value is discarded by both dispatchers (void).
// Slots [1..6] verified/consistent with stock (OnUIEventEx already has 4 formals;
// OnUnload 2-arg and OnSetVisible 3-arg call-site-proven).

struct SUIArguments;   // FlashUI value type -- GLOBAL namespace (defined in guimodule/SUITypes.h)
struct SUIEventDesc;   // GLOBAL namespace (defined in guimodule/SUIEventDesc.h)

namespace Offsets {

struct IUIElement;
struct IFlashPlayer;

struct IUIElementEventListener {
    // pResult: nullable out-param -> 16-byte TUIData { int32 type; pad; value @+0x08 }
    // (type tag 4 observed = int; enum ordering vs EUIDataTypes UNVERIFIED).
    virtual void OnUIEvent(IUIElement* pSender, const SUIEventDesc& event, const SUIArguments& args, void* pResult) = 0;  // [0] 0x00  0x1803B6E80 (base default: empty)  [KCD2 4-param variant]
    virtual void OnUIEventEx(IUIElement* pSender, const char* fscommand, const SUIArguments& args, void* pUserData) = 0;  // [1] 0x08  0x1803B6E80 (base default: empty)
    virtual void OnInit(IUIElement* pSender, IFlashPlayer* pFlashPlayer) = 0;                                             // [2] 0x10  0x1803B6E80 (base default: empty)
    virtual void OnUnload(IUIElement* pSender) = 0;                                                                       // [3] 0x18  0x1803B6E80 (base default: empty)
    virtual void OnSetVisible(IUIElement* pSender, bool bVisible) = 0;                                                    // [4] 0x20  0x1803B6E80 (base default: empty)
    virtual void OnInstanceCreated(IUIElement* pSender, IUIElement* pNewInstance) = 0;                                    // [5] 0x28  0x1803B6E80 (base default: empty)
    virtual void OnInstanceDestroyed(IUIElement* pSender, IUIElement* pDeletedInstance) = 0;                              // [6] 0x30  0x1803B6E80 (base default: empty)
    virtual void Dtor(char flags) = 0;                                                                                    // [7] 0x38  0x182AFDA44  base scalar deleting dtor
};

}  // namespace Offsets
