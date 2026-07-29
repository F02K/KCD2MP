#pragma once
#include <cstdint>

// -----------------------------------------------
// IActionMap -- one named CryAction action map, KCD2 binary vtable order (impl CActionMap).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6.  Vtable 0x184048530 (COL 0x1845E8D38), exactly 31 slots -- ALL
// decompiled and mapped 2026-07-20.  Instances: 0x90-byte objects created by
// CActionMapManager::CreateActionMap [16] (pool-alloc + ctor 0x180A1436C); obtained via
// IActionMapManager::GetActionMap [19].
//
// Object layout (from the method bodies): +0x08 flag8 (Reset recomputes = name=="default"),
// +0x09 m_enabled, +0x0A m_exclusivity, +0x0C m_priority (int, resolved from the profile's
// <priorities> table), +0x10 CActionMapManager* back-ptr, +0x18 action rb-tree (node key
// @+0x20 = ActionId qword, payload @+0x28), +0x20 tree size, +0x28 m_listenerEntityId,
// +0x58 m_name (CryStringT data ptr), +0x60 rebound-input count.
//
// ActionId = CCryName: one qword holding a POOLED char* (header with refcount at str-12).
// Construct with 0x1823C92D8(&id, cstr) [REL 306489], release with 0x1808E5C94(_, id)
// [REL 48736].  NEVER pass a plain string literal as an ActionId -- the copy paths AddRef
// through the pool header in front of the data.
//
// SActionInput (KCD2) = 0x108 bytes.  Defaults ctor 0x180A10F34 [REL 55229], dtor
// 0x180A11A74 [REL 55244] (frees the two name strings' heap spills + the block vector).
//   +0x00 int deviceId (AddInputDeviceMapping ids: keyboard=1, xboxpad=2, pspad=0x10)
//   +0x08 input, +0x40 defaultInput -- 0x38-byte stack-strings {size,cap=0x1F,ptr,SSO[0x20]};
//         assign with 0x180A11F14(member, &cstr) [REL 55255]
//   +0x78 inputBlockMode  +0x80 inputsToBlock vec  +0x88 blockTime  +0x8C blockActivation
//   +0x90 blockDeviceIndex byte (+0x91 all-devices flag)  +0x98 inputCRC (auto-filled)
//   +0xA8 pressTriggerDelay  +0xB0 pressTriggerDelayRepeatOverride(-1.0)
//   +0xC0 analogCompareVal  +0xC8 holdTriggerDelay  +0xD8 releaseTriggerThreshold(-1.0)
//   +0xE0 holdRepeatDelay  +0xE8 holdTriggerDelayRepeatOverride(-1.0)
//   +0xF0 activationFlags (EActionActivationMode: onPress=1 onRelease=2 onHold=4 always=8
//         retriggerable=0x10 noModifiers=0x20 consoleCmd=0x40; attr->bit map 0x180A132E7)
//   +0xF4 pressDelayPriority  +0xFC analogCompareOp

