#ifndef IAntiCheatManager_h
#define IAntiCheatManager_h


#if !defined(_RELEASE) && !PROFILE_PERFORMANCE_NET_COMPATIBLE
#define DEV_CHEAT_HANDLING
#endif

#if _MSC_VER > 1000
#pragma once
#endif

typedef int32 TCheatType;
typedef int32 TCheatAssetGroup;
typedef int32 TAntiCheatVarIdx;

UNIQUE_IFACE struct IAntiCheatManager
{
	// <interfuscator:shuffle>
  virtual ~IAntiCheatManager(){}
  virtual int RetrieveHashMethod() = 0;
  virtual int GetAssetGroupCount() = 0;
  virtual XmlNodeRef GetFileProbeConfiguration() = 0;

	virtual void FlagActivity(TCheatType type, uint16 channelId, const char * message) = 0;
	virtual void FlagActivity(TCheatType type, uint16 channelId) = 0;
	virtual void FlagActivity(TCheatType type, uint16 channelId, float param1) = 0;
	virtual void FlagActivity(TCheatType type, uint16 channelId, float param1, float param2) = 0;	// Please update MAX_FLAG_ACTIVITY_PARAMS if you add more parameters
	virtual void FlagActivity(TCheatType type, uint16 channelId, float param1, const char * message) = 0;
	virtual void FlagActivity(TCheatType type, uint16 channelId, float param1, float param2, const char * message) = 0;
	

	virtual TCheatType FindCheatType(const char* name) = 0;
  virtual TCheatAssetGroup FindAssetTypeByExtension(const char * ext) = 0;
  virtual TCheatAssetGroup FindAssetTypeByWeight() = 0;
	virtual void InitSession() = 0;
  virtual void OnSessionEnd() = 0;
	// </interfuscator:shuffle>
};

#endif
