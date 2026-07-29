#pragma once
#include <cstdint>

// -----------------------------------------------
// IFlashVariableObject — KCD2 binary vtable order (INTERFUSCATED)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashPlayer.h
// (<interfuscator:shuffle> block => binary slot order != SDK declaration order;
// this replica is BINARY order, re-verified per slot against the KCD1 named map).
// RTTI: .?AUIFlashVariableObject@@ (global namespace, struct)
// Impl: CFlashVariableObject (.?AVCFlashVariableObject@@), single base at +0x00,
//       vtable 0x183A55BA0 (47 slots; KCD1 had 46 — one NEW Invoke overload),
//       sizeof == 0x70 (operator new(0x70) at every creation site:
//       CFlashPlayer::GetVariable/Create*/..., Clone, GetMember, GetElement, ...).
//   Ctor: CFlashVariableObject::Ctor_1807F5E50(this, GFxValue* src,
//         sharedFilePath* (player+0xE8), sharedLock* (player+0xF8 SRWLOCK))
//
// Layout (from Clone 0x18244A754 + method bodies):
//   +0x00 vfptr                        +0x08 GFxValue m_value { ObjectInterface*@+0x08,
//   type@+0x10, value@+0x18 }          +0x20/+0x28 shared filepath ptr + refholder
//   +0x30 returned-string cache        +0x48/+0x50 SRWLOCK* + refholder
//   +0x58 intrusive list node          +0x68 owner backptr (Clone sets = self)
//
// GFxValue runtime types observed: 4=string 5=wstring 6=object 7=array 8=displayobject;
// type guards: IsObject sub_1807F5F58, IsArray (t&0x8F)==7, IsDisplayObject ==8.
// Object ops forward to GFx ObjectInterface helpers passing the raw AS object
// (this+0x18) and an isDisplayObject flag — same shape as KCD1, new helper addrs.
//
// KCD1 -> KCD2 deltas:
//  * NEW adjacent overload pair [13]/[14]: Invoke now also accepts an
//    IFlashVariableObject* argument array (each arg's GFxValue read from obj+8).
//  * dtor moved to the last slot [46] (KCD1 [45]); Release vcalls self +0x170.
//  * SFlashCxform layout CHANGED in the binary: mul = 4 floats @ +0x00, but
//    add = 4 BYTES @ +0x10 (KCD1/stock SDK: 8 floats). See SetColorTransform [31].
//  * SFlashDisplayInfo layout CHANGED: m_varsSet WORD moved to +0x34 (KCD1 +0x2A);
//    float fields +0x00..+0x24 and visible byte +0x28 unchanged; +0x2C..+0x33
//    unknown new fields. Do NOT pass the stock-SDK struct to [25]/[26].

struct SFlashVarValue;
struct SFlashCxform;       // KCD2 binary layout: { float mul[4]; uint8 add[4]; } — see banner
struct SFlashDisplayInfo;  // KCD2 binary layout: varsSet WORD @ +0x34 — see banner
template<typename F> struct Matrix33_tpl;
template<typename F> struct Matrix44_tpl;
typedef Matrix33_tpl<float> Matrix33;   // matches CryCommon Cry_Math typedef (f32 == float)
typedef Matrix44_tpl<float> Matrix44;

namespace Offsets {

struct IFlashVariableObject {
    struct ObjectVisitor {            // declared OUTSIDE the SDK shuffle block — order trusted
        virtual void Visit(const char* pName) = 0;       // [0]
    protected:
        virtual ~ObjectVisitor() {}                      // [1]
    };

