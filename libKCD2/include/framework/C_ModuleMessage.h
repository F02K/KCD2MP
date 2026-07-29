#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::C_ModuleMessage -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// Root polymorphic base of every inter-module message (no parent).
// RTTI .?AVC_ModuleMessage@framework@wh@@  primary vtable 0x183AED198 (5 slots)
// scalar-deleting dtor 0x18257DCDC (frees 24).
//
// [MODERATE vs KCD1] IsExtended threshold moved (m_type > 0x3F -> m_type > 0x46) and the message-type
// enum was renumbered. Only a few KCD2 type values were re-derived (0x32, 0x38, 0x3A, 0x4C observed
// on base messages); SaveGameRequest type is still 4.

namespace wh::framework {

class C_ModuleMessage {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ModuleMessage;
    virtual ~C_ModuleMessage() = default;                      // [0]  0x18257DCDC
    virtual void OnProcessed() {}                              // [1]  nullsub in base
    virtual void _vf2() {}                                     // [2]  nullsub (distinct slot; role unresolved)
    virtual void _vf3() {}                                     // [3]  nullsub (distinct slot; role unresolved)
    virtual bool IsExtended() const { return m_type > 0x46; }  // [4]  0x18047A02C

    int32_t m_type;       // +0x08  message-type enum, PARTIAL: observed {4=SaveGameRequest, 0x32, 0x38, 0x3A, 0x4C}; full set needs a sweep of all derived ctors (>=0x47 = extended)
    int32_t m_param;      // +0x0C  (inline builders set -1)
    uint8_t m_flag;       // +0x10  (inline builders set 0)
    uint8_t _pad11[7];    // +0x11
};
static_assert(sizeof(C_ModuleMessage) == 0x18);

// Save-request save-kind. [UNVERIFIED semantics] recovered byte values from the builder/validator;
// symbolic member names are inferred from each branch's behaviour, not from a reflected enum.
enum class E_SaveType : uint8_t {
    Generic         = 0,
    Manual          = 1,
    Save            = 2,
    Special         = 3,   // skips the "no pending chunk" ctor guard
    SlotChecked     = 5,   // free-slot query; requires state byte == 9
    LevelTransition = 6,   // level-transition / checkpoint autosave
    RestBed         = 8,   // rest / bed autosave
};

// Save-game request (the message the game builds inline via sub_180FC31A0 then dispatches).
// vtable 0x183E19478 (same 5-slot shape as the base); m_type == 4.
class C_ModuleMessageSaveGameRequest : public C_ModuleMessage {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ModuleMessageSaveGameRequest;
    E_SaveType saveType;   // +0x18  builder arg
    uint8_t    _pad19[7];  // +0x19  (heap size not literal-confirmed; layout rounds to 0x20)
};
static_assert(sizeof(C_ModuleMessageSaveGameRequest) == 0x20);

}  // namespace wh::framework
