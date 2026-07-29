#include "Scaleform/GFxMovieRoot.h"
#include "REL/Relocation.h"

// GFxMovieRoot load-queue surface -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
// Evidence + pipeline notes in the header banner.

void GFxMovieRoot::ProcessLoadQueue()
{
    // sub_181CAEF50: pops m_pLoadQueueHead, dispatches each entry on m_type
    // (&4/&8/&0x10 -> vars/XML/CSS handlers, else normal-SWF 0x181CAE530),
    // frees the entry, then culls finished m_pLoadProgressHead tasks. Same call
    // GFxMovieView::Advance (0x181CA4D90) makes on all 3 of its exit paths.
    using Fn = void(__fastcall*)(GFxMovieRoot*);
    static REL::Relocation<Fn> fn{ REL::ID(63) };
    fn(this);
}

void GFxMovieRoot::AddLoadQueueEntry(GFxLoadQueueEntry* entry)
{
    // sub_181CA3900: img:// / imgps:// URL sniff, dead state-23 immediate gate,
    // then tail-append to m_pLoadQueueHead + stamp entry->m_entryId with
    // ++m_lastLoadQueueEntryCnt.
    using Fn = void(__fastcall*)(GFxMovieRoot*, GFxLoadQueueEntry*);
    static REL::Relocation<Fn> fn{ REL::ID(62) };
    fn(this, entry);
}
