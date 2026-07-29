#pragma once
#include <cstdint>

// -----------------------------------------------
// IActionMapManager -- CryAction action maps, KCD2 binary vtable order (impl CActionMapManager).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  Primary vtable 0x184048018, 63 slots (KCD1: 56).
// Reached via CCryAction+0x530 (IGameFramework slot [28] GetIActionMapManager).
// Full evidence table: KCD2/analysis/vtables_reslot/IInput_IActionMap_IGFListener.md.
// Unnamed _vfN slots are real functions whose role is UNVERIFIED -- do not call them.
//
// [FUNDAMENTAL vs KCD1] All KCD1 methods survive; four KCD2-inserted slots ([12] dispatch,
// [20] GetActionMap-by-CRC, [25] name-from-maps, [32] filter query) slide the block:
// EnableActionMap 27->29, EnableFilter 28->30, FilterAction 29->31, IsFilterEnabled 30->33;
// KCD1 tail [22..52] keeps relative order at +2..+3. KCD1's const GetActionMap [53] is
// replaced by GetNameAllActionMapsIncludesThis [56]; WH extras [57..60] precede the clears.

namespace Offsets {

struct IActionMapManager {
    virtual void Dtor(char flags) = 0;  // [0] 0x183560ACC  scalar deleting dtor
    virtual void Update() = 0;  // [1] 0x1808E4C60  gated by +0xFC [KCD1 1]
    virtual void Reset() = 0;  // [2] 0x183580B14  per-map reset [KCD1 2, conf MED]
    virtual void Clear() = 0;  // [3] 0x18173CE54  clears containers @+0x18 [KCD1 3, conf MED]
    virtual bool InitActionMaps(const char* filename) = 0;  // [4] 0x183573BFC  "InitActionMaps: Failed to open %s" [KCD1 4]
    virtual void SetLoadFromXMLPath(const char* path) = 0;  // [5] 0x183584EC4  stores CryString @+0x40 [KCD1 5]
    virtual const char* GetLoadFromXMLPath() = 0;  // [6] 0x181A8B5A0  returns +0x40 [KCD1 6]
    virtual bool LoadFromXML(void* xmlNode) = 0;  // [7] 0x180A13E3C  version/priorities/actionmap/actionfilter [KCD1 7]
    virtual bool LoadRebindDataFromXML(void* xmlNode) = 0;  // [8] 0x183575DE0  [KCD1 8]
    virtual bool SaveRebindDataToXML(void* xmlNode) = 0;  // [9] 0x183582DDC  [KCD1 9]
    virtual void AddExtraActionListener(void* l, const char* actionMap) = 0;  // [10] 0x180562E20  map @+0x50 (named) or list @+0x18 [KCD1 10]
    virtual void RemoveExtraActionListener(void* l, const char* actionMap) = 0;  // [11] 0x180562D28  mirror of [10] [KCD1 11]
    virtual void _vf12() = 0;  // [12]
    virtual void AddAlwaysActionListener(void* l) = 0;  // [13] 0x183561ED8  refcounted list @+0xB0 [KCD1 13]
    virtual void RemoveAlwaysActionListener(void* l) = 0;  // [14] 0x18357F82C  [KCD1 14]
    virtual void RemoveAllAlwaysActionListeners() = 0;  // [15] 0x18357F77C  [KCD1 15]
    virtual void* CreateActionMap(const char* name) = 0;  // [16] 0x180A141C4  "CreateActionMap: ... already exists" [KCD1 16]
    virtual bool RemoveActionMap(const char* name) = 0;  // [17] 0x18357F6E4  [KCD1 17]
    virtual void RemoveAllActionMaps() = 0;  // [18] 0x18173CF70  clears @+0x50 & @+0x90 [KCD1 18]
    virtual void* GetActionMap(const char* name) = 0;  // [19] 0x180A10AF4  map @+0x50 lookup -> *(entry+0x28) [KCD1 19]
    virtual void* GetActionMapByCRC(uint32_t crc) = 0;  // [20] 0x1803F7B08  KCD2-INSERTED (TLS-guarded) [conf LOW]
    virtual void* CreateActionFilter(const char* name, int type) = 0;  // [21] 0x18079A728  88B filter, map @+0x60 [KCD1 20, conf MED]
    virtual void* GetActionFilter(const char* name) = 0;  // [22] 0x18079A7BC  filter map @+0x60 [KCD1 21, conf MED]
    virtual void* CreateActionMapIterator() = 0;  // [23] 0x180602518  CGenericPtrMapIterator over @+0x50 [KCD1 22]
    virtual void* CreateActionFilterIterator() = 0;  // [24] 0x183565388  over @+0x60 [KCD1 23]
    virtual void _vf25() = 0;  // [25]
    virtual const void* GetActionInput(const char* action) = 0;  // [26] 0x180A10970  "...accross in all actionmap" [KCD1 24]
    virtual const void* GetActionInputByMap(const char* map, const char* action) = 0;  // [27] 0x180A10908  "...in actionmap: %s" [KCD1 25]
    virtual void Enable(bool enable, bool resetStateOnDisable) = 0;  // [28] 0x1806025B8  +0xFC; clears input state [KCD1 26]
    virtual void EnableActionMap(const char* name, bool enable) = 0;  // [29] 0x18085AB44  "Missing action map '%s'"; empty = all [KCD1 27]
    virtual void EnableFilter(const char* name, bool enable) = 0;  // [30] 0x1806169B8  "Missing action map filter '%s'" [KCD1 28]
    virtual bool FilterAction(const void* action) = 0;  // [31] 0x1806169D4  filter+0x40 sweep [KCD1 29, conf MED]
    virtual void _vf32() = 0;  // [32]
    virtual bool IsFilterEnabled(const char* name) = 0;  // [33] 0x1835742A4  filter map @+0x60 -> filter+0x50 [KCD1 30]
    virtual void ReleaseFilteredActions() = 0;  // [34] 0x18085AEB0  [KCD1 31, conf MED]
    virtual void ClearStoredCurrentInputData() = 0;  // [35] 0x181AAE840  +0xE8=-1, +0xFF=0 [KCD1 32]
    virtual bool ReBindActionInput(const char* map, const char* action, const char* oldInput, const char* newInput) = 0;  // [36] 0x18357DF2C  [KCD1 33]
    virtual int GetVersion() = 0;  // [37] 0x181A74B90  +0xEC [KCD1 34]
    virtual void SetVersion(int v) = 0;  // [38] 0x181AAED30  +0xEC [KCD1 35]
    virtual void EnumerateActions(void* enumerator) = 0;  // [39] 0x18356CF78  callback per action name [KCD1 36]
    virtual int GetActionsCount() = 0;  // [40] 0x183570D4C  sum of map+0x40 [KCD1 37]
    virtual int GetActionMapsCount() = 0;  // [41] 0x181A783D0  +0x58 [KCD1 38]
    virtual bool AddInputDeviceMapping(int deviceType, const char* deviceName) = 0;  // [42] 0x18173CCC0  [KCD1 39]
    virtual bool RemoveInputDeviceMapping(int deviceType) = 0;  // [43] 0x18357FC84  [KCD1 40]
    virtual void ClearInputDevicesMappings() = 0;  // [44] 0x183564954  vector @+0xD0 [KCD1 41]
    virtual int GetNumInputDeviceData() = 0;  // [45] 0x18169F208  (end-begin)/0x30 @+0xD0 [KCD1 42]
    virtual const void* GetInputDeviceDataByIndex(int i) = 0;  // [46] 0x180A12DE8  [KCD1 43]
    virtual const void* GetInputDeviceDataByTypeName(const char* type) = 0;  // [47] 0x1815BB2EC  [KCD1 44]
    virtual const void* GetInputDeviceDataByType(int type) = 0;  // [48] 0x1835717C4  [KCD1 45]
    virtual void RemoveAllRefireData() = 0;  // [49] 0x180616D28  filters @+0xA0 [KCD1 46, conf MED]
    virtual bool LoadControllerLayoutFile(const char* layoutKey) = 0;  // [50] 0x183575260  [KCD1 47]
    virtual uint32_t GetDefaultActionEntity() = 0;  // [51] 0x181AAE920  dword +0x118 [KCD1 48]
    virtual void SetDefaultActionEntity(uint32_t id, bool updateAll) = 0;  // [52] 0x180602470  +0x118 + listener notify [KCD1 49]
    virtual void RegisterActionMapEventListener(void* l) = 0;  // [53] 0x180C3D524  list @+0x108 [KCD1 50]
    virtual void UnregisterActionMapEventListener(void* l) = 0;  // [54] 0x1835875BC  [KCD1 51]
    virtual bool ActionFiltered(const void* action) = 0;  // [55] 0x1803F7CAC  filters @+0x60 sweep [KCD1 52]
    virtual const char* GetNameAllActionMapsIncludesThis(const char* actionMap) = 0;  // [56] 0x1803F78D8  KCD2 replacement of KCD1 [53] [conf MED]
    virtual void _vf57() = 0;  // [57]
    virtual void _vf58() = 0;  // [58]
    virtual void _vf59() = 0;  // [59]
    virtual void _vf60() = 0;  // [60]
    virtual void RemoveAllActionFilters() = 0;  // [61] 0x18173CEC4  clears @+0x60 [KCD1 54]
    virtual void RemoveAllInputBlockData() = 0;  // [62] 0x18173D028  clears @+0x70 [KCD1 55, conf MED]
};

}  // namespace Offsets
