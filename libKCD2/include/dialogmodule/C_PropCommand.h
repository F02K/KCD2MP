#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_PropCommand -- prop timeline command base (KCD2 1.5.6, kd7u).
// sizeof 0x18.  Abstract.
// -----------------------------------------------
// RTTI TD 0x184B53338; vtable 0x183A936A8 (12 slots); field init sub_180CF6968;
// deleting-dtor [0] sub_180EB8034. Serialize [2] is _purecall at this layer. Appends two
// slots to the I_DialogCommand contract: [10] (role UNVERIFIED, pure) and
// [11] IsInstant() (C_InstantPropCommand returns 1, C_AnimationPropCommand returns 0).

namespace wh::dialogmodule::data {

class C_PropCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PropCommand;
    virtual void _vf10() = 0;       // [10] [role UNVERIFIED]
    virtual bool IsInstant() = 0;   // [11]

    CryStringT<char> m_propName;    // +0x10
};
static_assert(sizeof(C_PropCommand) == 0x18, "C_PropCommand must be 0x18");

}  // namespace wh::dialogmodule::data
