#include "TaskInterface.h"
#include "Offsets/Offsets.h"
#include "crysystem/CCryAction.h"
#include "REL.h"

static std::mutex          s_taskLock;
static std::queue<KCSE::TaskFn>  s_tasks;

using PostUpdateFn = void(__fastcall*)(void* pThis, bool haveFocus, unsigned int updateFlags);
static PostUpdateFn g_origPostUpdate = nullptr;

void __fastcall Hooked_PostUpdate(void* pThis, bool haveFocus, unsigned int updateFlags)
{
    g_origPostUpdate(pThis, haveFocus, updateFlags);
    TaskInterface::ProcessTasks();
}

namespace TaskInterface {

void AddTask(KCSE::TaskFn task)
{
    std::lock_guard lock(s_taskLock);
    s_tasks.push(task);
}

void ProcessTasks()
{
    std::queue<KCSE::TaskFn> localQueue;
    {
        std::lock_guard lock(s_taskLock);
        localQueue.swap(s_tasks);
    }

    while (!localQueue.empty()) {
        auto task = localQueue.front();
        localQueue.pop();
        task();
    }
}


static constexpr std::size_t kSlot_PostUpdate = 11;

void InstallHook()
{
    auto* pFramework = CCryAction::GetInstance();
    g_origPostUpdate = reinterpret_cast<PostUpdateFn>(
        REL::Relocation<>{ *reinterpret_cast<std::uintptr_t*>(pFramework) }.write_vfunc(kSlot_PostUpdate, Hooked_PostUpdate));
}

void RemoveHook()
{
    auto* pFramework = CCryAction::GetInstance();
    if (pFramework && g_origPostUpdate)
        REL::Relocation<>{ *reinterpret_cast<std::uintptr_t*>(pFramework) }.write_vfunc(kSlot_PostUpdate, g_origPostUpdate);
}

}  // namespace TaskInterface