namespace Offsets {

struct IActionMap {
    virtual void Dtor(char flags) = 0;  // [0] 0x183560A64  scalar deleting dtor
    virtual void GetMemoryUsage(void* pSizer) = 0;  // [1] 0x183571E18  sizer walk over the action tree
    virtual void Release() = 0;  // [2] 0x18357F2A4  = mgr->RemoveActionMap(m_name)
    virtual void Clear() = 0;  // [3] 0x1835648F4  unbind-all + tree clear + counts 0
    // [4] 0x180A11610 -- rb-tree find by ActionId, NULL when absent (returns node+0x28).
    virtual void* GetAction(const void* actionId) = 0;
    virtual void* GetAction_(const void* actionId) = 0;  // [5] 0x180A10A24  const twin via lower_bound helper 0x180A10AC0
    // [6] 0x1835652B4 -- "Unable to create action, %s already exists" (warn + false when present).
    virtual bool CreateAction(const void* actionId) = 0;
    virtual bool RemoveAction(const void* actionId) = 0;  // [7] 0x18357F5C8  "Failed to find action: %s"
    virtual int GetActionsCount() = 0;  // [8] 0x181A727F0  tree size @+0x20
    // [9] 0x183561A4C -- add WITHOUT binding to the live input rows (creates the action if
    // missing); index = insertion position (-1 append) [conf MED on index semantics].
    virtual bool AddActionInput(const void* actionId, const void* pActionInput, int index) = 0;
    // [10] 0x180A10D00 -- THE one-call add: creates the action if missing ("Failed to create
    // action: %s"), deep-copies the 0x108 SActionInput into a pool alloc (auto-fills inputCRC
    // from the input string, rejects duplicate inputs), then binds it into the manager's
    // dispatch (0x180A11B7C).  VERIFIED end-to-end -- the internals are exactly what
    // LoadFromXMLNode [16] drives per <action> child.
    virtual bool AddAndBindActionInput(const void* actionId, const void* pActionInput) = 0;
    virtual bool RemoveActionInput(const void* actionId, const char* szInput) = 0;  // [11] 0x180A1312C  by lowercase-CRC of input
    virtual bool ReBindActionInput_Device(const void* actionId, const char* szNewInput, int device, int byDeviceIndex) = 0;  // [12] 0x18357DE9C
    virtual bool ReBindActionInput(const void* actionId, const char* szCurrentInput, const char* szNewInput) = 0;  // [13] 0x180A12CA4
    virtual int GetNumRebindedInputs() = 0;  // [14] 0x181A74B40  count @+0x60 [conf MED]
    virtual bool Reset() = 0;  // [15] 0x183580A1C  every input := defaultInput; +0x08 := (name=="default")
    // [16] 0x180A12514 -- parse one <actionmap> XML node into THIS map.  VERIFIED additive:
    // no clear; "priority"/"exclusivity" attrs only update when present; per <action> child:
    // GetAction -> exists? warn "Can't add action" + skip : create + parse activation attrs
    // (0x180A132B4) + AddAndBind one input per device-mapping attr (keyboard=/xboxpad=/...).
    // The node's own "name" attr is not read.  pXmlNodeRef = IXmlNode**.
    virtual bool LoadFromXMLNode(void* pXmlNodeRef) = 0;
    virtual bool LoadRebindingDataFromXML(void* pXmlNodeRef) = 0;  // [17] 0x183575F68  <action name><rebind device input .../>
    virtual bool SaveRebindingDataToXML(void* pXmlNodeRef) = 0;  // [18] 0x183582EB0  writes name attr + action/rebind children
    virtual void* CreateActionIterator(void** out) = 0;  // [19] 0x180A13C20  0x20-byte iterator (vtbl 0x183A6B320) over the action tree [conf MED]
    virtual void SetActionListener(uint32_t entityId) = 0;  // [20] 0x181AAEC00  +0x28 [conf MED]
    virtual uint32_t GetActionListener() = 0;  // [21] 0x180836120  +0x28 [conf MED]
    virtual const char* GetName() = 0;  // [22] 0x181A7F820  m_name data ptr @+0x58 (IDA FLIRT mislabel "GetRootCollection")
    virtual void SetFlag8(bool v) = 0;  // [23] 0x18085AB38  byte +0x08 [role UNVERIFIED -- Reset writes name=="default" here]
    virtual bool GetFlag8() = 0;  // [24] 0x181A7DE10  byte +0x08 [role UNVERIFIED]
    // [25] 0x18085ADF4 -- byte +0x09; disable releases per-action input state + filtered
    // actions, enable pokes mgr+0x48; both fire the type-3 action-map event (0x18085B248)
    // that C_ActionSets::OnActionMapEvent rebuilds its active set from.
    virtual void Enable(bool enable) = 0;
    virtual bool Enabled() = 0;  // [26] 0x181A88910  byte +0x09
    virtual int GetPriority() = 0;  // [27] 0x181A76130  +0x0C
    virtual bool GetExclusivity() = 0;  // [28] 0x181AAEB60  byte +0x0A
    virtual void* _vf29() = 0;  // [29] 0x181A7FAB0  returns &this->+0x68 [member UNIDENTIFIED]
    virtual void* _vf30() = 0;  // [30] 0x181A846E0  returns &this->+0x78 [member UNIDENTIFIED]
};

}  // namespace Offsets
