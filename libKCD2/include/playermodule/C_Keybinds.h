#pragma once
#include <cstdint>
#include <unordered_map>
#include <boost/container/vector.hpp>
#include "Offsets/vtables/IInputEventListener.h"
#include "Offsets/vtables/IPlayerProfileListener.h"
#include "E_KeybindController.h"
#include "I_UIKeybinds.h"
#include "S_KeybindsGroup.h"
#include "S_KeybindsSuperaction.h"

// -----------------------------------------------
// wh::playermodule::C_Keybinds -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1F0.
// -----------------------------------------------
// RTTI .?AVC_Keybinds@playermodule@wh@@ (TD 0x184DD5588; COLs 0x18443B4E8 @+0x00 /
// 0x18443B4C0 @+0x08).  ctor sub_18124417C, dtor sub_182E13CD8 (deleting sub_182E14278).
// Allocated (0x1F0) in the C_PlayerModule ctor (sub_180BE9208 @0x180BE92E4) and owned at
// C_PlayerModule+0x40; reach it via *(*(S_GameContext+0x128)+0x40) (sub_1809155C8 singleton).
//
// Bases (RTTI BaseClassArray):
//   IInputEventListener @+0x00, vtable 0x183F60E18: [0] OnInputEvent = sub_1811E077C, the
//     REBIND CAPTURE SINK -- if m_rebindController != Invalid, validates the raw event
//     (deviceId in {0,13,527,1036}) and applies it via ChangeInput + notifies, then re-arms
//     Invalid; [1..3] default stubs 0x180838AE0 / 0x18066CD10 / 0x18041A6A0.  The pending
//     triple (+0x1D0/+0x1D8/+0x1E0) is armed through the Lua/UI "KeybindsRebindInput" path.
//   IPlayerProfileListener @+0x08, vtable 0x183F60DF8: [0] dtor thunk sub_18213AA94,
//     [1] SaveToProfile sub_180F68714 (serialize all superactions x3 controllers to the
//     "keybinds_settings" profile key), [2] LoadFromProfile sub_180F68820 (parse + rebuild
//     pending sets via sub_180F68D58, then CommitBindings per controller).
//
// Config: LoadConfig sub_180601FB4 loads Libs/Config/keybindSuperactions.xml; parser
// sub_1808FB4CC fills m_superactions / m_uiGroups / m_conflictGraph / m_actionToSuperactions
// (<conflict> groups are symmetric all-pairs, <include conflict=> flattened at load).
//
// Slot model per superaction+controller (S_KeybindsSuperactionSlots): A defaults / B pending
// / C applied.  Rebinds write B freely -- a conflicting key is NEVER rejected at capture.
// The ONLY conflict block is the settings-page commit gate calling HasConflicts():
// Apply handler sub_181F90490 and Back-confirm callback sub_181F91630 (lambdas capturing the
// C_UISettings screen) raise the OK-only ControlsConflict(13) dialog on true and commit on
// false.  Choke for conflicting-binds cheat = force HasConflicts -> false (RVA 0x2E16AA4,
// xref-closed: sole consumers are the two gates via wrapper sub_182BA6B24).  Full flow doc:
// KCD2/analysis/keybind_conflicts_re.md.
//
// Internals not forwarded (call-chain documentation): NotifyChangeListener sub_182E18A10
// (m_pChangeListener slot [1] ChangeInput, arg1 = controller device bitflag),
// NotifyConflictFlag sub_182E188DC (slot [0] SetConflict), per-superaction conflict refresh
// sub_180F68F64, action-map (un)register sub_180A111BC(mgr, actionSet, controller, vec,
// 1=register/0=unregister), profile-XML apply sub_180F68D58, superaction lookup
// sub_182E1AA3C / find-or-insert sub_182E140A8 (on m_superactions).

namespace wh::playermodule {

class C_Keybinds
    : public Offsets::IInputEventListener      // +0x00  vtable 0x183F60E18
    , public Offsets::IPlayerProfileListener   // +0x08  vtable 0x183F60DF8
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Keybinds;

