#pragma once
#include <cstdint>

// -----------------------------------------------
// rttr::variant -- KCD2 WHGame.dll 1.5.6 (e4cp).  sizeof 0x18.
// -----------------------------------------------
// master/0.9.7-dev shape, data-FIRST: 16-byte variant_data storage + one policy fn ptr.
// Layout PROVEN multiple ways: move-ctor 0x1806926A8 / move-assign 0x1804F8734 (copy
// [+0x10] then invoke it); is_valid 0x1804F9CDC / is_nullptr 0x1804F8678 one-liners
// (`mov rax,[rcx+10h]; jmp rax`, op in cl); C_ConstantInterfacePort default ctor
// (policy 0x1804FBC18 at +0x68, variant at +0x58); C_DataPort::GetValueImpl frame.
// Policy ABI: bool policy(uint8_t op, variant_data* src, void* arg). Verified ops
// (agent_metadata_variant.md): 0 DESTROY, 1 CLONE, 2 SWAP, 5 GET_VALUE, 6 GET_TYPE,
// 7 GET_PTR, 8 GET_RAW_TYPE, 9 GET_RAW_PTR, 0xA GET_ADDRESS_CONTAINER, 0xF IS_VALID,
// 0x10 IS_NULLPTR, 0x11 CONVERT, 0x12 COMPARE_EQUAL, 0x13 COMPARE_LESS; 0xB..0xE
// container/view ops positionally inferred [UNVERIFIED]. Values <= 0x10 bytes live
// inline; big-policy types hold a heap ptr in m_storage[0] (std::string policy
// 0x1804F9D78, new 0x20); E_ConceptFunctionMetadata int32 policy 0x1804FA438 (WH
// metadata keys). Invalid/empty sentinel policy: 0x1804FBC18 (create_variant default;
// node factory failure; empty ports). Policy-managed -- tool code must not copy/destroy
// variants by hand; treat as opaque unless calling binary helpers.

namespace rttr {
namespace detail {

struct variant_data {      // 16-byte storage: small types inline, big-policy types hold a
    void* m_storage[2];    // heap ptr in [0] (upper 8B untouched by any policy inspected)
};

}  // namespace detail

class variant {
public:
    using policy_func = bool (*)(std::uint8_t op, detail::variant_data* src, void* arg);

    detail::variant_data m_data;   // +0x00  in-place value storage (or heap ptr in [0])
    policy_func          m_policy; // +0x10  per-type ops table fn; 0x1804FBC18 = invalid
};
static_assert(sizeof(variant) == 0x18, "rttr::variant is {16B data, policy ptr}");

}  // namespace rttr
