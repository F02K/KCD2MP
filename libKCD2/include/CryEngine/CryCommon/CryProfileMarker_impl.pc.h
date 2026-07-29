////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   CryProfileMarker_impl.pc.h
//  Version:     v1.00
//  Created:     11/12/2002 by Christopher Bolte.
//  Compilers:   Visual Studio.NET
//  Description: Platform Profiling Marker Implementation for PC using GPA marker
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////
#if !defined(CRY_PROFILE_MARKER_IMPL_PC_H_)
#define CRY_PROFILE_MARKER_IMPL_PC_H_


#if defined(CRY_PROFILE_MARKERS_USE_GPA)
#	include <ittnotify.h>
#	pragma comment(lib, "gpasdk_d.lib")
#endif


////////////////////////////////////////////////////////////////////////////
namespace CryProfile {
namespace detail {


#if defined(CRY_PROFILE_MARKERS_USE_GPA)
	////////////////////////////////////////////////////////////////////////////
	// util function to create a unique domain for GPA in CryEngine
	__itt_domain* GetDomain()
	{
		static __itt_domain* domain = __itt_domain_create("CryEngine");
		return domain;
	}

	////////////////////////////////////////////////////////////////////////////
	// util function to create a unique string handle for GPA
	__itt_string_handle* GetStringHandle( const char *pName )
	{
		static volatile int _lock = 0;
		static std::map< string, __itt_string_handle* > _handle_lookup;

		__itt_string_handle *pHandle = NULL;

		if (!pName)
		{
			return pHandle;
		}
		
		{ // first try a simple read lock to prevent thread contention
			CryReadLock( &_lock, false );
			std::map< string, __itt_string_handle* >::iterator it = _handle_lookup.find( CONST_TEMP_STRING(pName) );
			if(it !=  _handle_lookup.end())
			{
				pHandle = it->second;
				CryReleaseReadLock( &_lock );
				return pHandle;
			}
			CryReleaseReadLock( &_lock );
		}


		// nothing found, use write lock to add a new element safely
		{
			CryWriteLock( &_lock );
			// check again to make sure not two thread want to add the same handle
			std::map< string, __itt_string_handle* >::iterator it = _handle_lookup.find( CONST_TEMP_STRING(pName) );
			if(it !=  _handle_lookup.end())
				pHandle = it->second;
			else
			{
				pHandle = __itt_string_handle_create(pName);
				_handle_lookup.insert( std::make_pair( string(pName), pHandle ) );
			}
			CryReleaseWriteLock( &_lock );		
			return pHandle;
		}
	}
#endif // #if defined(CRY_PROFILE_MARKERS_USE_GPA)

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Set the Thread Name
	void SetThreadName( const char *pName ) 
	{
#if defined(CRY_PROFILE_MARKERS_USE_GPA)
		//DISABLED: As it breaks GPA Frame Analyzer __itt_thread_set_name( pName ); 
#endif
	}

	////////////////////////////////////////////////////////////////////////////
	// Set a one Profiling Event marker
	void SetProfilingEvent( const char *pName )
	{
#if defined(CRY_PROFILE_MARKERS_USE_GPA)
			//DISABLED: As it breaks GPA Frame Analyzer __itt_marker( GetDomain(),  __itt_null, GetStringHandle(pName), __itt_scope_track);
#endif
	}

	////////////////////////////////////////////////////////////////////////////
	// Set the beginning of a profile marker
	void PushProfilingMarker( const char *pName )
	{
#if defined(CRY_PROFILE_MARKERS_USE_GPA)
		__itt_domain* pDomain = GetDomain();
		__itt_string_handle* pNameHandle = GetStringHandle(pName);

		if (!pDomain || !pNameHandle)
		{
			return;
		}

		__itt_task_begin(pDomain, __itt_null, __itt_null, pNameHandle);
#endif
	}

	////////////////////////////////////////////////////////////////////////////
	// Set the end of a profiling marker
	void PopProfilingMarker()
	{
#if defined(CRY_PROFILE_MARKERS_USE_GPA)
		if (__itt_domain* pDomain = GetDomain())
		{
			__itt_task_end(pDomain);
		}
#endif
	}

} // namespace detail
} // namespace CryProfile

#endif // CRY_PROFILE_MARKER_IMPL_PC_H_
