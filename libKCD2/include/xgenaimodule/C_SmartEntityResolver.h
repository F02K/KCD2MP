#pragma once
#include <cstdint>
#include "I_SmartObjectsManagerListener.h"
#include "I_SmartAreasManagerListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEntityResolver : I_SmartObjectsManagerListener
// (@+0x0) + I_SmartAreasManagerListener (@+0x8) -- resolves smart entities via
// manager events; SINGLETON at qword_185496120 (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x30 (create-site proven: sub_180AF26C8 @0x180af2715:
// qword_18549D378(48,&v6) -> ctor sub_180AF2764).
// -----------------------------------------------
// RTTI TD rva 0x4F5A4F0.  Vtables: +0x0 rva 0x3A78C78 (4 slots: [0]0xA2A8D8
// [1]0x1491830 [2]nullsub [3]deleting-dtor thunk 0x32D6B30); +0x8 rva
// 0x3A78C58 (3 slots: [0]0xD9BBF4 [1]0x3FBC58 [2]nullsub).  ctor subscribes to
// the SmartObjectsManager listener list and to C_SmartAreaManager (vtbl+128,
// passing this+8).  Alt construction path sub_1832D6640.

namespace wh::xgenaimodule {

class C_SmartEntityResolver
    : public I_SmartObjectsManagerListener,
      public I_SmartAreasManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEntityResolver;
    // I_SmartObjectsManagerListener impls
    void SomlVf0() override;                    // [0] 0xA2A8D8
    void SomlVf1() override;                    // [1] 0x1491830
    void SomlVf2() override;                    // [2] nullsub
    ~C_SmartEntityResolver() override;          // [3] deleting-dtor thunk 0x32D6B30
    // I_SmartAreasManagerListener impls
    void _vf0() override;                       // [0] 0xD9BBF4
    void OnSmartAreaRemoved(void* area) override; // [1] 0x3FBC58
    void _vf2() override;                       // [2] nullsub

    void*   m_defaultRepA;   // +0x10  default 'rep' = shared-empty static unk_18566BE78 of a mutable length-prefixed char-buffer string ({u32 cap@ptr-8, u32 len@ptr-4}; resized by sub_180BD7F70) [U string class name]
    int16_t m_idA0;          // +0x18  ctor: -1
    int16_t m_idA1;          // +0x1A  ctor: -1
    uint8_t _pad1C[4];       // +0x1C
    void*   m_defaultRepB;   // +0x20  &unk_18566BE78 -- same shared-empty length-prefixed string sentinel as m_defaultRepA
    int16_t m_idB0;          // +0x28  ctor: -1
    int16_t m_idB1;          // +0x2A  ctor: -1
    uint8_t _pad2C[4];       // +0x2C
};
static_assert(sizeof(C_SmartEntityResolver) == 0x30, "C_SmartEntityResolver must be 0x30 (alloc 48 at sub_180AF26C8)");

}  // namespace wh::xgenaimodule
