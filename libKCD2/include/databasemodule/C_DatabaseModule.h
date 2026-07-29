#pragma once
#include <cstdint>
#include <vector>
#include "I_DatabaseModule.h"
#include "C_DynamicEnumManager.h"
#include "../framework/C_BaseModule.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::databasemodule::C_DatabaseModule -- the database module (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184AEF568.  Vtables: primary 0x183C38C80 (C_BaseModule/I_ModuleMessageListener),
// +0x10 0x183C38CC0 (the 31-slot I_DatabaseModule impl).  sizeof 0xF8 (PROVEN: module
// bootstrap sub_180C9DA44 raw-allocs 248 then ctor sub_18192BBB4).  The bootstrap stores
// this+0x10 (the I_DatabaseModule subobject) into module-registry+0x148 (slot [41]); the ctor
// also publishes the undisplaced instance to global qword_185168B48.  Dtor sub_182740A88
// confirms the container roles annotated below.

namespace wh::databasemodule {

struct S_DatabaseTableRecord;    // 88B table registration record (name SYNTHETIC; layout in comment below)
struct S_TableLayoutCacheEntry;  // 24B {CryStringT name, void* type, void* layout} memo entry (name SYNTHETIC)

class C_DatabaseModule
    : public wh::framework::C_BaseModule   // +0x00
    , public I_DatabaseModule              // +0x10
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DatabaseModule;

    // +0x18  std::vector<S_DatabaseTableRecord*>; RegisterTable (slot7 0x180D17E54) news an 88B
    // record (ctor 0x180D17F20: name/path CryStrings @+0x28/+0x30, type @+0x38, schema @+0x40,
    // flags @+0x50) and push_backs its ptr; dtor 0x182740AF1 destroys each via sub_180D182CC.
    std::vector<S_DatabaseTableRecord*> m_tableRecords;   // +0x18
    // +0x30  std::vector of OWNED polymorphic objects (dtor 0x182740B23 deleting-dtors each:
    // (**vtbl)(elem,1)); LoadTable 0x180D18444 returns false when empty. Element class unresolved.
    std::vector<void*> m_vec30;   // +0x30
    CryStringT<char> m_str48;   // +0x48  [role UNVERIFIED]
    uint64_t m_unk50;           // +0x50  ctor 0; NOT an owning std::vector (dtor never frees 0x50/0x58/0x60)
    uint64_t m_unk58;           // +0x58  ctor 0; role unresolved (no writer found in ctor/dtor/impl cluster)
    uint64_t m_unk60;           // +0x60  ctor 0
    // +0x68  std::vector<S_TableLayoutCacheEntry*> memo cache keyed by (reflected table type, name)
    // -> built layout; builder 0x180D17458 allocs 24B {CryStringT<char> name@0, void* type@8,
    // void* layout@16}; dtor 0x182740B44 decrefs name (sub_1804FD898) then frees the block.
    std::vector<S_TableLayoutCacheEntry*> m_layoutCache;   // +0x68
    // +0x80  std::vector<char*> of formatted path buffers; builder 0x180D17458 allocs a char buf
    // and sprintf's a "...%s..." template (sub_180466874) then pushes; dtor 0x182740B77 frees each raw.
    std::vector<char*> m_pathBuffers;   // +0x80
    C_DynamicEnumManager m_dynamicEnums;   // +0x98..0xDF
    void*    m_pSelfHandleE0;   // +0xE0  -> ctor-alloc'd 8B heap block { C_DatabaseModule* self }; freed raw in dtor (self back-ref handle)
    uint64_t m_unkE8;           // +0xE8  never written by ctor or dtor; role unresolved
    void*    m_pSelfHandleF0;   // +0xF0  -> ctor-alloc'd 16B load-context { C_DatabaseModule* self@0, int32 loadErrorCount@8 }; sub_180D18490 does ++[+8] on load error

    // Reads the undisplaced instance global qword_185168B48 (RVA 0x5168B48).
    // Impl in src/databasemodule/databasemodule.cpp.
    static C_DatabaseModule* GetInstance();
};
static_assert(sizeof(C_DatabaseModule) == 0xF8, "C_DatabaseModule must be 0xF8 (bootstrap alloc(248))");

}  // namespace wh::databasemodule
