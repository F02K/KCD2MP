#include "guimodule/ItemUiPresentation.h"

namespace wh::guimodule {

void GetItemUiPresentation(const CryGUID& itemClass, uint32_t priceContext,
                           CryStringT<char>& outName, CryStringT<char>& outIcon,
                           float& outPrice)
{
    // sub_180515434: a1 is pure stack scratch (overwritten before any use) -- pass 0.
    using Fn = void(__fastcall*)(uint64_t, const CryGUID*, uint32_t,
                                 CryStringT<char>*, CryStringT<char>*, float*);
    static REL::Relocation<Fn> fn{ REL::ID(30029) };
    fn(0, &itemClass, priceContext, &outName, &outIcon, &outPrice);
}

}  // namespace wh::guimodule
