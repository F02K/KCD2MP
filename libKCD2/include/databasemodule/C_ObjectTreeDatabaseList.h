#pragma once
#include <cstdint>
#include "C_ObjectDatabaseBase.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::databasemodule::C_ObjectTreeDatabaseList<TObject> -- MIRROR of the
// binary's C_ObjectTreeDatabase<TObject, std::list> instantiation (KCD2
// WHGame.dll 1.5.6, kd7u).  Subobject size 0x70 (list variant is 8 smaller
// than the vector variant's 0x78).
// -----------------------------------------------
// NOT a distinct binary class: the real template is
// C_ObjectTreeDatabase<Row, Container>; this repo's C_ObjectTreeDatabase.h
// mirror collapsed the container parameter to the std::vector shape, so the
// std::list instantiation (sole user: xgenaimodule::C_SmartEntityDatabase,
// base ctor sub_181977C80) gets this sibling mirror.  Row store @+0x30 is a
// std::list (0x10: _Myhead sentinel node = 0x10 + sizeof(TObject), _Mysize);
// the tree strings/flags shift down 8 vs the vector variant.

namespace wh::databasemodule {

template <typename TObject>
class C_ObjectTreeDatabaseList : public C_ObjectDatabaseBase {
public:
    std::list<TObject> m_rows;      // +0x30  MSVC std::list {_Myhead +0x30, _Mysize +0x38}; ctor sub_181977C80 allocs 216B sentinel node (_Next=_Prev=self), node = 0x10 + sizeof(TObject)
    CryStringT<char> m_namespace;   // +0x40
    CryStringT<char> m_name;        // +0x48
    CryStringT<char> m_group;       // +0x50
    uint8_t          m_loaded;      // +0x58  vtable slot [6] returns it
    uint8_t          _pad59[7];     // +0x59
    std::set<void*>  m_registry;    // +0x60  MSVC std::set {_Myhead +0x60, _Mysize +0x68}; ctor sub_1803F7FAC empty-set sentinel (40-byte _Tree node, L=P=R=self, _Color=1 _Isnil=1) -> 8-byte value (UNRESOLVED)
};
static_assert(sizeof(C_ObjectTreeDatabaseList<int>) == 0x70,
              "list-variant tree DB subobject must be 0x70");

}  // namespace wh::databasemodule
