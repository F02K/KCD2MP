#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseIdIndexed<TBase, ...> -- id-index decorator
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template (C_SoulArchetypeDatabase / C_SocialClassDatabase stacks).
// LAYOUT RESOLVED (concrete ctors sub_1815A1290 / sub_18192F920): the template adds the index
// vector @+0x98 plus an UNINITIALIZED qword @+0xB0, ending at +0xB8. The I_DynamicEnum base seen
// at +0xB8 in the databases is declared by each CONCRETE class (its vfptr is written by the
// concrete ctor with the concrete vftable), NOT by this template -- the earlier "impossible
// layout" reading came from mis-attributing that base here.

namespace wh::databasemodule {

template <typename TBase>
class C_ObjectDatabaseIdIndexed : public TBase {
public:
    std::vector<void*> m_idIndex;   // +0x98  ctor zeroes; element type UNRESOLVED
    uint64_t m_unkB0;               // +0xB0  VERIFIED uninit: concrete ctor sub_1815A1290 zeroes m_idIndex vec +0x98/+0xA0/+0xA8 then writes DynamicEnum vfptr +0xB8, skipping +0xB0; type/role unresolved
};

}  // namespace wh::databasemodule
