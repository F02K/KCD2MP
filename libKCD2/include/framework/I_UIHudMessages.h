#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::framework::I_UIHudMessages -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 7 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudMessages@framework@wh@@ (TD 0x184CD0AC8). Framework-side face of the
// info-text / tutorial-message HUD. C_UIHudMessages implements it @+0x58 (vtable
// 0x183D2EA98): 4 methods + RTTR trio thunks [4..6] (0x18213A680/0x18213A5FC/0x18213A5CC).
// No virtual dtor slot. Slot bodies (C_UIHudMessages impls):
//   [0] 0x1812C15EC  flash info-text call (CryStringT text, int, int) -- matches the rttr
//       method_wrapper void(CryStringT const&,int,int) registered on C_UIHudMessages
//   [1] 0x180C44E70  flash "HideInfoText"()
//   [2] 0x181A8E8E0  returns &owner.m_messageTexts[id] (the CryStringT[12] cache @+0x110:
//       `return subobj + 8*(id+23)`)
//   [3] 0x182B8D9AC  returns a CryStringT* selected by a message-type flag word
//       (0x8/0x2/0x100 -> statics unk_18557F050/unk_18557F048/unk_18557F058; 0x4 ->
//       from ctx+0x50 via sub_180B40F98(,2); else magic-static default)
// Method names coined from behavior/flash names.

namespace wh::framework {

class I_UIHudMessages {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudMessages;
    virtual void ShowInfoText(const CryStringT<char>& text, int a3, int a4) = 0;   // [0] 0x1812C15EC (tail param roles UNVERIFIED)
    virtual void HideInfoText() = 0;                                               // [1] 0x180C44E70 flash "HideInfoText"
    virtual CryStringT<char>* GetMessageText(uint8_t messageId) = 0;               // [2] 0x181A8E8E0 -> &m_messageTexts[messageId]
    virtual CryStringT<char>* GetMessageSound(uint16_t typeFlags) = 0;             // [3] 0x182B8D9AC (name coined, semantics partial)
    RTTR_ENABLE()  // [4..6] thunks -0x58; baseless (creator 0x18185C950); in C_UIHudMessages's RTTR_ENABLE(C_UIBase, I_UIHudMessages)
};
static_assert(sizeof(I_UIHudMessages) == 8);

}  // namespace wh::framework
