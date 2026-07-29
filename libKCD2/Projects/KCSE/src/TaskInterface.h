#pragma once

#include "KCSE/KCSEAPI.h"
#include <queue>
#include <mutex>

namespace TaskInterface {

void AddTask(KCSE::TaskFn task);
void ProcessTasks();
void InstallHook();
void RemoveHook();

}  // namespace TaskInterface
