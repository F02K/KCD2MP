#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_VariableIndex -- AI variable symbol-table interface: maps a
// uint16 name-hash -> variable, with a parent-index chain (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x10 (vptr + m_parent; NOT a pure interface -- slots [6][7][9][17]
// are concrete and operate on +0x08).
// -----------------------------------------------
// RTTI TD rva 0x4EFBF08; vtable 0x183A3FAB0, 20 slots (most _purecall in the base;
// concrete in C_VariableIndex).  Slot roles from the C_VariableIndex override bodies
// (g4b_dump_varindex_iface_decomp.txt); signatures inferred from those bodies [U
// exact prototypes].  Key hash fn sub_1805B8998 (name -> uint16).

namespace wh::xgenaimodule {

class C_Variable;

class I_VariableIndex {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_VariableIndex;
    virtual ~I_VariableIndex();                                    // [0]  base dtor body sub_18345F140
    virtual void* GetVarByKeyHash(uint16_t hash, bool recurseToParent) = 0;  // [1]  impl sub_1805C6DFC: binsearch +0x10 map, miss -> parent
    virtual void* GetVarByName(const char* name, bool recurse) = 0;          // [2]  impl sub_1805C71F0: hash(sub_1805B8998) -> [1]
    virtual void  Insert(void* entry) = 0;                         // [3]  impl sub_1805B889C [U signature]
    virtual void* CreateVariable(void* desc) = 0;                  // [4]  impl sub_1805B33EC -> variable factory sub_1805B34A8 [U signature]
    virtual void  RemoveVariable(const char* name) = 0;            // [5]  impl sub_1809FDC74 (binsearch + memmove)
    virtual void  SetParent(I_VariableIndex* parent);              // [6]  CONCRETE sub_1805B66DC: if(parent!=m_parent) m_parent=parent
    virtual void  ClearParent();                                   // [7]  CONCRETE sub_181A75F80: m_parent=0
    virtual void  CollectAll(void* outVec, bool recurse) = 0;      // [8]  impl sub_183460C88
    virtual I_VariableIndex* GetParent();                          // [9]  CONCRETE sub_180947210: return m_parent
    virtual void  ForEachChild() = 0;                              // [10] impl sub_18041B184 (child->vtbl+232) [U signature]
    virtual void  _vf11() = 0;                                     // [11] [U role]
    virtual void  Serialize(void* stream) = 0;                     // [12] [U signature]
    virtual void  Deserialize(void* stream) = 0;                   // [13] [U signature]
    virtual void  NetSerialize(void* stream) = 0;                  // [14] stream tag 6401 [U signature]
    virtual void  ParseFromString(const char* str) = 0;            // [15] [U signature]
    virtual void  GetIdentity(void* outIdentity) = 0;              // [16] builds S_VariableIdentity [U signature]
    virtual void* GetRootContext();                                // [17] CONCRETE: parent ? parent+0x88 : qword_185338030 [U role]
    virtual const char* GetVariableName(uint64_t handle);          // [18] base = empty string; C_VariableIndex ovr sub_183460BB0
    virtual bool  _vf19();                                         // [19] base = return false (sub_180838AE0) [U role]

    I_VariableIndex* m_parent;   // +0x08  parent index chain (read/written by [6][7][9])
};
static_assert(sizeof(I_VariableIndex) == 0x10, "I_VariableIndex must be 0x10 (vptr + parent)");

}  // namespace wh::xgenaimodule
