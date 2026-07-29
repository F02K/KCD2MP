#include "framework/C_LocalizedString.h"
#include "Offsets/Offsets.h"

// C_LocalizedString helpers -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::framework {

CryStringT<char> C_LocalizedString::Resolve(const CryStringT<char>& label)
{
    // sub_1803C235C: the UI label localizer (returns the label verbatim when it
    // has no loc entry). Returns by value -- the binary constructs the result
    // into the caller-provided return slot, which is exactly what this
    // signature compiles to.
    using Fn = CryStringT<char>(__fastcall*)(const CryStringT<char>&);
    static REL::Relocation<Fn> fn{ REL::ID(1) };
    return fn(label);
}

}  // namespace wh::framework
