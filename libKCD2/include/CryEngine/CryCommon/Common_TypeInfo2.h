#ifndef COMMON_TYPEINFO2_H
#define COMMON_TYPEINFO2_H

#include "Cry_Vector3.h"

//@Scott
//I'd like to have this struct in CommonTypeInfo.h
//But there seems to be no way without getting a linker error
STRUCT_INFO_T_BEGIN(Vec3_tpl, typename, F)
	VAR_INFO(x)
	VAR_INFO(y)
	VAR_INFO(z)
STRUCT_INFO_T_END(Vec3_tpl, typename, F)

#endif