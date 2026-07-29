#pragma once
#include <cstdint>

// -----------------------------------------------
// SFlashDisplayInfo — Flash display-object transform block (KCD2 BINARY layout)
// -----------------------------------------------
// CryCommon/IFlashPlayer.h type, GLOBAL namespace. KCD2 1.5.6: vs the stock CE3
// SDK shape (0x2C, m_varsSet @+0x2A), the binary's GetDisplayInfo/SetDisplayInfo
// slots ([25]/[26]) use sizeof 0x38 with 8 unknown/new bytes @+0x2C..+0x33 and
// m_varsSet moved to +0x34. Float block +0x00..+0x27, visible byte +0x28 and the
// FDIF_* bit values are unchanged. Proven from the CFlashPlayer slot bodies
// (analysis/guimodule_port/flash_player_layer.md); +0x2C..+0x33 are unread by
// those two slots — contents UNVERIFIED, zero-init for safety.
//
// Do NOT include the stock CryCommon/IFlashPlayer.h in a TU using this replica
// (it defines the stale shape under the same name). SDK inline accessors kept.

struct SFlashDisplayInfo {
    enum Flags {
        FDIF_X         = 0x001,
        FDIF_Y         = 0x002,
        FDIF_Z         = 0x004,

        FDIF_XScale    = 0x008,
        FDIF_YScale    = 0x010,
        FDIF_ZScale    = 0x020,

        FDIF_Rotation  = 0x040,
        FDIF_XRotation = 0x080,
        FDIF_YRotation = 0x100,

        FDIF_Alpha     = 0x200,
        FDIF_Visible   = 0x400,
    };

    void Clear() { m_varsSet = 0; }

    void SetX(float x) { SetFlags(FDIF_X); m_x = x; }
    void SetY(float y) { SetFlags(FDIF_Y); m_y = y; }
    void SetZ(float z) { SetFlags(FDIF_Z); m_z = z; }

    void SetXScale(float xscale) { SetFlags(FDIF_XScale); m_xscale = xscale; }  // 100 == 100%
    void SetYScale(float yscale) { SetFlags(FDIF_YScale); m_yscale = yscale; }  // 100 == 100%
    void SetZScale(float zscale) { SetFlags(FDIF_ZScale); m_zscale = zscale; }  // 100 == 100%

    void SetRotation(float degrees)  { SetFlags(FDIF_Rotation);  m_rotation  = degrees; }
    void SetXRotation(float degrees) { SetFlags(FDIF_XRotation); m_xrotation = degrees; }
    void SetYRotation(float degrees) { SetFlags(FDIF_YRotation); m_yrotation = degrees; }

    void SetAlpha(float alpha)    { SetFlags(FDIF_Alpha);   m_alpha   = alpha; }
    void SetVisible(bool visible) { SetFlags(FDIF_Visible); m_visible = visible; }

    void SetPosition(float x, float y)          { SetFlags(FDIF_X | FDIF_Y);          m_x = x; m_y = y; }
    void SetPosition(float x, float y, float z) { SetFlags(FDIF_X | FDIF_Y | FDIF_Z); m_x = x; m_y = y; m_z = z; }

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetZ() const { return m_z; }

    float GetXScale() const { return m_xscale; }
    float GetYScale() const { return m_yscale; }
    float GetZScale() const { return m_zscale; }

    float GetRotation() const  { return m_rotation; }
    float GetXRotation() const { return m_xrotation; }
    float GetYRotation() const { return m_yrotation; }

    float GetAlpha() const   { return m_alpha; }
    bool  GetVisible() const { return m_visible; }

    bool IsAnyFlagSet() const            { return 0 != m_varsSet; }
    bool IsFlagSet(unsigned int f) const { return 0 != (m_varsSet & f); }

private:
    void SetFlags(unsigned int flags) { m_varsSet |= static_cast<unsigned short>(flags); }

    float          m_x = 0;                 // +0x00
    float          m_y = 0;                 // +0x04
    float          m_z = 0;                 // +0x08
    float          m_xscale = 0;            // +0x0C
    float          m_yscale = 0;            // +0x10
    float          m_zscale = 0;            // +0x14
    float          m_rotation = 0;          // +0x18
    float          m_xrotation = 0;         // +0x1C
    float          m_yrotation = 0;         // +0x20
    float          m_alpha = 0;             // +0x24
    bool           m_visible = false;       // +0x28
    unsigned char  m_pad29[3] = {};         // +0x29
    unsigned char  m_unknown2C[8] = {};     // +0x2C  [KCD2-new; contents UNVERIFIED]
    unsigned short m_varsSet = 0;           // +0x34  (stock CE3 had it at +0x2A)
};
static_assert(sizeof(SFlashDisplayInfo) == 0x38, "KCD2 binary SFlashDisplayInfo is 0x38");
