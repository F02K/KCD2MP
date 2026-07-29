#include "guimodule/E_MarkType.h"
#include "Offsets/Offsets.h"

// E_MarkType helpers -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).

namespace wh::guimodule {

const char* GetMarkTypeName(E_MarkType::Type type)
{
    // sub_180C4D664: the mark_type -> name switch (97 cases; default "?" =
    // unk_183B797F0). Source of every enumerator name in E_MarkType.h.
    using Fn = const char*(__fastcall*)(int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(30) };
    return fn(static_cast<int32_t>(type));
}

}  // namespace wh::guimodule
