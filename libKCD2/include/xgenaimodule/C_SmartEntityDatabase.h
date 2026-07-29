#pragma once
#include <cstdint>
#include <unordered_map>
#include "I_SmartEntityDatabase.h"
#include "../databasemodule/C_ObjectTreeDatabaseList.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEntityDatabase : I_SmartEntityDatabase (@+0x0) +
// databasemodule::C_ObjectTreeDatabase<S_SmartEntityTemplate, std::list>
// (@+0x08, list variant 0x70 -- mirrored via C_ObjectTreeDatabaseList) --
// smart-entity template DB, HEAP singleton (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xC0 (create-site proven: sub_18109764C: operator new(192) -> ctor
// sub_181541050).  tablepath "ai/smartEntity", typename "SmartEntity", ns
// "wh::xgenaimodule::".
// -----------------------------------------------
// RTTI TD rva 0x4F51C08; primary COL 0x40DF370, vtable rva 0x3BB0710 (6
// slots); base COL 0x40DF398, vtable rva 0x3BB0A60.  Row = S_SmartEntityTemplate
// sizeof 200 (std::list node 216 = 0x10 + 200); row fields [U].  +0x78 column
// handle "brain" (registered size 24); +0x80 index map key/value types [U].

namespace wh::xgenaimodule {

struct S_SmartEntityTemplate {
    inline static constexpr auto RTTI = Offsets::RTTI_S_SmartEntityTemplate;
    uint8_t _raw[200];   // sizeof proven (list node 216); fields [U]
};
static_assert(sizeof(S_SmartEntityTemplate) == 200, "S_SmartEntityTemplate must be 200");

class C_SmartEntityDatabase
    : public I_SmartEntityDatabase,
      public databasemodule::C_ObjectTreeDatabaseList<S_SmartEntityTemplate> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEntityDatabase;
    // I_SmartEntityDatabase impls [U roles]
    void SedVf0() override; void SedVf1() override; void SedVf2() override;
    void SedVf3() override; void SedVf4() override; void SedVf5() override;

    void* m_colBrain;   // +0x78  opaque column handle: return of (reg->vf[96])("brain",24) where reg=*(sub_1809155C8()+0x148) reflection column registry; prior vf[72]("brain",0) declares it [U handle==ptr|index]
    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x80  (0x40) key/value types [U]
};
static_assert(sizeof(C_SmartEntityDatabase) == 0xC0, "C_SmartEntityDatabase must be 0xC0 (new 192 at sub_18109764C)");

}  // namespace wh::xgenaimodule
