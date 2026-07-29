#pragma once
#include "C_SingleVariable.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SingleVariableLuaReadOnly : C_SingleVariable -- a
// C_SingleVariable exposed read-only to Lua (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x58 (create-site proven: ctor path sub_18041B94C operator new(88) --
// builds a C_SingleVariable, overwrites the vptr, registers a Lua accessor
// functor).  NO new data members.
// -----------------------------------------------
// RTTI TD rva 0x4FFD450; COL rva 0x40CCEC8; vtable 0x183A37DF0, 34 slots --
// shares almost all C_SingleVariable slots; differs at [5] (sub_18041A6A0) and
// [0] (deleting dtor sub_181238544, also decrements a Lua-registration counter).

namespace wh::xgenaimodule {

class C_SingleVariableLuaReadOnly : public C_SingleVariable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SingleVariableLuaReadOnly;
    ~C_SingleVariableLuaReadOnly() override;   // [0] sub_181238544 (Lua-unregister + free)
    void _vf5() override;                      // [5] sub_18041A6A0 [U role]
};
static_assert(sizeof(C_SingleVariableLuaReadOnly) == 0x58, "C_SingleVariableLuaReadOnly must be 0x58 (operator new(88))");

}  // namespace wh::xgenaimodule
