#pragma once
#include <cstdint>

// -----------------------------------------------
// IInput -- CryEngine input system, KCD2 binary vtable order (impl CDXInput).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  No CBaseInput/CInput RTTI exists in KCD2 -- the concrete
// input impl is CDXInput (DirectInput/GDK), primary vtable 0x184078F18, 72 slots (KCD1: 53).
// Full evidence table: KCD2/analysis/vtables_reslot/IInput_IActionMap_IGFListener.md.
// Unnamed _vfN slots are real functions whose role is UNVERIFIED -- do not call them.
//
// [FUNDAMENTAL vs KCD1] KCD1 order survives but slides down around inserted slots:
// RemoveInputDevice @[10], a getter @[27], ClearSpecificAnalogKeyState @[39], plus a new
// device-enumeration/cursor tail [47..55]/[62]. EnableEventPosting 10->11, PostInputEvent
// 12->13, Init 18->24, Update 20->26, ClearKeyState 30->37, GetModifiers 35->43,
// SetBlockingInput 41->56, FlushBufferedInputs 52->68.

namespace Offsets {

struct IInput {
    virtual void Dtor(char flags) = 0;  // [0] 0x183830478  scalar deleting dtor
    virtual bool AddConsoleEventListener(void* l) = 0;  // [1] 0x180BC5158  dedup-append @+0x58 [KCD1 1]
    virtual bool RemoveConsoleEventListener(void* l) = 0;  // [2] 0x180605C08  remove @+0x58 [KCD1 2]
    virtual bool AddEventListener(void* l) = 0;  // [3] 0x180BC514C  dedup-append @+0x80 [KCD1 3]
    virtual bool RemoveEventListener(void* l) = 0;  // [4] 0x183838754  remove @+0x80 [KCD1 4]
    virtual bool AddTouchEventListener(void* l, const char* name) = 0;  // [5] 0x183837EE0  touch vector @+0xB0 [KCD1 5]
    virtual bool RemoveTouchEventListener(void* l) = 0;  // [6] 0x182AAE214  deferred-removal guarded [KCD1 6]
    virtual void SetExclusiveListener(void* l) = 0;  // [7] 0x181A9D550  +0xA8 = arg (ICF-folded) [KCD1 7]
    virtual void* GetExclusiveListener() = 0;  // [8] 0x181A74C20  returns +0xA8 (ICF-folded) [KCD1 8]
    virtual bool AddInputDevice(void* device) = 0;  // [9] 0x182106150  "Input device added:" [KCD1 9]
    virtual bool RemoveInputDevice(void* device) = 0;  // [10] 0x18383888C  device map removal -- KCD2-INSERTED
    virtual void EnableEventPosting(bool b) = 0;  // [11] 0x18383801C  "InputDebug: EnableEventPosting(%s)" [KCD1 10]
    virtual bool IsEventPostingEnabled() = 0;  // [12] 0x181A7E040  byte +0xD8 [KCD1 11]
    virtual void PostInputEvent(const void* event, bool bForce) = 0;  // [13] 0x18085BFE8  "InputDebug: '%s' - %s" broadcast [KCD1 12]
    virtual void PostTouchEvent(const void* event, bool bForce) = 0;  // [14] 0x183838680  "InputDebug [Touch]" [KCD1 13]
    virtual void PostUnicodeEvent(const void* event, bool bForce) = 0;  // [15] 0x180A58AB8  "Unicode input codepoint (%u)" [KCD1 14]
    virtual void ForceFeedbackEvent(const void* event) = 0;  // [16] 0x180A1CF20  map @+0x18 insert [KCD1 15, conf LOW]
    virtual void _vf17() = 0;  // [17]
    virtual void _vf18() = 0;  // [18]
    virtual void _vf19() = 0;  // [19]
    virtual void _vf20() = 0;  // [20]
    virtual void _vf21() = 0;  // [21]
    virtual void _vf22() = 0;  // [22]
    virtual void _vf23() = 0;  // [23]
    virtual bool Init() = 0;  // [24] 0x180DA1FCC  "Initializing DirectInput"; adds kb/mouse/XInput/PS/GDK [KCD1 18]
    virtual void PostInit() = 0;  // [25] 0x1819A202C  registers cvar wh_i_deadZone [KCD1 19]
    virtual void Update(bool bFocus) = 0;  // [26] 0x18085BC4C  frame update over devices [KCD1 20]
    virtual void _vf27() = 0;  // [27]
    virtual void ShutDown() = 0;  // [28] 0x183833E00  "DXInput Shutdown" [KCD1 21]
    virtual void SetExclusiveMode(int deviceType, bool exclusive, void* hwnd) = 0;  // [29] 0x1808EB3D8  "SetExclusiveMode(%d, %s)" [KCD1 22]
    virtual bool InputState(const void* keyNameId, int state) = 0;  // [30] 0x183838420  device+0x78 sweep [KCD1 23]
    virtual const char* GetKeyName(int keyId) = 0;  // [31] 0x183838288  device+0xA8 [KCD1 24]
    virtual const char* GetKeyNameByEvent(const void* event) = 0;  // [32] 0x1838381BC  device+0xB0 [KCD1 25]
    virtual char GetInputCharAscii(const void* event) = 0;  // [33] 0x1838380F8  device+0xB8 [KCD1 26]
    virtual const void* LookupSymbol(int deviceType, int deviceIndex, int keyId) = 0;  // [34] 0x1838384E0  device+0xC8 [KCD1 27]
    virtual const void* GetSymbolByName(const char* name) = 0;  // [35] 0x180A107F8  device+0xD0 [KCD1 28]
    virtual const char* GetOSKeyName(const void* event) = 0;  // [36] 0x183838340  device+0xC0 [KCD1 29]
    virtual void ClearKeyState() = 0;  // [37] 0x180859F98  "InputDebug: ClearKeyState" + ToUnicode flush [KCD1 30]
    virtual void ClearAnalogKeyState() = 0;  // [38] 0x18085A250  "InputDebug: ClearAnalogKeyState" [KCD1 31]
    virtual void ClearSpecificAnalogKeyState(const void* keys) = 0;  // [39] 0x180617924  KCD2-INSERTED
    virtual void RetriggerKeyState() = 0;  // [40] 0x18085B594  "InputDebug: RetriggerKeyState" [KCD1 32]
    virtual bool Retriggering() = 0;  // [41] 0x1805094F0  byte +0xD9 [KCD1 33]
    virtual bool HasInputDeviceOfType(int type) = 0;  // [42] 0x1817C4598  device+0xD8 sweep [KCD1 34]
    virtual uint32_t GetModifiers() = 0;  // [43] 0x18085CFA0  dword +0x148 (zeroed by ClearKeyState) [KCD1 35]
    virtual void EnableDevice(int deviceIndex, bool enable) = 0;  // [44] 0x183837F78  device+0x58/+0xE0 [KCD1 36]
    virtual void SetDeadZone(float dz) = 0;  // [45] 0x183838C9C  "InputDebug: SetDeadZone [%0.3f]" [KCD1 37]
    virtual void RestoreDefaultDeadZone() = 0;  // [46] 0x183838A6C  cvar wh_i_defaultDeadZone [KCD1 38]
    virtual void* GetDevice(void* predicate) = 0;  // [47] 0x182106300  device vector @+0x110 scan [KCD1 39, conf MED]
    virtual void _vf48() = 0;  // [48]
    virtual void _vf49() = 0;  // [49]
    virtual void _vf50() = 0;  // [50]
    virtual void _vf51() = 0;  // [51]
    virtual void _vf52() = 0;  // [52]
    virtual void _vf53() = 0;  // [53]
    virtual void _vf54() = 0;  // [54]
    virtual void _vf55() = 0;  // [55]
    virtual void SetBlockingInput(const void* blockData) = 0;  // [56] 0x183838B9C  blocking list @+0x138 [KCD1 41]
    virtual void RemoveBlockingInput(const void* blockData) = 0;  // [57] 0x1838386FC  [KCD1 42]
    virtual bool HasBlockingInput(const void* blockData) = 0;  // [58] 0x1838383E4  [KCD1 43]
    virtual uint32_t GetNumBlockingInputs() = 0;  // [59] 0x181A74D40  dword +0x140 [KCD1 44]
    virtual void ClearBlockingInputs() = 0;  // [60] 0x183837F3C  [KCD1 45]
    virtual bool ShouldBlockInputEventPosting(int keyId, int deviceType, int deviceIndex) = 0;  // [61] 0x18085C248  [KCD1 46]
    virtual int ShowCursorCount(bool bShow) = 0;  // [62] 0x1808EB3CC  Win32 ShowCursor(a2) -- KCD2-NEW
    virtual void* GetKinectInput() = 0;  // [63] 0x181A77FE0  +0x158 getter [KCD1 47, conf LOW]
    virtual void* GetNaturalPointInput() = 0;  // [64] 0x181A75270  +0x170 getter [KCD1 48, conf LOW]
    virtual void* GetEyeTrackerInput() = 0;  // [65] 0x181A74210  +0x168 getter (WH Tobii) [KCD1 49, conf LOW]
    virtual void _vf66() = 0;  // [66]
    virtual const char* GetKeyNameLocalized(const void* event) = 0;  // [67] 0x1812D044C  device+0x130 (WH) [KCD1 51, conf MED]
    virtual void FlushBufferedInputs() = 0;  // [68] 0x183838060  "InputDebug: FlushBufferedInputs" [KCD1 52]
    virtual void _vf69() = 0;  // [69]
    virtual void _vf70() = 0;  // [70]
    virtual void _vf71() = 0;  // [71]
};

}  // namespace Offsets