    // Binary vtable 0x183A55BA0 (47 slots).
    // NOTE: each same-name overload PAIR is declared higher-slot-FIRST, because
    // MSVC emits adjacent virtual overloads reversed (2nd-declared -> lower slot).
    virtual void Release() = 0;                                             // [0]  0x000 0x1807F60A0  vcall self vf+0x170 == slot [46] scalar dtor(1)
    virtual IFlashVariableObject* Clone() const = 0;                        // [1]  0x008 0x18244A754  operator new(0x70) + CFlashVariableObject::`vftable' + member copy
    virtual bool IsObject() const = 0;                                      // [2]  0x010 0x18244E110  sub_1807F5F58(GFxValue) type guard (object/array/displayobject)
    virtual bool IsArray() const = 0;                                       // [3]  0x018 0x1807F6F2C  (type&0x8F) == 7
    virtual bool IsDisplayObject() const = 0;                               // [4]  0x020 0x1807F4DB8  (type&0x8F) == 8
    virtual SFlashVarValue ToVarValue() const = 0;                          // [5]  0x028 0x182451F24  GFxValue(this+8) -> SFlashVarValue via sub_1807F659C
    virtual bool HasMember(const char* pMemberName) const = 0;              // [6]  0x030 0x18244D990  ObjectInterface::HasMember sub_181C2E7C0
    virtual bool SetMember(const char* pMemberName,
                           const SFlashVarValue& value) = 0;                // [8]  0x040 0x1824512A4  SFlashVarValue -> GFxValue (sub_1807F6490); sub_181C30F50
    virtual bool SetMember(const char* pMemberName,
                           const IFlashVariableObject* pVarObj) = 0;        // [7]  0x038 0x18245134C  GFxValue copy from pVarObj+8 (sub_1807F5ED0), nullptr -> undefined; sub_181C30F50
    virtual bool GetMember(const char* pMemberName,
                           SFlashVarValue& value) const = 0;                // [10] 0x050 0x18244CADC  sub_181C2E390 + sub_1807F659C; (w)string results cached at this+0x30
    virtual bool GetMember(const char* pMemberName,
                           IFlashVariableObject*& pVarObj) const = 0;       // [9]  0x048 0x1807F5D44  sub_181C2E390 + new(0x70) Ctor_1807F5E50
    virtual void VisitMembers(ObjectVisitor* pVisitor) const = 0;           // [11] 0x058 0x182452F9C  wraps pVisitor in `CFlashVariableObject::VisitMembers'::VisitorAdaptor::`vftable'; sub_181C314A0
    virtual bool DeleteMember(const char* pMemberName) = 0;                 // [12] 0x060 0x18244BBCC  ObjectInterface::DeleteMember sub_181C2D9E0
    // Invoke overload pair — KCD2-NEW: the IFlashVariableObject*-args overload did
    // not exist in KCD1. SFlashVarValue* overload declared FIRST (higher slot [14]).
    virtual bool Invoke(const char* pMethodName, const SFlashVarValue* pArgs,
                        unsigned int numArgs, SFlashVarValue* pResult = 0) = 0; // [14] 0x070 0x1807F60F0  alloca GFxValue[numArgs] from 0x10-stride array (sub_1807F6490); ObjectInterface::Invoke sub_181C2EA20
    virtual bool Invoke(const char* pMethodName, const IFlashVariableObject* const* pArgObjs,
                        unsigned int numArgs, SFlashVarValue* pResult = 0) = 0; // [13] 0x068 0x18244DDD0  KCD2-NEW: GFxValue copied from each pArgObjs[i]+8 (sub_1807F5ED0); sub_181C2EA20
    virtual unsigned int GetArraySize() const = 0;                          // [15] 0x078 0x18244C564  type==7; ObjectInterface::GetArraySize sub_181C2DED0
    virtual bool SetArraySize(unsigned int size) = 0;                       // [16] 0x080 0x1807F6F3C  sub_181C2FD10
    virtual bool SetElement(unsigned int idx, const SFlashVarValue& value) = 0; // [18] 0x090 0x1807F63FC  sub_1807F6490; ObjectInterface::SetElement sub_181C30B30
    virtual bool SetElement(unsigned int idx,
                            const IFlashVariableObject* pVarObj) = 0;       // [17] 0x088 0x1824511B4  sub_1807F5ED0 (null -> undefined); sub_181C30B30
    virtual bool GetElement(unsigned int idx, SFlashVarValue& value) const = 0; // [20] 0x0A0 0x18244C908  sub_181C2E240 + sub_1807F659C (+string cache +0x30)
    virtual bool GetElement(unsigned int idx,
                            IFlashVariableObject*& pVarObj) const = 0;      // [19] 0x098 0x18244C810  ObjectInterface::GetElement sub_181C2E240 + new(0x70)
    virtual bool PushBack(const SFlashVarValue& value) = 0;                 // [22] 0x0B0 0x18244F51C  sub_1807F6490; ObjectInterface::PushBack sub_181C2F420
    virtual bool PushBack(const IFlashVariableObject* pVarObj) = 0;         // [21] 0x0A8 0x18244F5A4  sub_1807F5ED0 (null -> undefined); sub_181C2F420
    virtual bool PopBack() = 0;                                             // [23] 0x0B8 0x18244EF48  ObjectInterface::PopBack sub_181C2F2A0(..., 0)
    virtual bool RemoveElements(unsigned int idx, int count = -1) = 0;      // [24] 0x0C0 0x18244FC8C  ObjectInterface::RemoveElements sub_181C2F9A0(idx, count)
    virtual bool SetDisplayInfo(const SFlashDisplayInfo& info) = 0;         // [25] 0x0C8 0x1807F4520  reads KCD2 SFlashDisplayInfo (x@0..alpha@0x24, visible@0x28, m_varsSet@0x34!), remaps FDIF_* bits, sub_181C2FD90
    virtual bool GetDisplayInfo(SFlashDisplayInfo& info) const = 0;         // [26] 0x0D0 0x1807F4784  sub_181C2DF30, inverse bit/field mapping (varsSet written to +0x34)
    virtual bool SetDisplayMatrix(const Matrix33& mat) = 0;                 // [27] 0x0D8 0x182451110  copies 6 floats (2x3 affine) -> GMatrix2D; sub_181C309B0
    virtual bool GetDisplayMatrix(Matrix33& mat) const = 0;                 // [28] 0x0E0 0x18244C760  sub_181C2E1D0; reconstitutes 3x3 (third row 0,0,1)
    virtual bool Set3DMatrix(const Matrix44& mat) = 0;                      // [29] 0x0E8 0x182450E30  16 floats transposed; sub_181C30D80
    virtual bool Get3DMatrix(Matrix44& mat) const = 0;                      // [30] 0x0F0 0x18244C42C  sub_181C2E300, transposed back
    virtual bool SetColorTransform(const SFlashCxform& cx) = 0;             // [31] 0x0F8 0x182451064  mul: 4 floats @cx+0; add: 4 BYTES @cx+0x10 (float-widened) -> GRenderer::Cxform; sub_181C2FD30. LAYOUT DELTA vs stock SDK (add was ColorF)
    virtual bool GetColorTransform(SFlashCxform& cx) const = 0;             // [32] 0x100 0x18244C65C  sub_181C2DEF0, inverse (add floats truncated to bytes)
    virtual bool SetVisible(bool visible) = 0;                              // [33] 0x108 0x1807F4B0C  builds GFx DisplayInfo with only Visible flag (0x40) set; sub_181C2FD90
    virtual bool SetText(const char* pText) = 0;                            // [35] 0x118 0x182451604  sub_181C31220(...,html=0): direct TextField set if AS type==4, else SetMember("text", GFxValue type 4 = STRING)
    virtual bool SetText(const wchar_t* pText) = 0;                         // [34] 0x110 0x18245166C  sub_181C312E0(...,html=0): fallback GFxValue type 5 = WSTRING
    virtual bool SetTextHTML(const char* pHtml) = 0;                        // [37] 0x128 0x1824516D4  sub_181C31220(...,html=1): member name "htmlText"
    virtual bool SetTextHTML(const wchar_t* pHtml) = 0;                     // [36] 0x120 0x18245173C  sub_181C312E0(...,html=1)
    virtual bool GetText(SFlashVarValue& text) const = 0;                   // [38] 0x130 0x18244CFE4  sub_181C2E530(...,html=0) + sub_1807F659C (+string cache)
    virtual bool GetTextHTML(SFlashVarValue& html) const = 0;               // [39] 0x138 0x18244D0DC  sub_181C2E530(...,html=1)
    virtual bool CreateEmptyMovieClip(IFlashVariableObject*& pVarObjMC, const char* pInstanceName,
                                      int depth = -1) = 0;                  // [40] 0x140 0x18244AE0C  ObjectInterface::CreateEmptyMovieClip sub_181C2CE30 + new(0x70)
    virtual bool AttachMovie(IFlashVariableObject*& pVarObjMC, const char* pSymbolName,
                             const char* pInstanceName, int depth = -1,
                             const IFlashVariableObject* pInitObj = 0) = 0; // [41] 0x148 0x1824496C8  ObjectInterface::AttachMovie sub_181C2C660; pInitObj's GFxValue at +8 (null-safe) + new(0x70)
    virtual bool GotoAndPlay(const char* pFrame) = 0;                       // [43] 0x158 0x180D83D8C  sub_181C2E720(label, stop=0)
    virtual bool GotoAndPlay(unsigned int frame) = 0;                       // [42] 0x150 0x18244D718  sub_181C2E6A0(frame, stop=0)
    virtual bool GotoAndStop(const char* pFrame) = 0;                       // [45] 0x168 0x18089B9D4  sub_181C2E720(label, stop=1)
    virtual bool GotoAndStop(unsigned int frame) = 0;                       // [44] 0x160 0x18089A86C  sub_181C2E6A0(frame, stop=1)
    virtual ~IFlashVariableObject() {}                                      // [46] 0x170 0x1807F60BC  scalar deleting dtor -> ~CFlashVariableObject sub_1807F5FA0 (+free sub_181AB5160 if flags&1); Release vcalls this slot
};

// RAII unique owner for engine-allocated IFlashVariableObjects. GetVariable /
// GetMember / GetElement / Create* / Clone each operator-new a fresh 0x70
// CFlashVariableObject; Release() (slot [0]) == `delete this` (scalar deleting
// dtor) — there is NO AddRef, so ownership is UNIQUE. Releases on scope exit;
// move-only. Use put() for the `IFlashVariableObject*&` out-params.
//   FlashVarPtr v;
//   fp->GetVariable("_root.Foo", v.put());
//   if (v) fp->SetVariable("_root.Bar", v.get());   // auto-Release at scope end
class FlashVarPtr {
    IFlashVariableObject* m_p = nullptr;
public:
    FlashVarPtr() = default;
    explicit FlashVarPtr(IFlashVariableObject* p) noexcept : m_p(p) {}
    ~FlashVarPtr() { if (m_p) m_p->Release(); }
    FlashVarPtr(FlashVarPtr&& o) noexcept : m_p(o.m_p) { o.m_p = nullptr; }
    FlashVarPtr& operator=(FlashVarPtr&& o) noexcept {
        if (this != &o) { if (m_p) m_p->Release(); m_p = o.m_p; o.m_p = nullptr; }
        return *this;
    }
    FlashVarPtr(const FlashVarPtr&) = delete;
    FlashVarPtr& operator=(const FlashVarPtr&) = delete;

    IFlashVariableObject*  get() const noexcept       { return m_p; }
    IFlashVariableObject*  operator->() const noexcept { return m_p; }
    explicit operator bool() const noexcept           { return m_p != nullptr; }
    // Releases any held value, then yields the slot for a Get*/Create* out-param.
    IFlashVariableObject*& put() noexcept { if (m_p) { m_p->Release(); m_p = nullptr; } return m_p; }
};

}  // namespace Offsets
