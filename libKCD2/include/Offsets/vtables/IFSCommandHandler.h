#pragma once

// -----------------------------------------------
// IFSCommandHandler -- CryEngine Scaleform fscommand() callback sink.
// -----------------------------------------------
// SDK: CryCommon/IFlashPlayer.h. Global-namespace SDK struct; modelled here in
// Offsets:: alongside the other flash vtable replicas.
//
// CFlashUIElement is the (only observed) implementor: it is the element's second
// base (+0x08). Its sub-vtable @ 0x184049BB8 has 2 slots. HandleFSCommand
// (0x181179C9C) packs the command+args into an SUIArguments and broadcasts to the
// element's registered UIElement event listeners (element +0x158 list).

namespace Offsets {

struct IFSCommandHandler {
    // [0] 0x181179C9C  a flash fscommand("cmd","args") -> element event listeners.
    virtual void HandleFSCommand(const char* pCommand, const char* pArgs, void* pUserData = nullptr) = 0;
protected:
    virtual ~IFSCommandHandler() {}   // [1] this-adjustor dtor thunk 0x18213D970
};

}  // namespace Offsets
