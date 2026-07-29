#pragma once
#include <cstdint>
#include <vector>
#include "E_QuestProgress.h"
#include "../conceptmodule/C_ModuleBase.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../framework/C_LocalizedString.h"
#include "../framework/S_ResourceHandle.h"

// -----------------------------------------------
// wh::questmodule::C_Quest -- quest concept node (KCD2 1.5.6, kd7u).  sizeof 0x150 (alloc 336).
// -----------------------------------------------
// Own vtable 0x183ED4620; ctor sub_180DC4B80 (C_ModuleBase chain sub_1806B193C). Size also
// proven by C_ActivityUnit (IS-A C_Quest, adds nothing, alloc 336).
// CHEAT: quest state get/set goes through the +0xB0 progress port
// (C_TypedPortRef<E_QuestProgress>); resolve quest nodes via C_QuestManager
// (C_QuestModule+0x10).

namespace wh::questmodule {

class C_Quest : public conceptmodule::C_ModuleBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Quest;
    framework::S_ResourceHandle m_questRef;                          // +0xA0  (&unk_1856691C8) [target UNVERIFIED]
    conceptmodule::C_TypedPortRef<E_QuestProgress> m_progressPort;   // +0xB0  quest-progress port (cheat lever)
    uint32_t m_paramF0;                                              // +0xF0  ctor 0 [role UNVERIFIED]
    uint32_t _padF4;                                                 // +0xF4
    framework::C_LocalizedString m_locF8;                            // +0xF8  [name/desc split UNVERIFIED]
    framework::C_LocalizedString m_loc108;                           // +0x108 [ditto]
    uint32_t m_param118;                                             // +0x118 ctor 1 [role UNVERIFIED]
    uint8_t  m_flag11C;                                              // +0x11C ctor 0 [role UNVERIFIED]
    uint8_t  _pad11D[3];                                             // +0x11D
    std::vector<uint64_t> m_vec120;                                  // +0x120  std::vector, 8-byte elems (dtor sub_1804134B4 frees (end-first)&~7); elem type UNVERIFIED
    std::vector<uint32_t> m_vec138;                                  // +0x138  std::vector, 4-byte elems (dtor sub_180A608D4 frees (end-first)&~3); elem type UNVERIFIED
};
static_assert(sizeof(C_Quest) == 0x150, "C_Quest must be 0x150 (alloc 336)");
static_assert(offsetof(C_Quest, m_progressPort) == 0xB0, "progress port at +0xB0");

}  // namespace wh::questmodule
