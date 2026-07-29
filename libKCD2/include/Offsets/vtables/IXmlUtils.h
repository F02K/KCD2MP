#pragma once
#include <cstdint>

// -----------------------------------------------
// IXmlUtils -- CrySystem XML utility singleton, KCD2 binary vtable order (impl CXmlUtils).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6.  Primary vtable 0x183A6CE40, exactly 18 slots (secondary
// vtable 0x183A6CDE0 belongs to an MI base -- do not use).  Instance: CSystem+0xA20,
// reached via Offsets::ISystem::GetXmlUtils() [132] (VERIFIED) -- cast the result.
// The KCD2 vtable does NOT follow the stock CryCommon IXml.h interfuscator order past
// the verified slots.  Both loaders return XmlNodeRef via the MSVC hidden out-pointer,
// declared here as an explicit IXmlNode** first parameter.
// Unnamed _vfN slots are real functions whose role is UNVERIFIED -- do not call them.

namespace Offsets {

struct IXmlNode;

struct IXmlUtils {
    virtual void _vf0() = 0;  // [0] 0x1824746E4  dtor region
    // [1] 0x1809792B4 -- load + parse an XML file (pak-aware).  VERIFIED by body: creates
    // parser 0x1813EE374, ParseFile via parser vf[3], then -- when bEnablePatching -- runs
    // the registered XML patcher (this+0x28, 0x182475824) over the tree.  Trailing bools
    // beyond the three stock args exist (parser-pool tuning); pass 0.
    virtual IXmlNode** LoadXmlFromFile(IXmlNode** out, const char* sFilename,
                                       bool bEnablePatching, bool b5, bool b6, bool b7) = 0;
    // [2] 0x1806E63CC -- parse an XML string buffer.  VERIFIED by body: creates parser
    // 0x1813EE374(pool, b6), parser vf[4] ParseBuffer(out, buffer, size, 1), releases the
    // parser.  Returns *out = root IXmlNode* with a reference the CALLER owns (release
    // with IXmlNode::Release when done).  bReuseStrings=true takes an unanalysed
    // stack-context path -- pass false.
    virtual IXmlNode** LoadXmlFromBuffer(IXmlNode** out, const char* buffer, uint32_t size,
                                         bool bReuseStrings, bool b6) = 0;
    virtual void _vf3() = 0;  // [3] 0x182479470
    virtual void _vf4() = 0;  // [4] 0x1809DD2D0
    virtual void _vf5() = 0;  // [5] 0x180A69F44
    virtual void _vf6() = 0;  // [6] 0x1806E41A8
    virtual void _vf7() = 0;  // [7] 0x182481DA0
    virtual void _vf8() = 0;  // [8] 0x18247B190
    virtual void _vf9() = 0;  // [9] 0x181A73990
    virtual void _vf10() = 0;  // [10] 0x18173B2D4
    virtual void _vf11() = 0;  // [11] 0x181209838
    virtual void _vf12() = 0;  // [12] 0x182476CA0
    virtual void _vf13() = 0;  // [13] 0x1803B6E80 nullsub
    virtual void _vf14() = 0;  // [14] 0x1824780C4
    virtual void _vf15() = 0;  // [15] 0x18248304C
    virtual void _vf16() = 0;  // [16] 0x18241BE00
    virtual void _vf17() = 0;  // [17] 0x182478DF8
};

}  // namespace Offsets
