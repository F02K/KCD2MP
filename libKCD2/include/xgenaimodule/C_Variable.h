#pragma once
#include <cstdint>
#include "Callbacks/C_NoDataExtensionBase.h"
#include "BehaviorTree/C_ExpressionRef.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_Variable -- base observable AI-variable value node: writes
// fire the Callbacks mix-in, children keyed by int-index or WUID (KCD2 WHGame.dll
// 1.5.6, kd7u).  ABSTRACT (18 _purecall slots).  sizeof 0x40 (proven: direct
// subclass C_SingleVariable places its first member at +0x40; factory allocates
// the subclasses, never C_Variable itself).
// -----------------------------------------------
// RTTI TD rva 0x4FFBF68; COL rva 0x40CCEF0; vtable 0x183A3FFF0, 34 slots.  Real
// base chain (RTTI, all mdisp 0): Callbacks::C_PlainCallbackExtension ->
// C_NoDataExtension<Functor0,1> -> C_Extension<X,Functor0,1,1> ->
// C_NoDataExtensionBase<Functor0,1>; the intermediates add no data and are folded
// into the modeled C_NoDataExtensionBase base here [U slot split].
// ctor sub_1805B810C(this, owner, name); copy-ctor sub_1805B8CC4; dtor
// sub_1805B835C.  Key type-tag globals: qword_185496300 = int-index,
// qword_185496320 = WUID (dispatchers [6][9][12][16][17][20] compare these and
// forward to the typed slot pairs).  Variable factory sub_1805B34A8 (case 0..3 ->
// Single/IndexedArray/AssociativeArray/CustomAssociative).
// Pure slots: 1,7,8,10,11,13,14,18,19,22,24,25,26,27,28,29,31,32.

namespace wh::xgenaimodule {

class C_Variable : public Callbacks::C_NoDataExtensionBase<Functor0, 1> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Variable;
    // [0] = ~C_NoDataExtensionBase override: scalar deleting dtor sub_18345F10C (body sub_1805B835C)
    virtual int          GetId() = 0;                       // [1]  PURE; used by [33] [U name]
    virtual const char*  GetName();                         // [2]  sub_18345FFB8 -- base returns "Implement this"
    virtual void         _vf3();                            // [3]  [U role]
    virtual void         _vf4();                            // [4]  [U role]
    virtual void         _vf5();                            // [5]  [U role]
    virtual void*        GetChildDispatch(void* ref);       // [6]  sub_1834602D0 -- key-tag dispatch -> [7]/[8]
    virtual void*        GetChildByWUID(const void* wuid) = 0;      // [7]  PURE in base (real in containers)
    virtual void*        GetChildByIndex(int idx) = 0;              // [8]  PURE in base
    virtual void*        GetOrCreateChildDispatch(void* ref);       // [9]  sub_18345F564 -> [10]/[11]
    virtual void*        GetOrCreateChildByWUID(const void* wuid) = 0;  // [10] PURE in base
    virtual void*        GetOrCreateChildByIndex(int idx) = 0;          // [11] PURE in base (auto-grows)
    virtual void         SetRemoveDispatch(void* ref);      // [12] sub_1834600D4 -> [13]/[14]
    virtual void         SetChildByKey(void* key) = 0;      // [13] PURE in base [U exact role of 13 vs 14]
    virtual void         RemoveChildByKey(void* key) = 0;   // [14] PURE in base
    virtual void         _vf15();                           // [15] [U role]
    virtual void*        _dispatch16(void* ref);            // [16] sub_1834629D0 [U paired role]
    virtual void*        _dispatch17(void* ref);            // [17] sub_183462934 [U paired role]
    virtual bool         HasChildByWUID(const void* wuid) = 0;  // [18] PURE in base
    virtual bool         HasChildByIndex(int idx) = 0;          // [19] PURE in base
    virtual void*        _dispatch20(void* ref);            // [20] sub_180A277FC [U paired role]
    virtual bool         IsKeyTypeSupported(void* typeTag); // [21] sub_18345F448: tag==WUID || tag==int
    virtual void         _vf22() = 0;                       // [22] PURE -- typed value get/set [U which]
    virtual void         _vf23();                           // [23] [U role]
    virtual void         _vf24() = 0;                       // [24] PURE [U role]
    virtual void         _vf25() = 0;                       // [25] PURE [U role]
    virtual void         _vf26() = 0;                       // [26] PURE [U role]
    virtual void         _vf27() = 0;                       // [27] PURE [U role]
    virtual void         _vf28() = 0;                       // [28] PURE [U role]
    virtual void         _vf29() = 0;                       // [29] PURE [U role]
    virtual void*        GetTypeName();                     // [30] sub_1824877BC -- thread-safe static string
    virtual void         _vf31() = 0;                       // [31] PURE -- serialize group [U which]
    virtual void         _vf32() = 0;                       // [32] PURE -- serialize group [U which]
    virtual void*        GetInfo(void* out);                // [33] sub_183460A98 -- fills (ownerName, [1](), flag bit1)

    uint8_t           m_flags;   // +0x18  bit0 set by ctor; bits1/2 copied by copy-ctor [U bit meanings]
    void*             m_owner;   // +0x20  owning named registry obj (CryStringT name @+0x08); factory interns via sub_1823CB42C(qword_1854965D8,name); read only for its name by GetInfo sub_183460A98 [U pointee class]
    CryStringT<char>  m_name;    // +0x28  variable name (sub_1804FD208)
    BehaviorTree::Expressions::C_ExpressionRef m_expr;   // +0x30  embedded expression ref
};
static_assert(sizeof(C_Variable) == 0x40, "C_Variable must be 0x40 (subclass first member at +0x40)");

}  // namespace wh::xgenaimodule
