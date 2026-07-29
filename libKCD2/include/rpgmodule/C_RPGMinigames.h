#pragma once
#include <cstdint>
#include <cstddef>
#include <random>
#include <unordered_map>
#include <vector>
#include "I_RPGMinigames.h"

// -----------------------------------------------
// wh::rpgmodule::C_RPGMinigames -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1438.
// -----------------------------------------------
// RTTI .?AVC_RPGMinigames@rpgmodule@wh@@ (TD 0x184D5FBF8; bases: I_RPGMinigames @+0x00).
// ctor sub_1817B45CC (source of the full layout below), vtable 0x183C16A90 (83 slots).
// Owned by C_RPGModule (+0x98, m_pMinigames); the grade path reaches it as
// *(S_GameContext+0x130)+0x98.  Save/load sub_1817A0A94: chunk 0x1FA1 = m_learnedRecipes,
// chunk 0x130F = the two document maps.  Slot semantics: I_RPGMinigames.h and
// analysis/alchemy_re/balancing_harness_research.md "Brewed-once gate research".

namespace wh::rpgmodule {

class C_RPGModule;

class C_RPGMinigames : public I_RPGMinigames {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGMinigames;

    // One learned-recipe record: MarkRecipeLearned lower_bound-inserts {id, 0x1FFFFFFF} /
    // ORs 0x1FFFFFFF into an existing entry (sub_181326B0C); the Is* queries bit-test.
    struct S_LearnedRecipe {
        int32_t  m_recipeId;   // +0x00
        uint32_t _pad04;       // +0x04
        uint64_t m_stepBits;   // +0x08  bit i = recipe step i known; no partial writer exists
    };
    static_assert(sizeof(S_LearnedRecipe) == 0x10, "learned-recipe entry stride is 0x10");

    // The two document maps hash by XOR-folding the guid's two qwords (readers
    // sub_18119F47C / sub_182D35C08; layout-neutral -- declared for type fidelity only).
    struct S_GuidHashXorFold {
        uint64_t operator()(const CryGUID& guid) const;   // declared, never defined
    };

    C_RPGModule* m_pModule;                        // +0x08  back-ptr (C_RPGModule ctor wires it)
    std::vector<S_LearnedRecipe> m_learnedRecipes; // +0x10  sorted by id -- recipe KNOWLEDGE
                                                   //        (brew-discovery AND document reading)
    std::unordered_map<CryGUID, float, S_GuidHashXorFold>
        m_docFloatMarks;                           // +0x28  recipe-doc guid -> float; read by
                                                   //        vf+0x120 (>0 short-circuits "known");
                                                   //        runtime writer NOT located [UNVERIFIED
                                                   //        float meaning]
    std::unordered_map<CryGUID, bool, S_GuidHashXorFold>
        m_docsStudied;                             // +0x68  document guid -> studied; the once-only
                                                   //        study-XP guard (sub_182CFB2AC marker
                                                   //        sub_182CFD12C); vf+0x120's set branch
    void*        _unkA8;                           // +0xA8  ctor-zeroed, semantics unknown
    std::mt19937 m_rng;                            // +0xB0  std::_Random_device-seeded in the ctor
                                                   //        (sub_180A61118; idx dword forced -1)
};
static_assert(sizeof(std::unordered_map<CryGUID, float, C_RPGMinigames::S_GuidHashXorFold>) == 0x40,
              "MSVC unordered_map header must be 0x40 to match the ctor field walk");
static_assert(sizeof(std::mt19937) == 0x1388, "MSVC mt19937 must span +0xB0..+0x1437");
static_assert(sizeof(C_RPGMinigames) == 0x1438, "C_RPGMinigames must be 0x1438");

}  // namespace wh::rpgmodule
