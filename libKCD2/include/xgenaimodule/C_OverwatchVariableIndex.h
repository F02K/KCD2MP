#pragma once
#include <cstdint>
#include <vector>
#include "I_VariableIndex.h"
#include "C_VariableIndex.h"
#include "Callbacks/C_NoDataExtensionBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_OverwatchVariableIndex : I_VariableIndex +
// Callbacks::C_NoDataExtension<Functor0,1> (@+0x10) -- MI overwatch variable
// index; does NOT derive from C_VariableIndex (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof UNPROVEN -- pool-allocated inside a larger agent object
// (sub_18206BEA0 pool / driver sub_18070D260); the confident standalone prefix is
// 0x40 (from dtor sub_180ED67AC).  NO static_assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FFD408.  Primary vtable (I_VariableIndex, 20 slots) rva
// 0x3AC7E88, COL 0x40CCE50; secondary (@+0x10, 1 slot) rva 0x3AC7E78, COL
// 0x40CCE78.  Overrides most of I_VariableIndex [1..16] with own bodies
// (0x181097../0x18346..); keeps concrete [6][7][17].  ctor sub_180ED6824
// (in-place); fields >= +0x40 written there may belong to the ENCLOSING agent
// object [U].  Real 2nd base is C_NoDataExtension<Functor0,1>; modeled via the
// data-identical folded root C_NoDataExtensionBase [U].

namespace wh::xgenaimodule {

class C_OverwatchVariableIndex
    : public I_VariableIndex,
      public Callbacks::C_NoDataExtensionBase<Functor0, 1> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_OverwatchVariableIndex;
    // Overrides I_VariableIndex [1..16] with own bodies (0x181097../0x18346.. region);
    // keeps the base-concrete [6][7][17].  Per-slot bodies not itemized [U addresses].
    void* GetVarByKeyHash(uint16_t hash, bool recurseToParent) override;  // [1]
    void* GetVarByName(const char* name, bool recurse) override;          // [2]
    void  Insert(void* entry) override;                                   // [3]
    void* CreateVariable(void* desc) override;                            // [4]
    void  RemoveVariable(const char* name) override;                      // [5]
    void  CollectAll(void* outVec, bool recurse) override;                // [8]
    void  ForEachChild() override;                                        // [10]
    void  _vf11() override;                                               // [11]
    void  Serialize(void* stream) override;                               // [12]
    void  Deserialize(void* stream) override;                             // [13]
    void  NetSerialize(void* stream) override;                            // [14]
    void  ParseFromString(const char* str) override;                      // [15]
    void  GetIdentity(void* outIdentity) override;                        // [16]

    std::vector<C_VariableIndex::S_Entry> m_map;   // +0x28  same Entry16 map shape as C_VariableIndex
    // +0x40.. overwatch state (int -1 @+0x40, word 1 @+0xA0, int id -1 @+0xA4, 64B-elem
    // vector @+0x48, vptr pair @+0x60/+0x70, ptr @+0xE0) -- possibly owned by the
    // ENCLOSING pooled agent, NOT modeled here [U].
};

}  // namespace wh::xgenaimodule
