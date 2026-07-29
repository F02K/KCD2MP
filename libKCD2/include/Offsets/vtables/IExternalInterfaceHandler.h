#pragma once

// -----------------------------------------------
// IExternalInterfaceHandler -- CryEngine Scaleform ExternalInterface.call() sink.
// -----------------------------------------------
// SDK: CryCommon/IFlashPlayer.h (NEW vs KCD1). Global-namespace SDK struct;
// modelled here in Offsets:: with the other flash vtable replicas.
//
// CFlashUIElement implements it as its third base (+0x10). Sub-vtable @ 0x184049BA0,
// 2 slots. HandleExternalInterfaceCall (0x1805579A4) builds an SUIArguments from the
// call and broadcasts to the element's UIElement event listeners (element +0x150 list),
// optionally writing a return value back.
//
// [SIGNATURE UNVERIFIED] arg shape modelled from the decompile (method name + packed
// args); the exact SDK parameter list was not certified -- do not call blindly.

namespace Offsets {

struct IExternalInterfaceHandler {
    // [0] 0x1805579A4  a flash ExternalInterface.call(method, ...) -> element listeners.
    virtual void HandleExternalInterfaceCall(const char* pMethodName, const void* pArgs,
                                             unsigned int numArgs, void* pResult = nullptr) = 0;
protected:
    virtual ~IExternalInterfaceHandler() {}   // [1] this-adjustor dtor thunk 0x18213D97C
};

}  // namespace Offsets
