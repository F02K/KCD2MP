#ifndef _CRY_COMMON_CRY_RESOURCECOLLECTOR_INTERFACE_HDR_
#define _CRY_COMMON_CRY_RESOURCECOLLECTOR_INTERFACE_HDR_

#pragma once

// used to collect the assets needed for streaming and to gather statistics
struct IResourceCollector
{
	// <interfuscator:shuffle>
	// Arguments:
	//   dwMemSize 0xffffffff if size is unknown
	// Returns:
	//   true=new resource was added, false=resource was already registered
	virtual bool AddResource( const char *szFileName, const uint32 dwMemSize ) = 0;

	// Arguments:
	//   szFileName - needs to be registered before with AddResource()
	//   pInstance - must not be 0
	virtual void AddInstance( const char *szFileName, void *pInstance ) = 0;
	//
	// Arguments:
	//   szFileName - needs to be registered before with AddResource()
	virtual void OpenDependencies( const char *szFileName ) = 0;
	//
	virtual void CloseDependencies() = 0;

	// Resets the internal data structure for the resource collector.
	virtual void Reset() = 0;
	// </interfuscator:shuffle>
protected:
	virtual ~IResourceCollector() {}
};


class NullResCollector : public IResourceCollector
{
public:
	virtual bool AddResource(const char* szFileName, const uint32 dwMemSize) { return true; }
	virtual void AddInstance( const char* szFileName, void* pInstance) {}
	virtual void OpenDependencies(const char* szFileName) {}
	virtual void CloseDependencies() {}
	virtual void Reset() {}

	virtual ~NullResCollector() {}
};


#endif // _CRY_COMMON_CRY_RESOURCECOLLECTOR_INTERFACE_HDR_


