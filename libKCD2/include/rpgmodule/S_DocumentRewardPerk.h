#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DocumentRewardPerk -- document_reward_perk table row
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x24, ALIGN 4 (PROVEN: C_DocumentRewardPerkDatabase ParseRows 0x1819C023C copies blob
// rows with stride 36 -- {16B @0, 16B @16, dword @32} -- and its vector push helpers
// (sub_180B1F154 / sub_182C7AE48) are COMDAT-folded with S_PerkCompanion's 36-byte pushes, so
// the in-vector element is packed to 36).  The 16-byte ids therefore CANNOT be CryGUID-typed
// here (8-byte alignment would pad to 0x28) -- mirrored as uint32[4] raw GUID values.
// Roles [INFERRED from the table name].

namespace wh::rpgmodule {

struct S_DocumentRewardPerk {
    uint32_t m_documentId[4];   // +0x00  16-byte GUID value, 4-aligned [role INFERRED]
    uint32_t m_perkId[4];       // +0x10  16-byte GUID value, 4-aligned [role INFERRED]
    int32_t  m_param20;         // +0x20  [role UNVERIFIED]
};
static_assert(sizeof(S_DocumentRewardPerk) == 0x24 && alignof(S_DocumentRewardPerk) == 4,
              "S_DocumentRewardPerk must be 0x24/align-4 (folded 36-byte vector pushes)");

}  // namespace wh::rpgmodule
