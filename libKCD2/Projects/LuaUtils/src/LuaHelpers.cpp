#include "LuaHelpers.h"

#include "Offsets/vtables/IFunctionHandler.h"
#include "Offsets/vtables/IScriptSystem.h"
#include "Offsets/vtables/IScriptTable.h"
#include "Offsets/vtables/IEntity.h"
#include "entitymodule/C_Item.h"
#include "entitymodule/S_ItemClass.h"

namespace luautils {

bool GetHandleParam(Offsets::IFunctionHandler* pH, int nIdx, uint64_t& out)
{
    ScriptAnyValue any;
    any.type = ANY_THANDLE;
    any.nHandle = 0;
    if (!pH->GetParamAny(nIdx, any))
        return false;
    out = (uint64_t)any.nHandle;
    return true;
}

Offsets::IScriptTable* BuildItemTable(Offsets::IScriptSystem* pSS, wh::entitymodule::C_Item* item)
{
    using namespace wh::entitymodule;

    Offsets::IScriptTable* t = pSS->CreateTable(0, 0);
    if (!t)
        return nullptr;

    t->SetValueAny("id", HandleValue(item->m_wuid.m_value));
    if (S_ItemClass* cls = item->m_pClassData) {
        t->SetValueAny("className", ScriptAnyValue(cls->m_name.c_str()));
        t->SetValueAny("type", ScriptAnyValue((int)cls->GetTypeId()));
    }
    t->SetValueAny("amount", ScriptAnyValue(item->m_amount));
    t->SetValueAny("health", ScriptAnyValue(item->m_health));
    t->SetValueAny("condition", ScriptAnyValue(item->GetCondition()));
    t->SetValueAny("quality", ScriptAnyValue(item->GetQuality()));
    t->SetValueAny("maxQuality", ScriptAnyValue(item->GetMaxQuality()));
    t->SetValueAny("isEquipped", ScriptAnyValue((item->m_flags & 1) != 0));

    wh::framework::WUID owner{};
    item->GetOwnerHandle(owner);
    t->SetValueAny("owner", HandleValue(owner.m_value));

    if (item->m_pLinkedEntity)
        t->SetValueAny("entity", HandleValue(item->m_pLinkedEntity->GetId()));

    return t;
}

}  // namespace luautils
