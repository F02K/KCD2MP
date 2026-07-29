#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "C_AnimationDatabaseGenerated.h"
#include "I_QueryableDatabase.h"

// -----------------------------------------------
// wh::animationmodule::C_ActionDatabaseGenerated<TData, TDBData, TQuery, TPreprocessor> -- KCD2 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_ActionDatabaseGenerated@US_CombatActionAttackData@..@US_CombatActionAttackDataDBData@
// ..@US_CombatActionAttackQueryData@..@V?$C_CombatActionPreprocessor@...@@animationmodule@wh@@ .
// This is the layer that makes an animation database QUERYABLE: it multiply-inherits
// I_QueryableDatabase<TQuery, TData> (MI subobject @+0xE8) and owns the registered QUERY-SELECTOR
// list that Enumerate walks.
//
// Layout evidence (attack DB ctor sub_1810DAF14, "C_ActionDatabaseGenerated" vtable phase):
//   +0xE8  I_QueryableDatabase<TQuery,TData> vtable (MI base; final vtable 0x183E50C90)
//   +0xF0  registered query-selector list -- a small-buffer vector {first@+0xF0, last@+0xF8,
//          capEnd@+0x100, + inline storage}. The ctor pushes:
//            * C_FullQuerySelector<TQuery, TData>            (8-byte, sub_1810DAF14)
//            * a 104-byte selector object   (sub_1810DB3BC)
//            * a 0x48 loader/registration record (built in the outermost ctor sub_1810DACF4)
//          Element = owned polymorphic selector pointer. Exact SBO capacity not pinned; modeled as
//          a raw {first,last,capEnd} + inline block. [tail container shape INFERRED]
//
// This layer is what routes S_AttackCandidateResultHolder::Enumerate(context, query) (sub_180687F3C)
// into the per-selector candidate walk (the std::function<void(unique_ptr<I_DatabaseSelector<TQuery,
// TData>> const&, S_QueryData&)>> referenced from S_AttackCandidateResultHolder.h).

namespace wh::animationmodule {

template <typename TQuery, typename TData> class I_DatabaseSelector;   // RTTI ?$I_DatabaseSelector@...@animationmodule@wh@@

template <typename TData, typename TDBData, typename TQuery, typename TPreprocessor>
class C_ActionDatabaseGenerated
    : public C_AnimationDatabaseGenerated<TData, TDBData, TPreprocessor>,
      public I_QueryableDatabase<TQuery, TData> {
public:
    // +0xF0 registered query-selector small-vector (see note above)
    std::vector<std::unique_ptr<I_DatabaseSelector<TQuery, TData>>> m_selectors;  // +0xF0 {first,last,end}; ctor zeroes all 3 (empty); elements = owned polymorphic query selectors
    uint64_t m_selectorSbo[6];    // +0x108  inline small-buffer storage [capacity INFERRED]
};

}  // namespace wh::animationmodule
