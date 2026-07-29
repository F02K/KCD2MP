#include "playermodule/C_AlchemyRecipeDatabase.h"
#include "Offsets/Offsets.h"

// C_AlchemyRecipeDatabase singleton accessor (KCD2 WHGame.dll 1.5.6, kd7u;
// ids = kcd2 address library).

namespace wh::playermodule {

C_AlchemyRecipeDatabase* C_AlchemyRecipeDatabase::GetInstance()
{
    // 0x185332AF0: the static-init instance itself (ctor sub_1819DE484).
    static REL::Relocation<C_AlchemyRecipeDatabase*> inst{ REL::ID(1251246) };
    return reinterpret_cast<C_AlchemyRecipeDatabase*>(inst.address());
}

C_AlchemyRecipe* C_AlchemyRecipeDatabase::GetRecipeById(uint32_t recipeId)
{
    return GetInstance()->GetById(recipeId);
}

}  // namespace wh::playermodule
