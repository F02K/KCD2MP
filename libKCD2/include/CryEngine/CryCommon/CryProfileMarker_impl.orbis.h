////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2013.
// -------------------------------------------------------------------------
//  File name:   CryProfileMarker_impl.orbis.h
//  Version:     v1.00
//  Created:     4/4/2013 by Charlie Cole
//  Compilers:   Visual Studio.NET
//  Description: Platform Profiling Marker Implementation for Orbis
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#if !defined(CRY_PROFILE_MARKER_IMPL_ORBIS_H_)
#define CRY_PROFILE_MARKER_IMPL_ORBIS_H_

#include <perf.h>

////////////////////////////////////////////////////////////////////////////
namespace CryProfile {
namespace detail {

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Set the Thread Name
	void SetThreadName( const char *pName ) 
	{ 
		//TODO
	}

	////////////////////////////////////////////////////////////////////////////
	// Set a one Profiling Event marker
	void SetProfilingEvent( const char *pName )
	{
		sceRazorCpuPushMarker(pName, SCE_RAZOR_COLOR_BLUE, SCE_RAZOR_MARKER_ENABLE_HUD);
		sceRazorCpuPopMarker();
	}

	////////////////////////////////////////////////////////////////////////////
	// Set the beginning of a profile marker
	void PushProfilingMarker( const char *pName )
	{
		sceRazorCpuPushMarker(pName, SCE_RAZOR_COLOR_RED, SCE_RAZOR_MARKER_ENABLE_HUD);
	}

	////////////////////////////////////////////////////////////////////////////
	// Set the end of a profiling marker
	void PopProfilingMarker()
	{
		sceRazorCpuPopMarker();
	}
} // namespace detail
} // namespace CryProfile

#endif // CRY_PROFILE_MARKER_IMPL_ORBIS_H_