#pragma once
#include <cstdint>
#include <cstddef>
#include <functional>

// -----------------------------------------------
// wh::animationmodule::C_CallbackAction -- Mannequin fragment action with lifecycle callbacks
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x268 (as the C_AlchemyAnimAction base; own exact
// allocation size UNVERIFIED).
// -----------------------------------------------
// RTTI .?AVC_CallbackAction@animationmodule@wh@@ (TD 0x4B493A8; name @0x184B493B8; sibling
// C_CallbackActionContext @0x184B42490).  Base chain (as compiled): C_CallbackAction :
// TAction<SAnimationContext> : CryMannequin IAction.  The engine IAction here does NOT match the
// stock SDK ICryMannequin.h layout closely enough to inherit it (KCD2-modified engine;
// interfuscator caution on Cry interfaces), so the object is modeled STANDALONE: the IAction/
// TAction base region +0x08..+0x90 is laid out from the byte-exact base init sub_180682DA8 and
// no virtuals are declared -- DO NOT call through this vtable from the header; use the dispatch/
// callback helpers below.
//
// Known vtable slots (C_AlchemyAnimAction vtable 0x183A47948; names TENTATIVE, interfuscator
// caution): [0] deleting dtor 0x180682C90 · [2] Exit 0x180682D28 (sets m_flags bit 0x20, fires
// m_onExit) · [4] Enter 0x180682CD4 (clears bit 0x20, fires m_onEnter) · [8] status/transition
// 0x181E3E120 · [12] Clone 0x18275D56C (allocs a fresh 0x90 TAction<SAnimationContext>) ·
// [14]/[17]/[20] delegate fires 0x180683010/0x180683144/0x180683084.
// Field order +0x20..+0x50 follows stock IAction (forcedScopeMask, priority, eStatus, flags,
// ..., fragmentID, fragTags 16B+4, optionIdx) -- fingerprinted by m_optionIdx's stock default
// OPTION_IDX_RANDOM (-2) at +0x50 in the base init.
//
// Helpers: keyed-param write sub_1806825C4(&m_paramArray, crc32Key, &value28B) appends/updates a
// 32-byte { int32 crcKey; byte[28] posrot } record (e.g. "TargetPos"); delegate reset
// sub_180682EA0; completion-callback install sub_180A96308(this, &fn, on) targets m_onEnter
// (+0xD0).  Dispatch to the player's Mannequin action controller: sub_1806C6C5C(playerCtx, &sp)
// -> (ctx+0x18 controller)->vtbl+0x98(action).

namespace wh::animationmodule {

class C_CallbackAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CallbackAction;

    // ---- IAction/TAction<SAnimationContext> base region (from sub_180682DA8) ----
    void*    _vtbl_dummy_do_not_use;   // +0x00 modeled explicitly: engine vtable (see banner -- no virtuals declared)
    uint64_t _pad08;                   // +0x08
    uint32_t _pad10;                   // +0x10
    float    m_activeTime;             // +0x14  init -1.0f [name TENTATIVE]
    uint64_t _pad18;                   // +0x18
    uint32_t m_forcedScopeMask;        // +0x20  init -1 (ACTION_SCOPES_ALL)
    int32_t  m_priority;               // +0x24  ctor arg a2 (autocook montage passes 5)
    uint32_t m_eStatus;                // +0x28  cleared by Enter (stock IAction::EStatus)
    uint32_t m_flags;                  // +0x2C  ctor arg a5; bit 0x20 = exited/stopping, 0x10 tested by [8]
    uint64_t _pad30;                   // +0x30
    int32_t  m_fragmentID;             // +0x38  ctor arg a3 -- Mannequin fragment id (runtime-resolved)
    uint8_t  m_tagState[16];           // +0x3C  Mannequin tag state (ctor arg, 16B)
    int32_t  m_tagStateExtra;          // +0x4C  ctor arg a4[4]
    int32_t  m_unk50;                  // +0x50  init -2
    int32_t  m_unk54;                  // +0x54  init 55432 (0xD888)
    uint32_t _pad58;                   // +0x58
    float    m_playbackWeight;         // +0x5C  init 1.0f [name TENTATIVE]
    float    m_playbackSpeed;          // +0x60  init 1.0f [name TENTATIVE]
    uint32_t _pad64;                   // +0x64
    // keyed 32-byte {crc32, posrot28} param records ("TargetPos" etc.; sub_1806825C4)
    void*    m_paramArrayHead;         // +0x68  init &unk_185666AA8 (empty-array sentinel)
    uint8_t  m_paramArrayState[0x18];  // +0x70  init xmmword_18409E678 + tail
    void*    m_paramArray2Head;        // +0x88  init &unk_185666AB8 (second keyed array)
    // ---- the 7 lifecycle std::function delegate slots (reset by sub_180682EA0) ----
    std::function<void(C_CallbackAction*)> m_onExit;       // +0x90  fired by [2] Exit when set
    std::function<void(C_CallbackAction*)> m_onEnter;      // +0xD0  fired by [4] Enter; completion cb installed here (sub_180A96308)
    std::function<void(C_CallbackAction*)> m_onDelegate2;  // +0x110  fired by [14] [role UNVERIFIED]
    std::function<void(C_CallbackAction*)> m_onDelegate3;  // +0x150  [role UNVERIFIED]
    std::function<void(C_CallbackAction*)> m_onDelegate4;  // +0x190  fired by [17] [role UNVERIFIED]
    std::function<void(C_CallbackAction*)> m_onDelegate5;  // +0x1D0  fired by [20] [role UNVERIFIED]
    std::function<void(C_CallbackAction*)> m_onDelegate6;  // +0x210  [role UNVERIFIED]
    uint8_t  _pad250[0x18];            // +0x250
};
static_assert(sizeof(std::function<void(C_CallbackAction*)>) == 0x40,
              "MSVC std::function must be 0x40 for the delegate-slot layout");
static_assert(sizeof(C_CallbackAction) == 0x268, "C_CallbackAction spans 0x268 (as AlchemyAnimAction base)");
static_assert(offsetof(C_CallbackAction, m_priority) == 0x24, "priority at 0x24");
static_assert(offsetof(C_CallbackAction, m_fragmentID) == 0x38, "fragmentID at 0x38");
static_assert(offsetof(C_CallbackAction, m_onEnter) == 0xD0, "completion delegate at 0xD0");

}  // namespace wh::animationmodule
