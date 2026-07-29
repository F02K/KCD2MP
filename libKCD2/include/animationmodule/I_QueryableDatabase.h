#pragma once

// -----------------------------------------------
// wh::animationmodule::I_QueryableDatabase<TQuery, TObject> -- KCD2 1.5.6 (kd7u). Interface (0x08).
// -----------------------------------------------
// RTTI .?AV?$I_QueryableDatabase@U<TQuery>@..@US<TObject>@..@@animationmodule@wh@@ -- MI base of the
// action databases at mdisp 0xE8. Abstract "query the database with a TQuery, get TObject
// candidates" facet, introduced by C_ActionDatabaseGenerated. It is what the attack-candidate
// enumerator (S_AttackCandidateResultHolder::Enumerate == sub_180687F3C) talks to when it walks the
// global moveset: the DB owns a list of registered query SELECTORS (I_DatabaseSelector /
// C_FullQuerySelector<TQuery,TObject>) that yield candidates matching a query.
//
// Concrete subobject vtable inside C_CombatActionAttackDatabase (@+0xE8, VA 0x183E50C90) = 3 slots:
//   [0] 0x1809CB23C  Query/Enumerate(query, out)   [name inferred]
//   [1] 0x181A7E500  GetSelectorCount/accessor      [name inferred]
//   [2] 0x1809CB6A8  Query variant / ForEachMatch   [name inferred]
// (slot [3] is already the +0x98 subobject COL 0x18422CE00 -> boundary). Names behaviour-inferred.

namespace wh::animationmodule {

template <typename TQuery, typename TObject>
class I_QueryableDatabase {
public:
    virtual ~I_QueryableDatabase() = default;
    virtual void Query(const TQuery& query, void* out) = 0;   // [0] 0x1809CB23C  (name inferred)
    virtual int  GetSelectorInfo() = 0;                       // [1] 0x181A7E500  (name inferred)
    virtual void QueryVariant(const TQuery& query, void* out) = 0; // [2] 0x1809CB6A8 (name inferred)
};

}  // namespace wh::animationmodule
