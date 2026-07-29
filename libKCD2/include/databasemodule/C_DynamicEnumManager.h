#pragma once
#include <cstdint>
#include <utility>
#include <vector>

// -----------------------------------------------
// wh::databasemodule::C_DynamicEnumManager -- registry of I_DynamicEnum providers
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Not heap-created: lives EMBEDDED inside C_DatabaseModule at +0x98 (ctor sub_18192BBB4
// writes its vftable there; dtor chain destroys it via sub_182740C5C(this+0x98)).
// Body: 7 zeroed qwords + a byte set to 1 [container roles UNVERIFIED -- the databases'
// I_DynamicEnum subobjects (@+0xB8 of each concrete DB) register here].

namespace wh::databasemodule {

class C_DatabaseModule;
class C_DatabaseDynamicEnum;   // RTTI .?AVC_DatabaseDynamicEnum@databasemodule@wh@@

class C_DynamicEnumManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DynamicEnumManager;
    virtual ~C_DynamicEnumManager();   // slot layout not yet mapped

    // +0x08  sorted flat-map: enum-name hash -> provider. dtor sub_182740A4C->sub_180D16750
    // (0x10 stride, restores C_DatabaseDynamicEnum vftable & frees ptr@elem+8); insert
    // sub_180D16D40 (16B elems, lower_bound on first int32). 19 built-ins registered by sub_180D16B54.
    std::vector<std::pair<uint32_t, C_DatabaseDynamicEnum*>> m_enumsByType;  // +0x08 (first/last/end)
    uint64_t m_unk20;    // +0x20  ctor 0
    uint64_t m_unk28;    // +0x28  ctor 0
    uint64_t m_unk30;    // +0x30  ctor 0
    C_DatabaseModule* m_owner;  // +0x38  owner back-ref; set by init sub_180D16B54 (a2), fed as owner+0x10 (I_DatabaseModule) to each provider
    uint8_t  m_flag40;   // +0x40  ctor 1 [role UNVERIFIED]
    uint8_t  _pad41[7];  // +0x41
};
static_assert(sizeof(C_DynamicEnumManager) == 0x48,
              "C_DynamicEnumManager must be 0x48 (embedded at C_DatabaseModule+0x98..0xDF)");

}  // namespace wh::databasemodule
