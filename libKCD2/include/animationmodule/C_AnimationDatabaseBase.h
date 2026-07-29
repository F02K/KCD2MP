#pragma once
#include <cstdint>
#include <unordered_map>
#include "C_ObjectAnimationDatabaseGenerated.h"
#include "I_AdbChangeListener.h"

// -----------------------------------------------
// wh::animationmodule::C_AnimationDatabaseBase<TData, TDBData, TObjAnimGen> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_AnimationDatabaseBase@US_CombatActionAttackData@..@US_CombatActionAttackDataDBData@..@
// V?$C_ObjectAnimationDatabaseGenerated@...@@animationmodule@wh@@ . The substantive animation-DB
// layer (the shared "tree/animation database" the prior lead named). It:
//   * multiply-inherits I_AdbChangeListener (MI subobject @+0x98) so the DB is notified on ADB
//     reload, and
//   * owns the animation-side OBJECT INDEX -- an MSVC std::unordered_map keyed by the loaded row's
//     key (id or name) to its TData* -- used for fast query resolution.
//
// Layout evidence (attack DB ctor sub_1810DB470, "C_AnimationDatabaseGenerated" vtable phase, which
// constructs this base's members):
//   +0x98  I_AdbChangeListener vtable (MI base; final vtable 0x183E50CB0)
//   +0xA0  uint16 m_flagA0 = 0
//   +0xA8  std::unordered_map (0x40): load-factor 1.0f @+0xA8, list head @+0xB0, bucket vector
//          @+0xC0/+0xD0, mask/maxidx @+0xE0=8 -- matches the MSVC _Hash signature (rule 7).
// (Also: C_ObjectTableDatabase::m_unk90 @+0x90 is set to 2 here.) The map's exact key/value types
// are not individually proven; modeled as <uint64, TData*> [key/value UNVERIFIED].

namespace wh::animationmodule {

template <typename TData, typename TDBData,
          typename TObjAnimGen = C_ObjectAnimationDatabaseGenerated<TData, TDBData>>
class C_AnimationDatabaseBase : public TObjAnimGen, public I_AdbChangeListener {
public:
    uint16_t m_flagA0;                                  // +0xA0  ctor 0
    // +0xA8  animation-side object index (name/id -> TData*), 0x40 MSVC unordered_map
    std::unordered_map<uint64_t, TData*> m_objectIndex; // +0xA8 [key/value UNVERIFIED]
};

}  // namespace wh::animationmodule
