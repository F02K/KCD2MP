#pragma once

// -----------------------------------------------
// wh::dialogmodule::C_DialogueTimingPreprocessor -- row preprocessor tag
// (KCD2 1.5.6, kd7u).  NOT RE'd.
// -----------------------------------------------
// Third template argument of the C_AnimationDatabaseGenerated instantiation behind
// C_DialogueTimingDatabaseGenerated (post-load animation-tag resolution over each
// S_DialogueTiming row). Contents UNVERIFIED -- declared empty so the template
// instantiates; do not use for sizeof math.

namespace wh::dialogmodule {

struct C_DialogueTimingPreprocessor {
};

}  // namespace wh::dialogmodule
