#pragma once
#include <cstdint>
#include "I_SmartObjectHelpersManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObjectHelpersManager : I_SmartObjectHelpersManager
// -- registry of smart-object helpers, owned by C_SmartObjectsManager (+0xB8)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x50 (create-site proven: new 80
// inside the C_SmartObjectsManager ctor @0x180beb573).  ctor sub_1807CEFE4;
// registers callback sub_1832F0040 into a manager list.
// -----------------------------------------------
// RTTI TD rva 0x4F5B150 (2 bases).  Vtable rva 0x3FECED8 (5 slots:
// [0]0x32EE060 [1]0x1096C08 [2]0x32EF8C0 [3]0x67CAB8 [4]0x1096CA4).

namespace wh::xgenaimodule {

class C_SmartObjectHelpersManager : public I_SmartObjectHelpersManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObjectHelpersManager;
    void SohVf0() override; void SohVf1() override; void SohVf2() override;
    void SohVf3() override; void SohVf4() override;

    float    m_scale08;       // +0x08  ctor: 1.0f [U role]
    uint8_t  _pad0C[4];       // +0x0C
    void*    m_listHead;      // +0x10  unordered_map<CryStringT<char>,void*> _List._Myhead: 32B sentinel {next,prev,pair<CryStringT<char>,void*>}; whole map = +0x08..+0x48
    size_t   m_helpersCount;  // +0x18  unordered_map _List._Mysize element count (ctor 0; erase does --count)
    uint8_t  m_hash20[0x28];  // +0x20..+0x48  hash-map (mask 7 @+0x38, count 8 @+0x40) [U interior]
    uint8_t  m_byte48;        // +0x48  ctor: 0 [U role]
    uint8_t  _pad49[7];       // +0x49
};
static_assert(sizeof(C_SmartObjectHelpersManager) == 0x50, "C_SmartObjectHelpersManager must be 0x50 (new 80)");

}  // namespace wh::xgenaimodule
