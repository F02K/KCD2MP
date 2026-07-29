// Compile-time assert.
// Syntax: COMPILE_TIME_ASSERT(BooleanExpression);
// 
// Inspired by the Boost library's BOOST_STATIC_ASSERT(),
// see http://www.boost.org/doc/libs/1_49_0/doc/html/boost_staticassert/how.html
// or http://www.boost.org/libs/static_assert

#ifndef __CompileTimeAssert_h__
#define __CompileTimeAssert_h__

#pragma once

#if defined(__cplusplus)

template <bool b>
struct COMPILE_TIME_ASSERT_FAIL;

template <>
struct COMPILE_TIME_ASSERT_FAIL<true>
{
};

template <int i>
struct COMPILE_TIME_ASSERT_TEST
{
	enum { dummy = i };
};

#define COMPILE_TIME_ASSERT_BUILD_NAME2(x, y) x##y
#define COMPILE_TIME_ASSERT_BUILD_NAME1(x, y) COMPILE_TIME_ASSERT_BUILD_NAME2(x, y)
#define COMPILE_TIME_ASSERT_BUILD_NAME(x, y) COMPILE_TIME_ASSERT_BUILD_NAME1(x, y)

#ifndef __RECODE__
	#define COMPILE_TIME_ASSERT(expr) \
		typedef COMPILE_TIME_ASSERT_TEST<sizeof(COMPILE_TIME_ASSERT_FAIL<(bool)(expr)>)> \
		COMPILE_TIME_ASSERT_BUILD_NAME(compile_time_assert_test_, __LINE__)
	// note: for MS Visual Studio we could use __COUNTER__ instead of __LINE__
#else
	#define COMPILE_TIME_ASSERT(expr)
#endif	// __RECODE__

#else

#define COMPILE_TIME_ASSERT(expr)

#endif

#endif
