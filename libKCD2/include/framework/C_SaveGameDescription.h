#pragma once
#include <cstdint>
#include <vector>
#include "E_SaveGameType.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::framework::C_SaveGameDescription -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD0.
// -----------------------------------------------
// RTTI .?AVC_SaveGameDescription@framework@wh@@ (TD 0x184A686A0, COL 0x18416FCB8)
// vtable 0x183A53050  ctor sub_180794684  populate sub_181E21850  alloc 208 in CreateSaveGame
// (sub_180794928). The per-save metadata header: one per .whs file, kept in the manager's
// per-type slot lists and (de)serialized as chunk id 501 (reader sub_180C3DB48).
// m_descriptionLine format: "%d|%d|%s|%s|%d|%s|%f|" = type|index|questName|location|timestamp|
// dateStr|playtimeHours. Save lookup matches m_saveName by strcmp (sub_1825BD2EC).
// m_fileName templates by type: "permanent%03d.whs" / "autosave%03d.whs" / "save%03d.whs" /
// "quicksave%03d.whs" / "exit.whs" / "crucialdecision%03d.whs".

namespace wh { struct S_ModInfo; }   // RTTI .?AUS_ModInfo@wh@@ -- 0x40: vptr + 6 CryStringT<char> + flag byte (rttr-registered)

namespace wh::framework {

class C_SaveGameDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SaveGameDescription;
    // DLC presence record (RTTR-confirmed vector<C_DLCDescription> element; interior not RE'd).
    class C_DLCDescription;

    virtual ~C_SaveGameDescription() = default;   // +0x00  vptr (slot order UNVERIFIED)
    E_SaveGameType m_saveType;        // +0x08  (uint8; see E_SaveGameType.h)
    uint8_t  _pad09[3];               // +0x09
    int32_t  m_saveIndex;             // +0x0C  %03d slot number (default -1)
    int64_t  m_timestamp;             // +0x10  __time64_t unix time (populate calls _time64)
    CryStringT<char> m_streamBuildVersion;  // +0x18
    int32_t  m_field20;               // +0x20  int32 (ctor -1); serialized int32 (chunk501 sub_1823CA40C @0x180C3DBEF); populate sets from a framework subobject vf (@0x181E21963)
    uint32_t _pad24;                  // +0x24
    CryStringT<char> m_saveName;      // +0x28  quest/user save name (lookup key)
    CryStringT<char> m_descriptionLine;  // +0x30  pipe-delimited summary (format above)
    uint8_t  m_field38;               // +0x38  uint8 (ctor 0 @0x1807946D1); no writer/reader across ctor/populate/chunk501/dtor -- role UNRESOLVED
    uint8_t  _pad39[3];               // +0x39
    int32_t  m_field3C;               // +0x3C  int32 (ctor 0 @0x1807946D5); no writer/reader across ctor/populate/chunk501/dtor -- role UNRESOLVED
    CryStringT<char> m_buildInfo;     // +0x40  cvar wh_sys_BuildInfo
    CryStringT<char> m_assemblyDate;  // +0x48  cvar wh_sys_assembly_date
    int32_t  m_gameReleaseVersion;    // +0x50  int32; ctor = sub_1808B0B34() = cvar wh_sys_GameReleaseVersion parsed (0x7FFFFFFF if unset); serialized int32 (chunk501 @0x180C3DC13)
    int32_t  m_field54;               // +0x54  int32; = 4th populate arg (CreateSaveGame passes *(manager+0x18C) @0x181E2192E); serialized int32 (chunk501 @0x180C3DC1F)
    uint8_t  m_installCompleted;      // +0x58  default 1; recomputed from
                                      //        wh_sys_GameSaveInstallCompleted
    uint8_t  _pad59[7];               // +0x59
    std::vector<C_DLCDescription> m_dlcList;  // +0x60  RTTR-confirmed (element interior not RE'd)
    int32_t  m_field78;               // +0x78  int32 (ctor 1); serialized int32 (chunk id 0x10 @0x180C3DCB6); populate sets from qword_18492D890 vf (@0x181E21982)
    uint32_t _pad7C;                  // +0x7C
    CryStringT<char> m_fileName;      // +0x80  per-type template (see banner)
    std::vector<S_ModInfo> m_modList;             // +0x88  {begin,end,cap}; move-assigned from the mod
                                                  //        system (sub_1818C9BF8) and filled by chunk-0xE
                                                  //        reader; element ctor sub_182434BD8 = vptr+6
                                                  //        CryStringT<char>+byte (0x40)
    uint64_t m_locationHandleA;       // +0xA0  location-handle pair written by populate (v30
    uint64_t m_locationHandleB;       // +0xA8  pair; semantics UNRESOLVED)
    uint64_t _unkB0[4];               // +0xB0..+0xD0  ctor-zeroed, unread in examined paths
};
static_assert(sizeof(C_SaveGameDescription) == 0xD0, "C_SaveGameDescription must be 0xD0");
static_assert(offsetof(C_SaveGameDescription, m_saveType) == 0x08, "type at 0x08");
static_assert(offsetof(C_SaveGameDescription, m_dlcList) == 0x60, "dlc list at 0x60");
static_assert(offsetof(C_SaveGameDescription, m_fileName) == 0x80, "file name at 0x80");

}  // namespace wh::framework
