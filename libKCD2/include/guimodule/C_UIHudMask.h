#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashBase.h"
#include "framework/E_HudElements.h"
#include "framework/I_UIHudMask.h"
#include "framework/I_SourceMonitorListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudMask -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x320.
// -----------------------------------------------
// RTTI .?AVC_UIHudMask@guimodule@wh@@ (TD 0x184CD0C10).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135FE0), IUIElementEventListener @+0x10
// (COL 0x4136008), framework::I_UIHudMask @+0x58 (COL 0x4136030),
// framework::I_SourceMonitorListener @+0x60 (COL 0x4136058).
// vtables: primary 0x183C13DD8 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C13D90 (8, [2] override 0x180C3BE5C = refresh-all thunk), mask
// 0x183C13D60 (5), source-monitor 0x183C13D50 (1). ctor sub_1817944DC; rttr
// as_std_shared_ptr creator sub_1817943BC (alloc 0x320).
// SINGLETON-STYLE GLOBAL: ctor publishes `this` at qword_18548BFA8 (RVA 0x548BFA8).
//
// ======================= THE KCD2 HUD-VISIBILITY / LETTERBOX LEVER =======================
// Replaces KCD1's per-element hide-REFCOUNT array (C_UIEHud+0x50 int[24], bit 20 =
// RatioStrips). KCD2 model, fully verified:
//   * 28 named elements (framework E_HudElements, RatioStrips = 22).
//   * Per-element visibility RULE table m_elementRules[28] @+0x158 (16B each: two
//     packed 51-bit UI-SOURCE masks; ctor default {0x0007FFFFFFFFFFFF, 0} via
//     sub_180BC67C0 = visible everywhere; per-element constants baked by the ctor
//     tail sub_1817945A8 through the mask-builder sub_181E25030).
//   * The framework "UI source monitor" (*(lazy ctx sub_1809155C8()+0xD8)) tracks which
//     named/numbered UI sources are active (dialog, menus, cutscene, ...). Its
//     vf[+0x10](ruleMask&)->bool evaluates a rule against the active source set;
//     vf[+0x18](sourceId)->bool tests one source.
//   * Refresh (per element id, lambda _Do_call 0x181F7C940): visible =
//     monitor->vf[+0x10](m_elementRules[id]); stored in the 28-bit set m_visibleBits
//     @+0x150 (writer sub_180556744); pushed to the "hud" movie per-element clip
//     (sub_180555A10: element NAME string + bool through the IFlashVariableObject
//     layer, elem vf[+0x20] + invoker sub_18041C37C). Runs for all 28 ids
//     (sub_180C3BF10) from Init sub_180C3CBCC, refresh-all sub_180C3BE68 (module msg
//     52 / listener [2]), and per-source-change.
//   * I_SourceMonitorListener callback sub_180555978: source NAME -> element id
//     (sub_180556410), flips the bit + pushes visibility + re-evaluates the letterbox.
//   * IsElementVisible (I_UIHudMask[1] sub_180555E04) = m_visibleBits[id] &&
//     m_elementSources[id]->vf[+0x10]() (per-element handle array @+0x70).
//   * wh_ui_ShowHud CVar (cached @+0x68; change-callback sub_182B8E3A0) toggles the
//     WHOLE "hud" element via IUIElement::SetVisible (vf+0xE0).
//
// LETTERBOX ("RatioStrips", element 22): the ratio-preset selector sub_1805561F0
// queries the source monitor -- source 4 (+in-dialog check via module+0x58 obj) -> 4,
// source 6 -> 1, source 20 -> 5, source 13 -> 2, source 14 -> 3, else 0 -- caches the
// preset at m_ratioPreset (+0x318) and on change flash-calls "SetRatioPreset"(int)
// (the AS-side moves the strips), plus "SetDialogShiftMovieClips"(bool) for sources
// 14/8. MODDING RECIPES: (a) force m_ratioPreset writes to 0 / hook sub_1805561F0 ->
// no letterbox anywhere; (b) clear bit 22 of m_visibleBits + rule[22] = {0,0} to kill
// the strips clip; (c) wh_ui_ShowHud 0 hides the whole HUD. Unlike KCD1 there is NO
// refcount to pin -- state is re-derived from the source monitor on every change.
// Full writeup: analysis/guimodule_port/hud_family.md.

struct ICVar;

namespace wh::guimodule {

class C_UIHudMask : public C_UIFlashBase,
                    public wh::framework::I_UIHudMask,
                    public wh::framework::I_SourceMonitorListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudMask;

