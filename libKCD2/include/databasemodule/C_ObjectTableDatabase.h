#pragma once
#include <cstdint>
#include <string>
#include "C_ObjectDatabase.h"

// -----------------------------------------------
// wh::databasemodule::C_ObjectTableDatabase<TRow, TDBData> -- game-table-backed database layer
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template (e.g. C_ObjectTableDatabase<S_ReputationChange, S_ReputationChangeDBData>;
// its vftable is written mid-ctor by sub_1811CEFE4). Binds the row container to a Tables.pak
// table: Register (base slot [7]) asks I_DatabaseModule for the table by name and stores the
// 88-byte table-metadata handle @+0x48; Load (base slot [9]) loads by name, verifies the handle
// type, then ParseRows ([17]).
// Strings are MSVC std::string (SSO buffer @+0x50/+0x70, size/capacity pairs -- GetTableName
// checks capacity > 0xF for the heap-ptr case).
// TDBData: per-row DB payload type (e.g. S_ReputationChangeDBData) -- consumed by the row parser;
// not stored inline here [role attribution INFERRED from the template signature].

namespace wh::databasemodule {

template <typename TRow, typename TDBData>
class C_ObjectTableDatabase : public C_ObjectDatabase<TRow> {
public:
    void*       m_pTableMeta;   // +0x48  88-byte table-metadata handle from I_DatabaseModule[12]; set by Register slot[7]; Load 0x180EF3A4C reads +0x38 type dword vs m_type:
                                //        {+0x00 raw row buffer, +0x08 row count (dword), +0x38 type
                                //        dword (verified by Load), +0x50 flags (bit3 = needs load)}
                                //        -- the KCD1 "loaded table view" reborn as a handle struct
    std::string m_tableName;    // +0x50  e.g. "reputation_change"
    std::string m_groupName;    // +0x70  e.g. "rpg"
    int32_t     m_unk90;        // +0x90  ctor 0; setter slot[21] sub_181A7E520 stores its int arg (mov [this+0x90],esi) -> int32_t; semantic role unresolved
    bool        m_isLoaded;     // +0x94  set by Load; read by IsLoaded [6]
    uint8_t     _pad95[3];      // +0x95
};

}  // namespace wh::databasemodule
