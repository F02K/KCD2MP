#pragma once
#include "C_BuffInstanceBase.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffInstance<TOwner> -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x90 (for C_Soul).
// -----------------------------------------------
// RTTI .?AV?$C_BuffInstance@VC_Soul@rpgmodule@wh@@@rpgmodule@wh@@ (TD 0x184CE4E10)
// vtable 0x183A38C38  scalar-deleting dtor sub_18063A7CC.  Intermediate template layer between
// C_BuffInstanceBase and C_SoulBuffInstance; adds ONLY the owner pointer @+0x88. The only
// instantiation in the binary is C_BuffInstance<C_Soul> (as in KCD1). The owner-type-specific
// virtuals it adds ([16..21] in the combined vtable) are KCD1 PRIORS -- KCD2 slot bodies not
// individually re-decompiled.

namespace wh::rpgmodule {

template<class TOwner>
class C_BuffInstance : public C_BuffInstanceBase {
public:
    // [16..21] KCD1 priors: UpdateSoul / Activate / ProcessModifiers / OnMaterialEffectStart /
    // OnMaterialEffectEnd / GetBuffClassFromDef (names + order KCD2-UNVERIFIED).
    virtual void UpdateOwner() = 0;             // [16]  (KCD1 "UpdateSoul")
    virtual void Activate() = 0;                // [17]
    virtual void ProcessModifiers() = 0;        // [18]
    virtual void OnMaterialEffectStart() {}     // [19]
    virtual void OnMaterialEffectEnd() {}       // [20]
    virtual uint32_t GetBuffClassFromDef() = 0; // [21]

    TOwner* m_pOwner;   // +0x88  owning soul (KCD1 name m_pSoul; set by AttachBuff;
                        //        CommitModifiers reads a1[17] -> soul sorted lists @C_Soul+0xA8)
};

}  // namespace wh::rpgmodule
