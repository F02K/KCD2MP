#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ReputationChange -- reputation_change table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven row type of C_ReputationChangeDatabase (C_ObjectTableDatabase<S_ReputationChange,
// S_ReputationChangeDBData>). Row stride 0x28 (PROVEN: the database registers its table metadata
// with rowStride 40). Field map from the S_ReputationChangeRequest ctor (sub_180785330), which
// reads the row at +0x00/+0x10/+0x14/+0x15/+0x18; the +0x04..+0x0F gap has no observed reader.
// Source data: Libs/Tables/rpg/reputation_change.xml.

namespace wh::rpgmodule {

struct S_ReputationChange {
    int32_t  m_id;                  // +0x00  row id (request.changeId)
    uint8_t          _pad04[4];     // +0x04  alignment before name pointer
    CryStringT<char> m_name;        // +0x08  reputation_change 'name' column (loader sub_181334244: converter sub_180942380 -> CryStringT::operator= sub_1804F3344)
    float    m_change;              // +0x10  base delta (scaled by weight + player derived stats 70/71)
    uint8_t  m_targetBits;          // +0x14  target layer bitmask
    uint8_t  m_canChangeHostility;  // +0x15
    uint8_t  _pad16[2];             // +0x16
    float    m_clamp;               // +0x18  +/-FLT_MAX = no clamp
    int32_t  m_notificationId;      // +0x1C  reputation_notification_id (loader sub_181334244 0x1813342d3 stores src int32 @+0x20)
    uint8_t  m_field20;             // +0x20  raw byte column (loader 0x1813342d9 stores src byte @+0x24; KCD2-added boolean, not in KCD1 schema) [role UNVERIFIED]
    uint8_t  _pad21[7];             // +0x21
};
static_assert(sizeof(S_ReputationChange) == 0x28, "S_ReputationChange must be 0x28 (registered row stride 40)");

}  // namespace wh::rpgmodule