    // Per-element visibility rule: two packed 51-bit UI-source masks (bit i = source i).
    // Which of the two qwords is the show vs hide set is UNVERIFIED (the evaluator
    // monitor->vf[+0x10] consumes the pair opaquely).
    struct S_HudElementRule {
        uint64_t m_sourceMaskA;   // +0x00  ctor default 0x0007FFFFFFFFFFFF (all 51 sources)
        uint64_t m_sourceMaskB;   // +0x08  ctor default 0
    };
    static_assert(sizeof(S_HudElementRule) == 0x10);

    // C_UIBase/C_UIFlashBase overrides.
    void Init(C_GUIModule* pModule) override;    // [1] 0x180C3CBCC  (doc above)
    void Deinit() override;                      // [2] 0x182B8CDFC
    void OnModuleMessage(void* msg) override;    // [3] 0x18085DD98  msg 52 -> refresh-all sub_180C3BE68
    void Update() override;                      // [4] 0x180533BD0  base drain + ratio-preset re-evaluate sub_1805561F0
    // I_UIHudMask override.
    bool IsElementVisible(wh::framework::E_HudElements::Type element) const override;   // [M1] 0x180555E04
    // I_SourceMonitorListener override.
    void OnSourceEvent(void* a2, bool bActive) override;   // [S0] 0x180555978  source name -> element bit + flash push

    // ---- Tool surface (non-virtual; impls in src/guimodule/C_UIHudMask.cpp) ----
    // The live "hud" mask screen -- ctor-published global qword_18548BFA8 (the KCD2
    // counterpart of KCD1's C_UIEHud global qword_183785948).
    static C_UIHudMask* GetInstance();
    // E_HudElements converters (the strcmp chain / switch the mask itself uses).
    static bool ElementNameToId(const char* name, wh::framework::E_HudElements::Type& outId);  // sub_180556410
    static const char* ElementIdToName(wh::framework::E_HudElements::Type id);                 // sub_180555AC0
    // Re-apply wh_ui_ShowHud + re-evaluate all 28 element rules + letterbox (module
    // msg 52 / listener [2] path). Undoes any direct m_visibleBits/m_ratioPreset pokes.
    void RefreshAll();                                                                         // sub_180C3BE68
    // Ratio-preset (letterbox) selector alone: source monitor -> preset, flash-calls
    // "SetRatioPreset"/"SetDialogShiftMovieClips" on change of m_ratioPreset.
    void UpdateRatioPreset();                                                                  // sub_1805561F0
    // Push one element's CURRENT visibility (I_UIHudMask query) to its "hud" movieclip.
    void PushElementVisibility(wh::framework::E_HudElements::Type id);                         // sub_180555A10

    ICVar*  m_pShowHudCVar;                  // +0x68  wh_ui_ShowHud (Init @0x180c3cc59; callback sub_182B8E3A0)
    ICVar*  m_elementSources[28];            // +0x70  per-element "wh_ui_Show<Element>" int CVars (RegisterInt sub_180B9512C(name,1,0x8000,help,0) @0x181F5E474; store @0x181F5E484); IsElementVisible reads (*vf[+0x10])()!=0 @0x180555e3d (GetIVal); on-change cb sub_182B8E15C (GetName vf[+0x78] -> ElementNameToId)
    uint32_t m_visibleBits;                  // +0x150 28-bit per-element visibility set (std::bitset<28>; writer sub_180556744)
    uint8_t  _pad154[4];                     // +0x154
    S_HudElementRule m_elementRules[28];     // +0x158 per-element source rules (ctor tail sub_1817945A8)
    uint8_t  m_ratioPreset;                  // +0x318 cached "SetRatioPreset" arg (0 = fullscreen, no strips; writer @0x1805562a3)
    uint8_t  _pad319[7];                     // +0x319
};
static_assert(sizeof(C_UIHudMask) == 0x320, "C_UIHudMask must be 0x320");
static_assert(offsetof(C_UIHudMask, m_elementSources) == 0x70, "source handles at 0x70 (memset 0xE0 @0x18179458c)");
static_assert(offsetof(C_UIHudMask, m_visibleBits) == 0x150, "visibility bitset at 0x150");
static_assert(offsetof(C_UIHudMask, m_elementRules) == 0x158, "rule table at 0x158 (init 16x28 @0x18179453e)");
static_assert(offsetof(C_UIHudMask, m_ratioPreset) == 0x318, "ratio preset cache at 0x318");

}  // namespace wh::guimodule
