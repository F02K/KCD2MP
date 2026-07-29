#pragma once
#include <cstdint>
#include "guimodule/C_CompassMark.h"

// -----------------------------------------------
// wh::guimodule::C_CheckpointMark -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI .?AVC_CheckpointMark@guimodule@wh@@ (TD 0x184C92430).
// Base: C_CompassMark @+0x00 (COLs 0x4349CF8/+0x00, 0x4349D20/+0x08, 0x4349CD0/+0x10;
// vtables 0x1847200E8 / 0x1847200A0 / 0x1847200B0 -- only per-class dtor thunks and the
// rttr trio differ from the base).  ctor sub_181F47B00 = C_CompassMark(type=0, obj=null,
// pos, param) + vtable swap; ALWAYS make_shared'd via sub_182AFB3DC(out, const Vec3* pos,
// const int* levelId) (alloc 0x90 = _Ref_count_obj2<C_CheckpointMark> 0x10 + 0x80,
// TD 0x184C91340), which also writes m_levelId and wires enable_shared_from_this.
//
// The player-placed custom map marker ("checkpoint").  Owned by
// C_UIMap::m_checkpointMarks (+0x588) -- ONE PER LEVEL (lookup sub_180DCB460 compares
// m_levelId against the current level id).  Created from the save-game stream
// (C_UIMap serializer sub_180DCA18C, chunk 0xC20: Vec3 + int per mark) and from the
// place-at-cursor action (sub_182AFDC74).  UIMap.GoToCheckpointMark (Lua, 0x182BA7810)
// teleports the player to m_pos of the current level's mark.
//
// [MODERATE vs KCD1] KCD1 kept a single shared_ptr<C_CompassMark> checkpoint
// (C_UIMap+0x180); KCD2 subclasses the mark and keeps a per-level vector.

namespace wh::guimodule {

class C_CheckpointMark : public C_CompassMark {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CheckpointMark;
    int32_t m_levelId;    // +0x78  level the mark was placed on (make_shared wrapper arg)
    uint32_t _pad7C;      // +0x7C
};
static_assert(sizeof(C_CheckpointMark) == 0x80, "C_CheckpointMark must be 0x80");
static_assert(offsetof(C_CheckpointMark, m_levelId) == 0x78, "level id at 0x78");

}  // namespace wh::guimodule
