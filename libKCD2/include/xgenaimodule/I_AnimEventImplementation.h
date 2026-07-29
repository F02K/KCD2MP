#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::xgenaimodule::I_AnimEventImplementation -- shared root of the 14 anim-event
// implementation strategy objects (KCD2 WHGame.dll 1.5.6, kd7u).  ABSTRACT
// (vf1/vf2/vf3 pure).  sizeof 0x08 (vptr only; all leaves are 8B except
// PickUp/Place at 16B).
// -----------------------------------------------
// RTTI TD rva 0x4FF37B0; COL rva 0x45C3A08; vtable 0x1840133D8, 13 slots.  Never
// instantiated; the vtable is referenced only by the two shared deleting-dtor
// bodies (family-A sub_1820C2210 / family-B sub_18342FB88, which reset the vptr
// here then tracked-free via sub_181AB5160).  Out-param struct types and exact
// prototypes are role-inferred [U signatures]; slot [11] = sub_1805F5DA0
// (return this) is shared by ALL 14 classes and never overridden.

namespace wh::xgenaimodule {

class I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_AnimEventImplementation;
    virtual ~I_AnimEventImplementation();                    // [0]  shared deleting dtor sub_1820C2210
    virtual void* GetName() = 0;                             // [1]  PURE -- per-class static string descriptor [U ret type]
    virtual bool  Apply(int param, void* ctx) = 0;           // [2]  PURE -- primary event action [U ret/params]
    virtual void  ApplySecondary(int param, void* ctx) = 0;  // [3]  PURE -- optional 2nd pass (leaves usually nullsub)
    virtual void* BuildStringA(void* out);                   // [4]  sub_1819D4C6C (default; via sub_181733254) [U]
    virtual int*  GetCategory(int* out);                     // [5]  sub_181A9BED0 -- *out = 0 default [U name]
    virtual void  SerializeHook6();                          // [6]  nullsub_1 default [U signature]
    virtual void  SerializeHook7();                          // [7]  nullsub_1 default [U signature]
    virtual void  SerializeHook8();                          // [8]  nullsub_1 default [U signature]
    virtual void* GetDefaultWuid(void* out);                 // [9]  sub_181AAB750 -- *out = qword_18533AD68 (WUID, 8B) [U]
    RTTR_ENABLE()  // [10..12]: get_type 0x183434AC4 (ex-"BuildStringB"), get_derived 0x1834347AC (ex-"GetIdLabel"); leaves override [10]/[12]
};
static_assert(sizeof(I_AnimEventImplementation) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
