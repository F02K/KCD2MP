#pragma once

// -----------------------------------------------
// ISubtitleHandler -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AUISubtitleHandler@@ (global namespace; CryAction
// ISubtitleManager.h -- interfuscator-eligible, so this replica follows the BINARY
// slot order, not the SDK header).
//
// Shape evidence: C_UISubtitles' ISubtitleHandler subobject (@+0x60, COL 0x41361E8,
// vtable 0x183C33578) = 3 slots: [0] deleting-dtor adjustor thunk 0x18213A548
// (-0x60 -> 0x182B8C08C), [1] 0x182B8F160 = ShowSubtitle(const char* localizationKey,
// bool bShow) -- crc32s the key (sub_1805AFA90), forwards to I_UISubtitles::ShowSubtitle
// / ClearSubtitles and caches the crc at owner+0x78 -- [2] nullsub_1. The SDK's second
// ShowSubtitle overload (SSubtitleInfo) collapses to the nullsub slot in this build;
// which of [1]/[2] is which overload is pinned by the observed body only.

namespace Offsets {

struct ISubtitleHandler {
    virtual ~ISubtitleHandler() = default;                              // [0]
    virtual void ShowSubtitle(const char* szKey, bool bShow) = 0;       // [1] 0x182B8F160 (C_UISubtitles impl)
    virtual void ShowSubtitleInfo(const void* pSubtitleInfo, bool bShow);  // [2] nullsub in C_UISubtitles (SDK SSubtitleInfo overload; name from SDK)
};
static_assert(sizeof(ISubtitleHandler) == 8);

}  // namespace Offsets
