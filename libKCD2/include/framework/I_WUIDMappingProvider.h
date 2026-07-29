#pragma once

// -----------------------------------------------
// wh::framework::I_WUIDMappingProvider -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Interface that resolves WUID <-> value/entity mappings. Polymorphic base of C_InventoryManager,
// providing its 3-slot vtable (GetValueForWuid, GetWuidForKey, dtor). The dtor is the LAST slot.

namespace wh::framework {

class I_WUIDMappingProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_WUIDMappingProvider;
    virtual void* GetValueForWuid(const void* wuid) = 0;         // [0]  sub_1823DCBE4 (hash-map lookup)
    virtual void  GetWuidForKey(void* out, const void* key) = 0; // [1]  sub_182A1FA50 (slot-table lookup)
    virtual ~I_WUIDMappingProvider() = default;                  // [2]
};

}  // namespace wh::framework
