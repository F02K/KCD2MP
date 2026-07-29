////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   CryProfileMarker.h
//  Version:     v1.00
//  Created:     11/12/2002 by Christopher Bolte.
//  Compilers:   Visual Studio.NET
//  Description: Platform Independent Profiling Marker
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#if !defined(CRY_PROFILE_MARKER_H_)
#define CRY_PROFILE_MARKER_H_

#include <IJobManager.h>
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// forward declaration for Implementation functions, these will be different 
// per platform/implementation
// TODO: Move the implementations later to it's own Platform Specific Library
namespace CryProfile {
namespace detail {

	void SetThreadName( const char *pName );
	void SetProfilingEvent( const char *pName );
	void PushProfilingMarker( const char *pName );
	void PopProfilingMarker();

} // namespace detail
} // namespace CryProfile


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Public Interface for use of Profile Marker
namespace CryProfile
{
	// Tell the Profiling tool the name of the calling Thread
	inline void SetThreadName( const char *pName ) { CryProfile::detail::SetThreadName(pName); }

	// Set a Profiling Event, this represents a single time, no duration event in the profiling Tool
	inline void SetProfilingEvent( const char *pName ) { CryProfile::detail::SetProfilingEvent(pName); }

	// direct push/pop for profiling markers
	inline void PushProfilingMarker( const char *pName ) { gEnv->pJobManager->PushProfilingMarker(pName); CryProfile::detail::PushProfilingMarker(pName); }
	inline void PopProfilingMarker() { gEnv->pJobManager->PopProfilingMarker();CryProfile::detail::PopProfilingMarker(); }

	// class to define a profile scope, to represent time events in profile tools
	class CScopedProfileMarker
	{
	public:
		inline CScopedProfileMarker( const char *pName ) { CryProfile::PushProfilingMarker(pName); }
		inline ~CScopedProfileMarker() { CryProfile::PopProfilingMarker(); }
	};

} // namespace CryProfile

// Util Macro to create scoped profiling markers
#define CRYPROFILE_CONCAT_(a,b) a ## b
#define CRYPROFILE_CONCAT(a,b) CRYPROFILE_CONCAT_(a,b)
#define CRYPROFILE_SCOPE_PROFILE_MARKER( name ) CryProfile::CScopedProfileMarker CRYPROFILE_CONCAT( __scopedProfileMarker, __LINE__ )(name);

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// For Release Builds, provide emtpy inline variants of all profile functions
#if !defined(ENABLE_PROFILING_CODE) || defined(SANDBOX_API)
inline void CryProfile::detail::SetThreadName( const char *pName ){}
inline void CryProfile::detail::SetProfilingEvent( const char *pName ){}
inline void CryProfile::detail::PushProfilingMarker( const char *pName ){}
inline void CryProfile::detail::PopProfilingMarker(){}
//For static builds
#elif defined(_LIB)
inline void CryProfile::detail::SetThreadName( const char *pName ){}
inline void CryProfile::detail::SetProfilingEvent( const char *pName ){}
inline void CryProfile::detail::PushProfilingMarker( const char *pName ){}
inline void CryProfile::detail::PopProfilingMarker(){}
#endif

#endif // CRY_PROFILE_MARKER_H_
