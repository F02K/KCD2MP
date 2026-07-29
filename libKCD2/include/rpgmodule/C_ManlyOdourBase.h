#pragma once

// -----------------------------------------------
// wh::rpgmodule::C_ManlyOdourBase -- empty marker/tag base of
// C_ManlyOdourStealthBuff (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD rva 0x4CE84D8.  CHD-proven secondary base of C_ManlyOdourStealthBuff
// at mdisp 0xD9 (MSVC empty non-EBO secondary base placement); contributes no
// vtable and no data.  Exists so the buff can be identified/cast by tag.

namespace wh::rpgmodule {

struct C_ManlyOdourBase {
    inline static constexpr auto RTTI = Offsets::RTTI_C_ManlyOdourBase;
};

}  // namespace wh::rpgmodule
