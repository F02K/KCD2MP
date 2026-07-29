#include "crysystem/CCryAction.h"
#include "Offsets/Offsets.h"

CCryAction* CCryAction::GetInstance()
{
    static REL::Relocation<CCryAction**> p{ REL::ID(2356) };  // qword_18549D328
    return *p;
}