    // True if any input in m_inputToSuperactions[controller] is held by two superactions the conflict graph pairs up.
    bool HasConflicts(E_KeybindController::Type controller) const;                                   // 0x182E16AA4
    // Rebind: pendingInputs[slot] = input (gated on m_type == Writeable); re-indexes m_inputToSuperactions and refreshes conflict flags both ways.  Empty input = unbind.
    void ChangeInput(E_KeybindController::Type controller, CryStringT<char> superaction,
                     int32_t slot, CryStringT<char> input);                                          // 0x182E181A4
    // Single-row reset: ChangeInput(superaction, slot, defaultInputs[slot] or empty).
    void ResetSlotToDefault(E_KeybindController::Type controller, CryStringT<char> superaction,
                            int32_t slot);                                                           // 0x182E185BC
    // Every superaction: pending = defaults slot-wise, refresh conflicts, then CommitBindings(controller, true).  The page Reset button lands here.
    void ResetToDefaults(E_KeybindController::Type controller);                                      // 0x182E18448
    // Unregister the applied set from the action maps, applied = copy(pending), register; saveProfile also persists to the user profile.
    void CommitBindings(E_KeybindController::Type controller, bool saveProfile);                     // 0x180F68DC8
    // Cross-check the input's users against m_conflictGraph; SET the UI conflict flag on every conflicting pair.
    void SetConflictsForInput(E_KeybindController::Type controller, CryStringT<char> input);         // 0x182E18AC4
    // Same walk; CLEAR the UI conflict flags instead.
    void ClearConflictsForInput(E_KeybindController::Type controller, CryStringT<char> input);       // 0x182E19384

    // superaction name -> record (node 0x140; lookup sub_182E1AA3C, find-or-insert sub_182E140A8)
    std::unordered_map<CryStringT<char>, S_KeybindsSuperaction> m_superactions;                      // +0x10
    // ui_group name -> section row (find-or-insert sub_182E14088)
    std::unordered_map<CryStringT<char>, S_KeybindsGroup> m_uiGroups;                                // +0x50
    // superaction -> SORTED adjacency of conflicting superactions (symmetric, self-inclusive)
    std::unordered_map<CryStringT<char>, boost::container::vector<CryStringT<char>>> m_conflictGraph;         // +0x90
    // per controller: input name -> SORTED superactions currently bound to it (HasConflicts scans this)
    std::unordered_map<CryStringT<char>, boost::container::vector<CryStringT<char>>> m_inputToSuperactions[3]; // +0xD0
    // engine action name -> SORTED superactions referencing it (<action> reverse index)
    std::unordered_map<CryStringT<char>, boost::container::vector<CryStringT<char>>> m_actionToSuperactions;  // +0x190
    E_KeybindController::Type m_rebindController;  // +0x1D0  armed rebind target; Invalid when idle (ctor 0x18124427E; reader OnInputEvent 0x1811E07A0)
    uint32_t                  _pad1D4;             // +0x1D4  never written -- modeled as padding (UNVERIFIED)
    CryStringT<char>          m_rebindSuperaction; // +0x1D8  armed rebind superaction name
    int32_t                   m_rebindSlot;        // +0x1E0  armed rebind slot index
    uint32_t                  _pad1E4;             // +0x1E4  never written -- modeled as padding (UNVERIFIED)
    I_UIKeybinds*             m_pChangeListener;   // +0x1E8  C_UIKeybinds' I_UIKeybinds subobject (+0x58); NOT owned; set by C_UIKeybinds::Init sub_1806127D4, cleared by Deinit sub_182BA6F48
};
static_assert(sizeof(C_Keybinds) == 0x1F0, "C_Keybinds must be 0x1F0");
static_assert(offsetof(C_Keybinds, m_superactions) == 0x10, "superaction map at 0x10");
static_assert(offsetof(C_Keybinds, m_conflictGraph) == 0x90, "conflict graph at 0x90");
static_assert(offsetof(C_Keybinds, m_inputToSuperactions) == 0xD0, "input index array at 0xD0");
static_assert(offsetof(C_Keybinds, m_rebindController) == 0x1D0, "rebind controller at 0x1D0");
static_assert(offsetof(C_Keybinds, m_pChangeListener) == 0x1E8, "UI listener at 0x1E8");

}  // namespace wh::playermodule
