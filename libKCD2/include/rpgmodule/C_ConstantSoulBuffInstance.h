#pragma once
#include "C_SoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_ConstantSoulBuffInstance : C_SoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_ConstantSoulBuffInstance@rpgmodule@wh@@ (TD 0x184CE4DA0)  vtable 0x183EFA5F0
// in-place ctor sub_182BC515C = {C_SoulBuffInstance ctor sub_18046C4DC; set vtable} -- adds NO
// members (pure behavioral specialization). make-with-def sub_1810A423C allocs 0xD8.
// The always-on ("constant") buff leaf: modifiers/duration/tags are NOT copied per-instance --
// they are resolved from the buff database via C_BuffInstanceBase::m_buffDefinitionId (WUID
// @+0x08), which is why this and most sibling leaves stay at the base 0xD8.
// Default-definition path: the make fn compares the incoming def WUID against a per-type
// sentinel global (qword_185326B58 -- RUNTIME-populated, all-zero in the on-disk image) and on
// match sub_18046C474 CONSTRUCTS a default id (generated 7-byte value | tag byte 0x01), it does
// not look one up.

namespace wh::rpgmodule {

class C_ConstantSoulBuffInstance : public C_SoulBuffInstance   // +0x00  (0xD8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ConstantSoulBuffInstance;
};
static_assert(sizeof(C_ConstantSoulBuffInstance) == 0xD8, "C_ConstantSoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
