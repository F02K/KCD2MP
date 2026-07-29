#include "Offsets/Offsets.h"

uintptr_t Offsets::GetBase() {
    return REL::Module::get().base();
}
