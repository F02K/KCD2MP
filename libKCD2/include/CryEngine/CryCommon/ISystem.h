#include DEVIRTUALIZE_HEADER_FIX(ISystem.h)

#ifndef _CRY_SYSTEM_H_
#define _CRY_SYSTEM_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef CRYSYSTEM_EXPORTS
	#define CRYSYSTEM_API DLL_EXPORT
#else
	#define CRYSYSTEM_API DLL_IMPORT
#endif

#include <platform.h> // Needed for LARGE_INTEGER (for consoles).

#include "CryAssert.h"
#include "CompileTimeAssert.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////
#include <IXml.h> // <> required for Interfuscator
#include <IValidator.h> // <> required for Interfuscator
#include <ILog.h> // <> required for Interfuscator
#include "CryVersion.h"
#include "smartptr.h"
#include <IMemory.h> // <> required for Interfuscator
#include <ISystemScheduler.h> // <> required for Interfuscator





struct ISystem;
struct ILog;
struct IProfileLogSystem;
struct IEntitySystem;
struct IEntity;
struct ICryPak;
struct IKeyboard;
struct IMouse;
struct IConsole;
struct IRemoteConsole;
struct IInput;
struct IRenderer;
struct IConsole;
namespace Telemetry { struct ITelemetrySystem; }
struct IProcess;
struct I3DEngine;
struct ITimer;
struct IGame;
struct IGameStartup;
struct IScriptSystem;
struct IAISystem;
struct IFlash;
struct INetwork;
struct ICryLobby;
struct ICryFont;
struct IMovieSystem;
struct IPhysicalWorld;
struct IMemoryManager;
struct ISoundSystem;
struct IMusicSystem;
struct IFrameProfileSystem;
struct IStatoscope;
class	 IDiskProfiler;
struct FrameProfiler;
struct IStreamEngine;
struct ICharacterManager;
struct SFileVersion;
struct IDataProbe;
struct INameTable;
struct IBudgetingSystem;
struct IFlowSystem;
struct IDialogSystem;
struct IMaterialEffects;
struct IParticleManager;
class IOpticsManager;
struct IHardwareMouse;
struct IAnimationGraphSystem;
struct IFlashPlayer;
struct IFlashPlayerBootStrapper;
struct IFlashLoadMovieHandler;
struct IHMDDevice;
class  ICrySizer;
struct ITestSystem;
class  IXMLBinarySerializer;
struct IReadWriteXMLSink;
struct IThreadTaskManager;
struct IResourceManager;
struct ITextModeConsole;
struct IAVI_Reader;
class Crc32Gen; 
class CPNoise3;
struct IFileChangeMonitor;
struct IVisualLog;
struct ILocalizationManager;
struct ICryFactoryRegistry;
struct ICodeCheckpointMgr;
struct ISoftCodeMgr;
struct IZLibCompressor;
struct IOutputPrintSink;
struct IPhysicsDebugRenderer;
struct IGameStream;
struct IOverloadSceneManager;
struct IFlashUI;
struct IServiceNetwork;
struct IRemoteCommandManager;

namespace LiveCreate
{
	struct IManager;
	struct IHost;
}

struct ILocalMemoryUsage;

typedef void* WIN_HWND;

class CCamera;
struct CLoadingTimeProfiler;

class ICmdLine;
class CFrameProfilerSection;

struct INotificationNetwork;
struct IPlatformOS;
struct ICryPerfHUD;

namespace JobManager { struct IJobManager; }

#define PROC_MENU		1
#define PROC_3DENGINE	2

// Summary:
//	 IDs for script userdata typing. 
// Remarks:
//	 Maybe they should be moved into the game.dll .
//##@{
#define USER_DATA_SOUND			1
#define USER_DATA_TEXTURE		2
#define USER_DATA_OBJECT		3
#define USER_DATA_LIGHT			4
#define USER_DATA_BONEHANDLER	5
#define USER_DATA_POINTER		6
//##@}

enum ESystemUpdateFlags
{
	ESYSUPDATE_IGNORE_AI			= 0x0001,
	ESYSUPDATE_IGNORE_PHYSICS = 0x0002,
	// Summary:
	//	 Special update mode for editor.
	ESYSUPDATE_EDITOR					=	0x0004,
	ESYSUPDATE_MULTIPLAYER		= 0x0008,
	ESYSUPDATE_EDITOR_AI_PHYSICS = 0x0010,
	ESYSUPDATE_EDITOR_ONLY = 0x0020 
};

// Description:
//	 Configuration specification, depends on user selected machine specification.
enum ESystemConfigSpec
{
	CONFIG_CUSTOM        = 0, // should always be first
	CONFIG_LOW_SPEC      = 1,
	CONFIG_MEDIUM_SPEC   = 2,
	CONFIG_HIGH_SPEC     = 3,
	CONFIG_VERYHIGH_SPEC = 4,

	CONFIG_X360 = 5,
	CONFIG_PS3 = 6,
	CONFIG_DURANGO = 7,
	CONFIG_ORBIS = 8,

	// Summary:
	//	 Specialized detail config setting.
	CONFIG_DETAIL_SPEC  = 9,
	
	
	END_CONFIG_SPEC_ENUM, // MUST BE LSAT VALUE. USED FOR ERROR CHECKING.
};

enum ESubsystem
{
	ESubsys_3DEngine = 0,
	ESubsys_AI = 1,
	ESubsys_Physics = 2,
	ESubsys_Renderer = 3,
	ESubsys_Script = 4
};

// Summary:
//	 Collates cycles taken per update.
struct sUpdateTimes
{
	uint32 PhysYields; 
	uint64 SysUpdateTime; 
	uint64 PhysStepTime; 
	uint64 RenderTime; 
	//extended yimes info
	uint64 physWaitTime;
	uint64 streamingWaitTime;
	uint64 animationWaitTime;
}; 

#ifdef PS3
	#define PS3_MEMORY_LIMIT (256*1024*1024)
	#define PS3_DEV_KIT_EXTENDED_MEMORY (256*1024*1024)
	#define PS3_MEMORY_BUDGET_CONSOLE_MODE (213*1024*1024)
	#define PS3_MEMORY_BUDGET_TOOL_MODE    (407*1024*1024)
	#define PS3_RSX_MEMORY_BUDGET          (249*1024*1024)
#endif

enum ESystemGlobalState	
{
	ESYSTEM_GLOBAL_STATE_UNKNOWN,
	ESYSTEM_GLOBAL_STATE_INIT,
	ESYSTEM_GLOBAL_STATE_RUNNING,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_PREPARE,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_MATERIALS,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_OBJECTS,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_CHARACTERS,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_STATIC_WORLD,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_ENTITIES,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_PRECACHE,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_START_TEXTURES,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_END,
	ESYSTEM_GLOBAL_STATE_LEVEL_LOAD_COMPLETE
};

// Summary:
//	 System wide events.
enum ESystemEvent
{
	// Description:
	// Seeds all random number generators to the same seed number, WParam will hold seed value.
	//##@{
	ESYSTEM_EVENT_RANDOM_SEED = 1,
	ESYSTEM_EVENT_RANDOM_ENABLE,
	ESYSTEM_EVENT_RANDOM_DISABLE,
	//##@}

	// Description:
	//	 Changes to main window focus.
	//	 wparam is not 0 is focused, 0 if not focused
	ESYSTEM_EVENT_CHANGE_FOCUS = 10,

	// Description:
	//	 Moves of the main window.
	//	 wparam=x, lparam=y
	ESYSTEM_EVENT_MOVE = 11,

	// Description:
	//	 Resizes of the main window.
	//	 wparam=width, lparam=height
	ESYSTEM_EVENT_RESIZE = 12,

	// Description:
	//	 Activation of the main window.
	//	 wparam=1/0, 1=active 0=inactive
	ESYSTEM_EVENT_ACTIVATE = 13,

	// Description:
	//	 Main window position changed.
	ESYSTEM_EVENT_POS_CHANGED = 14,

	// Description:
	//	 Main window style changed.
	ESYSTEM_EVENT_STYLE_CHANGED = 15,

	// Description:
	//	 Sent before the loading movie is begun
	ESYSTEM_EVENT_LEVEL_LOAD_START_PRELOADINGSCREEN,

	// Description:
	//	 Sent before the loading last save
	ESYSTEM_EVENT_LEVEL_LOAD_RESUME_GAME,

	// Description:
	//	 Sent before starting level, before game rules initialization and before ESYSTEM_EVENT_LEVEL_LOAD_START event
	//	 Used mostly for level loading profiling
	ESYSTEM_EVENT_LEVEL_LOAD_PREPARE,

	// Description:
	//	 Sent to start the active loading screen rendering.
	ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN,

	// Description:
	//	 Sent when loading screen is active
	ESYSTEM_EVENT_LEVEL_LOAD_LOADINGSCREEN_ACTIVE,

  // Description:
  //	 Sent before starting loading a new level.
  //	 Used for a more efficient resource management.
  ESYSTEM_EVENT_LEVEL_LOAD_START,

	// Description:
	//	 Sent after loading a level finished.
	//	 Used for a more efficient resource management.
	ESYSTEM_EVENT_LEVEL_LOAD_END,

	// Description:
	//	 Sent after trying to load a level failed.
	//	 Used for resetting the front end.
	ESYSTEM_EVENT_LEVEL_LOAD_ERROR,

	// Description:
	//	 Sent in case the level was requested to load, but it's not ready
	//	 Used in streaming install scenario for notifying the front end.
	ESYSTEM_EVENT_LEVEL_NOT_READY,

	// Description:
	//	 Sent after precaching of the streaming system has been done
	ESYSTEM_EVENT_LEVEL_PRECACHE_START,

	// Description:
	//	 Sent before object/texture precache stream requests are submitted
	ESYSTEM_EVENT_LEVEL_PRECACHE_FIRST_FRAME,

	// Description:
	//	Sent when level loading is completely finished with no more onscreen 
	//	movie or info rendering, and when actual gameplay can start
	ESYSTEM_EVENT_LEVEL_GAMEPLAY_START,

	// Level is unloading.
	ESYSTEM_EVENT_LEVEL_UNLOAD,

	// Summary:
	//	 Sent after level have been unloaded. For cleanup code.
	ESYSTEM_EVENT_LEVEL_POST_UNLOAD,

	// Summary:
	//	 Called when the game framework has been initialized.
	ESYSTEM_EVENT_GAME_POST_INIT,

	// Summary:
	//	 Called when the game framework has been initialized, not loading should happen in this event.
	ESYSTEM_EVENT_GAME_POST_INIT_DONE,

	// Summary:
	//	 Sent when system is shutting down.
	ESYSTEM_EVENT_SHUTDOWN,

	// Summary:
	//	 When keyboard layout changed.
	ESYSTEM_EVENT_LANGUAGE_CHANGE,

	// Description:
	//	 Toggled fullscreen.
	//	 wparam is 1 means we switched to fullscreen, 0 if for windowed
	ESYSTEM_EVENT_TOGGLE_FULLSCREEN,
	ESYSTEM_EVENT_SHARE_SHADER_COMBINATIONS,

	// Summary:
	//	 Start 3D post rendering
	ESYSTEM_EVENT_3D_POST_RENDERING_START,

	// Summary:
	//	 End 3D post rendering
	ESYSTEM_EVENT_3D_POST_RENDERING_END,

	// Summary:
	//	 Called before switching to level memory heap
	ESYSTEM_EVENT_SWITCHING_TO_LEVEL_HEAP,

	// Summary:
	//	 Called after switching to level memory heap
	ESYSTEM_EVENT_SWITCHED_TO_LEVEL_HEAP,

	// Summary:
	//	 Called before switching to global memory heap
	ESYSTEM_EVENT_SWITCHING_TO_GLOBAL_HEAP,

	// Summary:
	//	 Called after switching to global memory heap
	ESYSTEM_EVENT_SWITCHED_TO_GLOBAL_HEAP,

	// Description:
	//	 Sent after precaching of the streaming system has been done
	ESYSTEM_EVENT_LEVEL_PRECACHE_END,

	// Description:
	//		Sent when game mode switch begins
	ESYSTEM_EVENT_GAME_MODE_SWITCH_START,

	// Description:
	//		Sent when game mode switch ends
	ESYSTEM_EVENT_GAME_MODE_SWITCH_END,
	
	// Description:
	//	 Video notifications
	//	 wparam=[0/1/2/3] : [stop/play/pause/resume]
	ESYSTEM_EVENT_VIDEO,

	// Description:
	//	 Sent if the game is paused
	ESYSTEM_EVENT_GAME_PAUSED,

	// Description:
	//	 Sent if the game is resumed
	ESYSTEM_EVENT_GAME_RESUMED,

	// Description:
	//		Sent when time of day is set
	ESYSTEM_EVENT_TIME_OF_DAY_SET,

	// Description:
	//		Sent once the Editor finished initialization.
	ESYSTEM_EVENT_EDITOR_ON_INIT,

	// Description:
	//		Sent when frontend is initialised
	ESYSTEM_EVENT_FRONTEND_INITIALISED,

	// Description:
	//		Sent once the Editor switches between in-game and editing mode.
	ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED,

	// Description:
	//		Sent once the Editor switches simulation mode (AI/Physics).
	ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED,

	// Description:
	//		Sent when frontend is reloaded
	ESYSTEM_EVENT_FRONTEND_RELOADED,

	// Description:
	//		Sent once segmented world manager shifts the world
	ESYSTEM_EVENT_SHIFT_WORLD,

#ifdef DURANGO
	// Description: PLM (Process Life Management) events.
	ESYSTEM_EVENT_PLM_ON_RESUMING,
	ESYSTEM_EVENT_PLM_ON_SUSPENDING,
	ESYSTEM_EVENT_PLM_ON_CONSTRAINED,
	ESYSTEM_EVENT_PLM_ON_FULL,  // back from constrained to full resources
	ESYSTEM_EVENT_PLM_ON_TERMINATED,

	// durango only so far. They are triggered by voice/gesture standard system commands.
	ESYSTEM_EVENT_GLOBAL_SYSCMD_PAUSE,
	ESYSTEM_EVENT_GLOBAL_SYSCMD_SHOW_MENU,
	ESYSTEM_EVENT_GLOBAL_SYSCMD_PLAY,
	ESYSTEM_EVENT_GLOBAL_SYSCMD_BACK,
	ESYSTEM_EVENT_GLOBAL_SYSCMD_CHANGE_VIEW,

	ESYSTEM_EVENT_ACTIVATION_EVENT, // sent from external programs or the system.

	// ESYSTEM_EVENT_DURANGO_CHANGE_VISIBILITY is called when the app totally disappears or reappears. 
	// ESYSTEM_EVENT_CHANGE_FOCUS is called in that situation, but also when the app is constantly visible and only the focus changes. 
	// wparam = 0 -> visibility lost,  wparam = 1 -> visibility recovered
	ESYSTEM_EVENT_DURANGO_CHANGE_VISIBILITY, 

	// Matchmaking system events
	// wParam on state change is match search state
	// lParam unused
	ESYSTEM_EVENT_MATCHMAKING_SEARCH_STATE_CHANGE,
	// wParam and lParam unused
	ESYSTEM_EVENT_MATCHMAKING_GAME_SESSION_READY,

	// triggers when current user has been changed or started being signed out.
	ESYSTEM_EVENT_USER_SIGNOUT_STARTED,
	ESYSTEM_EVENT_USER_SIGNOUT_COMPLETED,

	// triggers when the user has completed the sign in process
	ESYSTEM_EVENT_USER_SIGNIN_COMPLETED,

	ESYSTEM_EVENT_USER_ADDED,
	ESYSTEM_EVENT_USER_REMOVED,
	ESYSTEM_EVENT_USER_CHANGED,

	// durango only so far. Triggers when the controller pairing changes, e.g. change user in account picker or handing over the controller to another user
	ESYSTEM_EVENT_CONTROLLER_PAIRING_CHANGED,
	ESYSTEM_EVENT_CONTROLLER_REMOVED,
	ESYSTEM_EVENT_CONTROLLER_ADDED,
#endif

	// Description:
	//		Currently durango only
	//		Triggers when streaming install had failed to open newly recevied pak files
	//		+ may be triggerd on platform error as well: like scratched disks or network problems
	//		when installing from store
	ESYSTEM_EVENT_STREAMING_INSTALL_ERROR,
	
	ESYSTEM_EVENT_USER = 0x1000,
};

// Description:
//	 User defined callback, which can be passed to ISystem.
struct ISystemUserCallback
{
	// <interfuscator:shuffle>
	virtual ~ISystemUserCallback(){}
	// Description:
	//	 This method is called at the earliest point the ISystem pointer can be used
    //	 the log might not be yet there.
	virtual void OnSystemConnect( ISystem *pSystem ) {}

	// Summary:
	//	 Signals to User that engine error occurred.
	// Return Value:
	//		True to Halt execution or false to ignore this error
	virtual bool OnError( const char *szErrorString ) = 0;

	// Notes:
	//	 If working in Editor environment notify user that engine want to Save current document.
	//	 This happens if critical error have occurred and engine gives a user way to save data and not lose it
	//	 due to crash.
#if defined(WIN32) || defined(WIN64)
	virtual bool OnSaveDocument() = 0;
#endif
	
	// Description:
	//	 Notifies user that system wants to switch out of current process.
	// Example:
	//  Called when pressing ESC in game mode to go to Menu.
	virtual void OnProcessSwitch() = 0;

	// Description:
	//	 Notifies user, usually editor, about initialization progress in system.
	virtual void OnInitProgress( const char *sProgressMsg ) = 0;

	// Description:
	//	 Initialization callback.  This is called early in CSystem::Init(), before
	//	 any of the other callback methods is called.
	// See also:
	//	 CSystem::Init()
	virtual void OnInit(ISystem *) { }

	// Summary:
	//	 Shutdown callback.
	virtual void OnShutdown() { }

	// Summary:
	//	 Quit callback.
	// See also:
	//   CSystem::Quit()
	virtual void OnQuit() { }

	// Description:
	//	 Notify user of an update iteration.  Called in the update loop.
	virtual void OnUpdate() { }

	// Description:
	//	 Show message by provider.
	virtual int ShowMessage(const char* text, const char* caption, unsigned int uType) { return CryMessageBox(text, caption, uType); }

	// Description:
	//	 Collects the memory information in the user program/application.
	virtual void GetMemoryUsage( ICrySizer* pSizer ) = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 Interface used for getting notified when a system event occurs.
struct ISystemEventListener
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventListener(){}
	virtual void OnSystemEventAnyThread( ESystemEvent event,UINT_PTR wparam,UINT_PTR lparam ) {}
	virtual void OnSystemEvent( ESystemEvent event,UINT_PTR wparam,UINT_PTR lparam ) = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 Structure used for getting notified when a system event occurs.
UNIQUE_IFACE struct ISystemEventDispatcher
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventDispatcher(){}
	virtual bool RegisterListener(ISystemEventListener *pListener) = 0;
	virtual bool RemoveListener(ISystemEventListener *pListener) = 0;

	virtual void OnSystemEvent( ESystemEvent event,UINT_PTR wparam,UINT_PTR lparam ) = 0;
	virtual void Update() = 0;

	//virtual void OnLocaleChange() = 0;
	// </interfuscator:shuffle>
};

struct IErrorObserver
{
	// <interfuscator:shuffle>
   virtual ~IErrorObserver() {}
    virtual void OnAssert(const char* condition, const char* message, const char* fileName, unsigned int fileLineNumber) = 0;
    virtual void OnFatalError(const char* message) = 0;
	// </interfuscator:shuffle>
};

enum ESystemProtectedFunctions
{
	eProtectedFunc_Save = 0,
	eProtectedFunc_Load = 1,
	eProtectedFuncsLast = 10,
};

struct SCvarsDefault
{
	SCvarsDefault()
	{
		sz_r_DriverDef = NULL;
		sz_e_GS_API_URI = NULL;
	}

	const char* sz_r_DriverDef;
	const char* sz_e_GS_API_URI;			//GameStream server address
};

#if defined(CVARS_WHITELIST)
struct ICVarsWhitelist
{
	// <interfuscator:shuffle>
	virtual ~ICVarsWhitelist() {};
	virtual bool IsWhiteListed(const string& command, bool silent) = 0;
	// </interfuscator:shuffle>
};
#endif // defined(CVARS_WHITELIST)

#ifdef DURANGO
struct SControllerPairingChanged
{
	struct SUserIdInfo
	{
		uint32 currentUserId;
		uint32 previousUserId;
	};

	SControllerPairingChanged(uint64 _raw) : raw(_raw) {}
	SControllerPairingChanged(uint32 currentUserId_ = 0, uint32 previousUserId_ = 0)
	{
		userInfo.currentUserId = currentUserId_;
		userInfo.previousUserId = previousUserId_;
	}

	union 
	{
		SUserIdInfo userInfo;
		uint64 raw;
	};
};
#endif

// Description:
//  Structure passed to Init method of ISystem interface.
struct SSystemInitParams
{
	void *hInstance;								//
	void *hWnd;										//
	ILog *pLog;										// You can specify your own ILog to be used by System.
	ILogCallback *pLogCallback;						// You can specify your own ILogCallback to be added on log creation (used by Editor).
	ISystemUserCallback *pUserCallback;				//
#if defined(CVARS_WHITELIST)
	ICVarsWhitelist* pCVarsWhitelist;		// CVars whitelist callback
#endif // defined(CVARS_WHITELIST)
	const char* sLogFileName;						// File name to use for log.
	IValidator *pValidator;							// You can specify different validator object to use by System.
	IOutputPrintSink *pPrintSync;				// Print Sync which can be used to catch all output from engine
	char szSystemCmdLine[2048];						// Command line.
	char szUserPath[256];						      // User alias path relative to My Documents folder.
	char szBinariesDir[256];

	bool bEditor;									// When running in Editor mode.
	bool bPreview;									// When running in Preview mode (Minimal initialization).
	bool bTestMode;									// When running in Automated testing mode.
	bool bDedicatedServer;							// When running a dedicated server.
	bool bExecuteCommandLine;						// can be switched of to suppress the feature or do it later during the initialization.
	bool bUIFramework;
	bool bSkipFont;										// Don't load CryFont.dll
	bool bSkipRenderer;									// Don't load Renderer
	bool bSkipConsole;									// Don't create console
	bool bSkipNetwork;									// Don't create Network
	bool bSkipLiveCreate;									// Don't create LiveCreate
	bool bMinimal;								// Don't load banks
	bool bSkipInput;							// do not load CryInput
	bool bTesting;								// CryUnit
	bool bNoRandom;								//use fixed generator init/seed
	bool bShaderCacheGen;					// When running in shadercache gen mode
	bool bUnattendedMode;							// When running as part of a build on build-machines: Prevent popping up of any dialog

	#ifdef DURANGO
	const EPLM_Event* pLastPLMEvent;
	#endif

	ISystem *pSystem;											// Pointer to existing ISystem interface, it will be reused if not NULL.
	IGameStartup *pGameStartup;						// Pointer to the calling GameStartup instance, to allow use of some game specific data during engine init.
//	char szLocalIP[256];									// local IP address (needed if we have several servers on one machine)
#if defined(LINUX) || defined(APPLE)
	void (*pCheckFunc)(void*);						// Authentication function (must be set).
#else
	void *pCheckFunc;								// Authentication function (must be set).
#endif

	typedef void* (*ProtectedFunction)( void *param1,void *param2 );
	ProtectedFunction pProtectedFunctions[eProtectedFuncsLast];			// Protected functions.

	SCvarsDefault	*pCvarsDefault;				// to override the default value of some cvar

	// Summary:
	//  Initialization defaults.
	SSystemInitParams()
	{
		hInstance = NULL;
		hWnd = NULL;
		pLog= NULL;
		pLogCallback = NULL;
		pUserCallback = NULL;
#if defined(CVARS_WHITELIST)
		pCVarsWhitelist = NULL;
#endif // defined(CVARS_WHITELIST)
		sLogFileName = NULL;
		pValidator = NULL;
		pPrintSync = NULL;
		memset(szSystemCmdLine, 0, sizeof(szSystemCmdLine));
		memset(szUserPath, 0, sizeof(szUserPath));
		memset(szBinariesDir, 0, sizeof(szBinariesDir));
		bEditor = false;
		bPreview = false;
		bTestMode = false;
		bDedicatedServer = false;
		bExecuteCommandLine = true;
		bUIFramework = false;
		bExecuteCommandLine=true;
		bSkipFont=false;
		bSkipRenderer=false;
		bSkipConsole=false;
		bSkipNetwork=false;
		bMinimal=false;
		bSkipInput = false;
		bSkipLiveCreate = false;
		bTesting = false;
		bNoRandom = false;
		bShaderCacheGen = false;
		bUnattendedMode = false;

		pSystem = NULL;
		pGameStartup = NULL;
		pCheckFunc = NULL;

#ifdef DURANGO
		pLastPLMEvent = NULL;
#endif

		memset(pProtectedFunctions, 0, sizeof(pProtectedFunctions));
		pCvarsDefault = NULL;
	}
};

// Summary:
//	 Typedef for frame profile callback function.
typedef void (*FrameProfilerSectionCallback)( class CFrameProfilerSection *pSection );

// Notes:
//	 Can be used for LoadConfiguration().
// See also:
//	 LoadConfiguration()
struct ILoadConfigurationEntrySink
{
	// <interfuscator:shuffle>
	virtual ~ILoadConfigurationEntrySink(){}
	virtual void OnLoadConfigurationEntry( const char *szKey, const char *szValue, const char *szGroup )=0;
	virtual void OnLoadConfigurationEntry_End() {}
	// </interfuscator:shuffle>
};

struct SPlatformInfo
{
	unsigned int numCoresAvailableToProcess;

#if defined(WIN32) || defined(WIN64)
	enum EWinVersion
	{
		WinUndetected,
		Win2000,
		WinXP,
		WinSrv2003,
		WinVista,
		Win7,
		Win8
	};

	EWinVersion winVer;
	bool win64Bit;
	bool vistaKB940105Required;
#endif
};

// Description:
//	Holds info about system update stats over perior of time (cvar-tweakable)

struct SSystemUpdateStats
{
	SSystemUpdateStats():avgUpdateTime(0.0f),minUpdateTime(0.0f),maxUpdateTime(0.0f){}
	float avgUpdateTime;
	float minUpdateTime;
	float maxUpdateTime;
};

// Description:
//	 Global environment.
//	 Contains pointers to all global often needed interfaces.
//	  This is a faster way to get interface pointer then calling ISystem interface to retrieve one.
// Notes:
//	 Some pointers can be NULL, use with care.
// See also:
//	 ISystem
struct SSystemGlobalEnvironment
{
	IDialogSystem*                pDialogSystem;
	I3DEngine*                    p3DEngine;
	INetwork*                     pNetwork;
	ICryLobby*                    pLobby;
	IScriptSystem*                pScriptSystem;
	IPhysicalWorld*               pPhysicalWorld;
	IFlowSystem*                  pFlowSystem;
	IInput*                       pInput;
	IMusicSystem*                 pMusicSystem;
	IStatoscope*                  pStatoscope;
	IAnimationGraphSystem*        pAnimationGraphSystem;
	ICryPak*                      pCryPak;
	IFileChangeMonitor*           pFileChangeMonitor;
	IProfileLogSystem*            pProfileLogSystem;
	IParticleManager*             pParticleManager;
	IOpticsManager*               pOpticsManager;
	IFrameProfileSystem*          pFrameProfileSystem;
	ITimer*                       pTimer;
	ICryFont*                     pCryFont;
	IGame*                        pGame;
	ILocalMemoryUsage*            pLocalMemoryUsage;
	IEntitySystem*                pEntitySystem;
	IConsole*                     pConsole;
	Telemetry::ITelemetrySystem*  pTelemetrySystem;
	ISoundSystem*                 pSoundSystem;
	ISystem*                      pSystem;
	ICharacterManager*            pCharacterManager;
	IAISystem*                    pAISystem;
	ILog*                         pLog;
	ICodeCheckpointMgr*           pCodeCheckpointMgr;
	IMovieSystem*                 pMovieSystem;
	INameTable*                   pNameTable;
	IVisualLog*                   pVisualLog;
	IRenderer*                    pRenderer;
	IHardwareMouse*               pHardwareMouse;
	IMaterialEffects*             pMaterialEffects;
	JobManager::IJobManager*      pJobManager;
	ISoftCodeMgr*                 pSoftCodeMgr;
	IGameStream*                  pGameStream;
	IOverloadSceneManager*        pOverloadSceneManager;
	IFlashUI*                     pFlashUI;
	IServiceNetwork*              pServiceNetwork;
	IRemoteCommandManager*        pRemoteCommandManager;

#if defined(DURANGO)
	void*                      pWindow;
	EPLM_State								ePLM_State;
#endif

#if defined(MAP_LOADING_SLICING)
	ISystemScheduler*          pSystemScheduler;
#endif
	LiveCreate::IManager*			pLiveCreateManager;
	LiveCreate::IHost*				pLiveCreateHost;

	threadID								 mMainThreadId;		//The main thread ID is used in multiple systems so should be stored globally

	//////////////////////////////////////////////////////////////////////////
	uint32                     nMainFrameID;

	//////////////////////////////////////////////////////////////////////////
	const char*                szCmdLine;  // Startup command line.

	//////////////////////////////////////////////////////////////////////////
	// Generic debug string which can be easily updated by any system and output by the debug handler
	enum { MAX_DEBUG_STRING_LENGTH = 128 };
	char											szDebugStatus[MAX_DEBUG_STRING_LENGTH];

	//////////////////////////////////////////////////////////////////////////
	// Used to tell if this is a server/multiplayer instance
	bool                       bServer;
	bool											 bMultiplayer;
	bool                       bHostMigrating;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Used by frame profiler.
	bool                       bProfilerEnabled;
	FrameProfilerSectionCallback callbackStartSection;
	FrameProfilerSectionCallback callbackEndSection;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Indicate Editor status.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Used by CRY_ASSERT
	bool											bIgnoreAllAsserts;
	bool											bNoAssertDialog;
	bool											bTesting;
	//////////////////////////////////////////////////////////////////////////

	bool											bNoRandomSeed;

	SPlatformInfo pi;

	// Protected functions.
	SSystemInitParams::ProtectedFunction pProtectedFunctions[eProtectedFuncsLast];  // Protected functions.

	//////////////////////////////////////////////////////////////////////////
	// Flag to able to print out of memory conditon
	bool											bIsOutOfMemory;

	ILINE const bool IsClient() const
	{
#if defined(CONSOLE)
		return true;
#else
		return bClient;
#endif
	}

	ILINE const bool IsDedicated() const
	{
#if defined(CONSOLE)
		return false;
#elif defined(DEDICATED_SERVER)
		return true;
#else
		return bDedicated;
#endif
	}

#if !defined(CONSOLE)
	ILINE void SetIsEditor(bool isEditor)
	{
		bEditor = isEditor;
	}

	ILINE void SetIsEditorGameMode(bool isEditorGameMode)
	{
		bEditorGameMode = isEditorGameMode;
	}

	ILINE void SetIsDedicated(bool isDedicated)
	{
#if defined(DEDICATED_SERVER)
		bDedicated = true;
#else
		bDedicated = isDedicated;
#endif
	}

	ILINE void SetIsClient(bool isClient)
	{
		bClient = isClient;
	}
#endif

	//this way the compiler can strip out code for consoles
	ILINE const bool IsEditor() const
	{
#if defined(CONSOLE)
		return false;
#else
		return bEditor;
#endif
	}

	ILINE const bool IsEditorGameMode() const
	{
#if defined(CONSOLE)
		return false;
#else
		return bEditorGameMode;
#endif
	}

	ILINE const bool IsEditing() const
	{
#if defined(CONSOLE)
		return false;
#else
		if (!pGame)
			return bEditor;
		else
			return bEditor && !bEditorGameMode;
#endif
	}

	ILINE const bool IsFMVPlaying() const
	{
		return m_isFMVPlaying;
	}

	ILINE void SetFMVIsPlaying(const bool isPlaying)
	{
		m_isFMVPlaying = isPlaying;
	}

	ILINE const bool IsCutscenePlaying() const
	{
		return m_isCutscenePlaying;
	}

	ILINE void SetCutsceneIsPlaying(const bool isPlaying)
	{
		m_isCutscenePlaying = isPlaying;
	}

	// Remove pointer indirection.
#if defined(SYS_ENV_AS_STRUCT)
	ILINE SSystemGlobalEnvironment* operator->()
	{
		return this;
	}

	ILINE const bool operator !()const
	{
		return false;
	}

	ILINE operator bool() const
	{
		return true;
	}
#endif

	// Getter function for jobmanager
	ILINE JobManager::IJobManager* GetJobManager()
	{
		return pJobManager;
	}

#if !defined(CONSOLE)
	bool bDedicatedArbitrator;

private:
	bool bClient;
	bool bEditor;          // Engine is running under editor.
	bool bEditorGameMode;  // Engine is in editor game mode.
	bool bDedicated;			 // Engine is in dedicated 
#endif

	bool m_isFMVPlaying;
	bool m_isCutscenePlaying;

public:
	SSystemGlobalEnvironment() : szCmdLine("") {};
};

// NOTE Nov 25, 2008: <pvl> the ISystem interface that follows has a member function
// called 'GetUserName'.  If we don't #undef'ine the same-named Win32 symbol here
// ISystem wouldn't even compile.
// TODO Nov 25, 2008: <pvl> there might be a better place for this?
#ifdef GetUserName
#undef GetUserName
#endif


UNIQUE_IFACE struct IProfilingSystem
{
	// <interfuscator:shuffle>
	virtual ~IProfilingSystem(){}
	//////////////////////////////////////////////////////////////////////////
	// VTune Profiling interface.

	// Summary:
	//	 Resumes vtune data collection.
	virtual void VTuneResume() = 0;
	// Summary:
	//	 Pauses vtune data collection.
	virtual void VTunePause() = 0;
	//////////////////////////////////////////////////////////////////////////

	// XBox360 Profiling interface
	// Summary:
	//	 Start data collection.
	// Argument: Filename for captured data. 
	virtual void StartProfilingX360(const char * fileName) = 0;
	// Summary:
	//	 Stop data collection and write to a hard drive.
	virtual void StopProfilingX360() = 0;
	// </interfuscator:shuffle>
};

////////////////////////////////////////////////////////////////////////////////////////////////

// Description:
//	 Main Engine Interface.
//	 Initialize and dispatch all engine's subsystems. 
UNIQUE_IFACE struct ISystem
{ 
	struct ILoadingProgressListener
	{
		// <interfuscator:shuffle>
		virtual ~ILoadingProgressListener(){}
		virtual void OnLoadingProgress(int steps) = 0;
		// </interfuscator:shuffle>
	};

#ifndef _RELEASE
	enum LevelLoadOrigin
	{
		eLLO_Unknown,
		eLLO_NewLevel,
		eLLO_Level2Level,
		eLLO_Resumed,
		eLLO_MapCmd,
	};

	struct ICheckpointData
	{
		int							m_totalLoads;
		LevelLoadOrigin	m_loadOrigin;
	};
#endif

	// <interfuscator:shuffle>
	virtual ~ISystem(){}
	// Summary:
	//	 Releases ISystem.
	virtual void Release() = 0;
	virtual ILoadConfigurationEntrySink* GetCVarsWhiteListConfigSink() const = 0; // will return NULL if no whitelisting

	// Summary:
	//	 Returns pointer to the global environment structure.
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment() = 0;

	// Summary:
	//	 Returns the root folder specified by the command line option "-root <path>"
	virtual const char* GetRootFolder() const = 0;

	// Summary:
	//	 Updates all subsystems (including the ScriptSink() )
	// Arguments:
	//   flags		- One or more flags from ESystemUpdateFlags structure.
	//   nPauseMode - 0=normal(no pause), 1=menu/pause, 2=cutscene
	virtual bool Update( int updateFlags=0, int nPauseMode=0 ) = 0;

	// Summary:
	//	 Updates only require components during loading
	virtual bool UpdateLoadtime() = 0;

	// Summary:
	//   Optimisation: do part of the update while waiting for occlusion queries to complete
	virtual void DoWorkDuringOcclusionChecks() = 0;
	virtual bool NeedDoWorkDuringOcclusionChecks() = 0;

	// Summary:
	//	 Renders subsystems.
	virtual void	Render() = 0;
	// Summary:
	//	 Begins rendering frame.
	virtual void	RenderBegin() = 0;
	// Summary:
	//	 Ends rendering frame and swap back buffer.
	virtual void	RenderEnd( bool bRenderStats=true ) = 0;

	//! Update screen and call some important tick functions during loading.
	virtual void SynchronousLoadingTick( const char* pFunc, int line ) = 0;

	// Description:
	//	 Renders the statistics; this is called from RenderEnd, but if the 
	//	 Host application (Editor) doesn't employ the Render cycle in ISystem,
	//	 it may call this method to render the essential statistics.
	virtual void RenderStatistics () = 0;
	virtual void RenderPhysicsStatistics (IPhysicalWorld* pWorld) = 0;

	// Summary:
	//	 Returns the current used memory.
	virtual uint32 GetUsedMemory() = 0;

	// Summary:
	//	 Retrieve the name of the user currently logged in to the computer.
	virtual const char *GetUserName() = 0;

	// Summary:
	//	 Gets current supported CPU features flags. (CPUF_SSE, CPUF_SSE2, CPUF_3DNOW, CPUF_MMX)
	virtual int GetCPUFlags() = 0;

	// Summary:
	//	 Gets number of CPUs
	virtual int GetLogicalCPUCount() = 0;

	// Summary:
	//	 Gets seconds per processor tick.
	virtual double GetSecondsPerCycle() = 0;

	// Summary:
	//	 Dumps the memory usage statistics to the logging default MB. (can be KB)
	virtual void DumpMemoryUsageStatistics(bool bUseKB=false) = 0;

	// Summary:
	//	 Quits the application.
	virtual void	Quit() = 0;
	// Summary:
	//	 Tells the system if it is relaunching or not.
	virtual void	Relaunch(bool bRelaunch) = 0;
	// Summary:
	//	 Returns true if the application is in the shutdown phase.
	virtual bool	IsQuitting() const = 0;
	// Summary:
	//	 Returns true if the application was initialized to generate the shader cache.
	virtual bool	IsShaderCacheGenMode() const = 0;
	// Summary:
	//	 Tells the system in which way we are using the serialization system.
	virtual void  SerializingFile(int mode) = 0;	
	virtual int IsSerializingFile() const = 0;

	virtual bool IsRelaunch() const = 0;
	// Summary:
	//   Displays an error message to display info for certain time
	// Arguments:
	//   acMessage - Message to show
	//   fTime - Amount of seconds to show onscreen
	virtual void DisplayErrorMessage(const char* acMessage, float fTime, const float* pfColor = 0, bool bHardError = true) = 0;

	// Description:
	//	 Displays error message.
	//	 Logs it to console and file and error message box then terminates execution.
	virtual void FatalError( const char *sFormat, ... ) PRINTF_PARAMS(2, 3) = 0;

	// Description:
	//	 Reports a bug using the crash handler.
	//	 Logs an error to the console and launches the crash handler, then continues execution.
	virtual void ReportBug( const char *sFormat, ... ) PRINTF_PARAMS(2, 3) = 0;
	
	// Description:
	//	 Report warning to current Validator object.
	//	 Doesn't terminate the execution.
	//##@{
	virtual void WarningV( EValidatorModule module, EValidatorSeverity severity, int flags, const char *file, const char *format, va_list args ) = 0;
	virtual void Warning( EValidatorModule module, EValidatorSeverity severity, int flags, const char *file, const char *format, ... ) = 0;
	//##@}

	// Description:
	//	 Report message by provider or by using CryMessageBox.
	//	 Doesn't terminate the execution.
	virtual int ShowMessage(const char* text, const char* caption, unsigned int uType) = 0;

	// Summary:
	//	 Compare specified verbosity level to the one currently set.
	virtual bool CheckLogVerbosity( int verbosity ) = 0;

	virtual bool IsUIFrameworkMode() { return false; }

	// return the related subsystem interface

	//
	virtual IZLibCompressor *GetIZLibCompressor() = 0;
	virtual ICryPerfHUD *GetPerfHUD() = 0;
	virtual IPlatformOS	*GetPlatformOS() = 0;
	virtual INotificationNetwork *GetINotificationNetwork() = 0;
	virtual IHardwareMouse *GetIHardwareMouse() = 0;
	virtual IDialogSystem *GetIDialogSystem() = 0;
	virtual IAnimationGraphSystem *GetIAnimationGraphSystem() = 0;
	virtual IFlowSystem *GetIFlowSystem() = 0;
	virtual IBudgetingSystem *GetIBudgetingSystem() = 0;
	virtual INameTable *GetINameTable() = 0;
	virtual IDiskProfiler *GetIDiskProfiler() = 0;	
	virtual IFrameProfileSystem *GetIProfileSystem() = 0;	
	virtual IValidator *GetIValidator() = 0;
	virtual IPhysicsDebugRenderer* GetIPhysicsDebugRenderer() = 0;
	virtual ICharacterManager *GetIAnimationSystem() = 0;
	virtual IStreamEngine *GetStreamEngine() = 0;
	virtual ICmdLine *GetICmdLine() = 0;
	virtual ILog *GetILog() = 0;
	virtual ICryPak *GetIPak()	= 0;
	virtual ICryFont *GetICryFont()	= 0;
	virtual IEntitySystem *GetIEntitySystem() = 0;
	virtual IMemoryManager *GetIMemoryManager() = 0;
	virtual IAISystem *GetAISystem() = 0;
	virtual IMovieSystem *GetIMovieSystem() = 0;
	virtual IPhysicalWorld *GetIPhysicalWorld() = 0;
	virtual IMusicSystem *GetIMusicSystem() = 0;
	virtual ISoundSystem *GetISoundSystem() = 0;
	virtual I3DEngine *GetI3DEngine() = 0;
	virtual IScriptSystem *GetIScriptSystem() = 0;
	virtual IConsole *GetIConsole() = 0;
	virtual IRemoteConsole *GetIRemoteConsole() = 0;
	// Returns:
	//   Can be NULL, because it only exists when running through the editor, not in pure game mode. 
	virtual IResourceManager *GetIResourceManager() = 0;
	virtual IThreadTaskManager *GetIThreadTaskManager() = 0;
	virtual IProfilingSystem * GetIProfilingSystem() = 0;
	virtual ISystemEventDispatcher *GetISystemEventDispatcher() = 0;
	virtual IVisualLog *GetIVisualLog() = 0;
	virtual IFileChangeMonitor *GetIFileChangeMonitor() = 0;

	virtual WIN_HWND GetHWND() = 0;

	virtual IGame *GetIGame() = 0;
	virtual INetwork *GetINetwork() = 0;
	virtual IRenderer *GetIRenderer() = 0;
	virtual IInput *GetIInput() = 0;
	virtual ITimer *GetITimer() = 0;

	virtual void SetLoadingProgressListener(ILoadingProgressListener *pListener) = 0;
	virtual ISystem::ILoadingProgressListener *GetLoadingProgressListener() const = 0;

	// Summary:
	//	 Game is created after System init, so has to be set explicitly.
	virtual void						SetIGame(IGame* pGame) = 0;
	virtual void            SetIFlowSystem(IFlowSystem* pFlowSystem) = 0;
	virtual void SetIAnimationGraphSystem(IAnimationGraphSystem* pAnimationGraphSystem) = 0;
	virtual void SetIDialogSystem(IDialogSystem* pDialogSystem) = 0;
	virtual void SetIMaterialEffects(IMaterialEffects* pMaterialEffects) = 0;
	virtual void SetIParticleManager(IParticleManager* pParticleManager) = 0;
	virtual void SetIOpticsManager(IOpticsManager* pOpticsManager) = 0;
	virtual void SetIFileChangeMonitor(IFileChangeMonitor* pFileChangeMonitor) = 0;
	virtual void SetIVisualLog(IVisualLog* pVisualLog) = 0;
	virtual void SetIFlashUI(IFlashUI* pFlashUI) = 0;
	// Summary:
	//	 Changes current user sub path, the path is always relative to the user documents folder. 
	// Example: 
	//	 "My Games\Crysis"
	virtual void ChangeUserPath( const char *sUserPath ) = 0;

	//virtual	const char			*GetGamePath()=0;

	virtual void DebugStats(bool checkpoint, bool leaks) = 0;
	virtual void DumpWinHeaps() = 0;
	virtual int DumpMMStats(bool log) = 0;

	// Arguments:
	//	 bValue - Set to true when running on a cheat protected server or a client that is connected to it (not used in singleplayer).
	virtual void SetForceNonDevMode( const bool bValue )=0;
	// Return Value:
	//	 True when running on a cheat protected server or a client that is connected to it (not used in singleplayer).
	virtual bool GetForceNonDevMode() const=0;
	virtual bool WasInDevMode() const=0;
	virtual bool IsDevMode() const=0;
	virtual bool IsMODValid(const char *szMODName) const=0;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IXmlNode interface.
	//////////////////////////////////////////////////////////////////////////
	
	// Summary:
	//	 Creates new xml node.
	virtual XmlNodeRef CreateXmlNode( const char *sNodeName="", bool bReuseStrings = false ) = 0;
	// Summary:
	//	 Loads xml from memory buffer, returns 0 if load failed.
	virtual XmlNodeRef LoadXmlFromBuffer( const char *buffer, size_t size, bool bReuseStrings = false ) = 0;
	// Summary:
	//	 Loads xml file, returns 0 if load failed.
	virtual XmlNodeRef LoadXmlFromFile( const char *sFilename, bool bReuseStrings = false ) = 0;
	// Summary:
	//	 Retrieves access to XML utilities interface.
	virtual IXmlUtils* GetXmlUtils() = 0;

	virtual void SetViewCamera(CCamera &Camera) = 0;
	virtual CCamera& GetViewCamera() = 0;

	// Description:
	//	 When ignore update sets to true, system will ignore and updates and render calls.
	virtual void IgnoreUpdates( bool bIgnore ) = 0;

	// Summary:
	//	 Sets the active process
	// Arguments:
	//	 process - A pointer to a class that implement the IProcess interface.
	virtual void SetIProcess(IProcess *process) = 0;

	// Summary:
	//	 Gets the active process.
	// Return Value:	
	//	 A pointer to the current active process.
	virtual IProcess* GetIProcess() = 0;

	// Return Value:
	//	 True if system running in Test mode.
	virtual bool IsTestMode() const = 0;

	// Summary:
	//	 Frame profiler functions
	virtual void SetFrameProfiler(bool on, bool display, char *prefix) = 0;


  //////////////////////////////////////////////////////////////////////////
  // Loading time/memory profiling
  //////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Starts function loading stats profiling.
	virtual struct SLoadingTimeContainer * StartLoadingSectionProfiling(CLoadingTimeProfiler * pProfiler, const char * szFuncName) = 0;

	// Summary:
	//	 Ends function loading stats profiling.
	virtual void EndLoadingSectionProfiling(CLoadingTimeProfiler * pProfiler) = 0;

	// Summary:
	//	 Prints loading stats into log.
	virtual void OutputLoadingTimeStats() = 0;

	// Summary:
	//	 Starts function loading stats profiling.
	virtual const char* GetLoadingProfilerCallstack() = 0;

	//////////////////////////////////////////////////////////////////////////
	// File version.
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Gets file version.
	virtual const SFileVersion& GetFileVersion() = 0;
  // Summary:
  //	 Gets product version.
  virtual const SFileVersion& GetProductVersion() = 0;
  // Summary:
  //	 Gets build version.
  virtual const SFileVersion& GetBuildVersion() = 0;
	
	// Summary:
	//	 Compressed file read & write
	//##@{
	virtual bool WriteCompressedFile(const char *filename, void *data, unsigned int bitlen) = 0;
	virtual unsigned int ReadCompressedFile(const char *filename, void *data, unsigned int maxbitlen) = 0;
	virtual unsigned int GetCompressedFileSize(const char *filename)=0;
	virtual bool CompressDataBlock( const void * input, size_t inputSize, void * output, size_t& outputSize, int level = 3 ) = 0;
	virtual bool DecompressDataBlock( const void * input, size_t inputSize, void * output, size_t& outputSize ) = 0;
	//##@}

	// Summary:
	//	 Retrieves IDataProbe interface.
	virtual IDataProbe* GetIDataProbe() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Configuration.
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Saves system configuration.
	virtual void SaveConfiguration() = 0;

	// Summary:
	//	 Loads system configuration
	// Arguments:
	//   pCallback - 0 means normal LoadConfigVar behaviour is used
	virtual void LoadConfiguration( const char *sFilename, ILoadConfigurationEntrySink *pSink=0 )=0;

	// Summary:
	//	 Retrieves current configuration specification for client or server.
	// Arguments:
	//   bClient - If true returns local client config spec, if false returns server config spec.
	virtual ESystemConfigSpec GetConfigSpec( bool bClient=true ) = 0;
	
	virtual ESystemConfigSpec GetMaxConfigSpec() const = 0;

	// Summary:
	//	 Changes current configuration specification for client or server.
	// Arguments:
	//   bClient - If true changes client config spec (sys_spec variable changed), 
	//             if false changes only server config spec (as known on the client).
	virtual void SetConfigSpec( ESystemConfigSpec spec,bool bClient ) = 0;
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Detects and set optimal spec.
	virtual void AutoDetectSpec(const bool detectResolution) = 0;

	// Summary:
	//	 Thread management for subsystems
	// Return Value:
	//	 Non-0 if the state was indeed changed, 0 if already in that state.
	virtual int SetThreadState(ESubsystem subsys, bool bActive) = 0;

	// Summary:
	//	 Creates and returns a usable object implementing ICrySizer interface.
	virtual ICrySizer* CreateSizer() = 0;

	// Summary:
	//	 Query if system is now paused.
	//	 Pause flag is set when calling system update with pause mode.
	virtual bool IsPaused() const = 0;

	// Summary:
	//	 Retrieves localized strings manager interface.
	virtual ILocalizationManager* GetLocalizationManager() = 0;

	// Summary:
	//	 Creates an instance of the IFlashPlayer interface.
	virtual IFlashPlayer* CreateFlashPlayerInstance() const = 0;
	virtual IFlashPlayerBootStrapper* CreateFlashPlayerBootStrapper() const = 0;
	virtual void SetFlashLoadMovieHandler(IFlashLoadMovieHandler* pHandler) const = 0;
	virtual void GetFlashProfileResults(float& accumTime, bool reset) const = 0;
	virtual void ResetFlashMeshCache() const = 0;
	virtual void GFxAmpEnable(bool bEnable) = 0;
	virtual void GFxAmpAdvanceFrame() = 0;

	// Summary:
	//	Gets instance of a head mounted display device.
	//	It is currently possible to have only one device per type, so far.
	//	To choose which device will be your current one, use the console 
	//	variable sys_CurrentHMDType = IHMDDevice::Type.
	virtual IHMDDevice* GetHMDDevice() const = 0;

	// Summary:
	//	 Creates an instance of the AVI Reader class.
	virtual IAVI_Reader *CreateAVIReader() = 0;
	// Release the AVI reader
	virtual void ReleaseAVIReader(IAVI_Reader *pAVIReader) = 0;

	virtual ITextModeConsole * GetITextModeConsole() = 0;

	// Summary:
	//	 Retrieves the crc32 singleton instance.
	virtual Crc32Gen* GetCrc32Gen() = 0;

	// Summary:
	//	 Retrieves the perlin noise singleton instance.
	virtual CPNoise3* GetNoiseGen() = 0;

	// Summary:
	//	 Retrieves system update counter.
	virtual uint64 GetUpdateCounter() = 0;

	// Summary:
	//	 Gets access to all registered factories.
	virtual ICryFactoryRegistry* GetCryFactoryRegistry() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// Error callback handling

	// Summary:
	//  Registers listeners to CryAssert and error messages. (may not be called if asserts are disabled)
	//  Each pointer can be registered only once. (stl::push_back_unique)
	//  It will return false if the pointer is already registered. Returns true, otherwise.
	virtual bool RegisterErrorObserver(IErrorObserver* errorObserver) = 0;

	// Summary:
	//  Unregisters listeners to CryAssert and error messages.
	//  It will return false if the pointer is not registered. Otherwise, returns true.
	virtual bool UnregisterErrorObserver(IErrorObserver* errorObserver) = 0;

	// Summary:
	//  Called after the processing of the assert message box(Windows or Xbox).
	//  It will be called even when asserts are disabled by the console variables.
	virtual void OnAssert(const char* condition, const char* message, const char* fileName, unsigned int fileLineNumber) = 0;

	// Summary:
	// Returns if the assert window from CryAssert is visible.
	// OBS1: needed by the editor, as in some cases it can freeze if during an assert engine it will handle 
	// some events such as mouse movement in a CryPhysics assert.
	// OBS2: it will always return false, if asserts are disabled or ignored.
	virtual bool IsAssertDialogVisible() const = 0;
	
	// Summary:
	// Sets the AssertVisisble internal variable.
	// Typically it should only be called by CryAssert.
	virtual void SetAssertVisible(bool bAssertVisble) = 0;
	//////////////////////////////////////////////////////////////////////////

	// Summary:
	//	 Get the index of the currently running Crytek application. (0 = first instance, 1 = second instance, etc)
	virtual int GetApplicationInstance() = 0;

	// Summary: 
	//		Retrieves the current stats for systems to update the respective time taken
	virtual sUpdateTimes& GetCurrentUpdateTimeStats() = 0; 

	// Summary: 
	//		Retrieves the array of update times and the number of entries  
	virtual const sUpdateTimes* GetUpdateTimeStats(uint32&, uint32&) = 0; 

	// Summary: 
	//		Clear all currently logged and drawn on screen error messages
	virtual void ClearErrorMessages() = 0;
	//////////////////////////////////////////////////////////////////////////
	// For debugging use only!, query current C++ call stack.
	//////////////////////////////////////////////////////////////////////////

	// Notes:
	//	 Pass nCount to indicate maximum number of functions to get.
	//	 For debugging use only, query current C++ call stack.
	// Description:
	//	 Fills array of function pointers, nCount return number of functions.
	virtual void debug_GetCallStack( const char **pFunctions,int &nCount ) = 0;
	// Summary:
	//	 Logs current callstack.
	// Notes:
	//   For debugging use only!, query current C++ call stack.
	virtual void debug_LogCallStack( int nMaxFuncs=32,int nFlags=0 ) = 0;

	// Summary:
	//	 Can be called through console 
	// Example:
	//	 #System.ApplicationTest("testcase0")
	// Arguments:
	//   szParam - 0 generates error
	virtual void ApplicationTest( const char *szParam ) = 0;
	// Return Value:
	//   0 if not activated, activate through #System.ApplicationTest
	virtual ITestSystem *GetITestSystem() = 0;

	// Description:
	//	 Execute command line arguments.
	//	 Should be after init game.
	// Example:
	//	 +g_gametype ASSAULT +map "testy"
	virtual void ExecuteCommandLine() = 0;

	// Description:
	//	GetSystemUpdate stats (all systems update without except console)
	//  very useful on dedicated server as we throttle it to fixed frequency
	//  returns zeroes if no updates happened yet
	virtual void GetUpdateStats(SSystemUpdateStats& stats) = 0;

	// Description:
	//	 Useful to investigate memory fragmentation.
	//	 Every time you call this from the console: #System.DumpMemoryCoverage()
	//	 it adds a line to "MemoryCoverage.bmp" (generated the first time, there is a max line count).
	virtual void DumpMemoryCoverage() = 0;
	virtual ESystemGlobalState	GetSystemGlobalState(void) = 0;
	virtual void SetSystemGlobalState(const ESystemGlobalState systemGlobalState) = 0;

	// Description:
	//	Reset the watchdog timer
	virtual void ResetWatchdogTimer() = 0;

	// Summary:
	//		Add a PlatformOS create flag
	virtual void AddPlatformOSCreateFlag( const uint8 createFlag ) = 0;

	// Summary:
	//		Asynchronous memcpy 
	// Note sync variable will be incremented (in calling thread) before job starts
	// and decremented when job finishes. Multiple async copies can therefore be
	// tied to the same sync variable, therefore it's advised to wait for completion with
	// while(*sync) (yield());
	virtual void AsyncMemcpy(void* dst, const void* src, size_t size, int nFlags, volatile int* sync) = 0;
	// </interfuscator:shuffle>
	
	virtual struct ILevelEncrypter *GetLevelEncrypter() = 0;
	virtual struct IEvaluationManager* GetEvaluationManager() = 0;
	virtual char* GetDeveloperName(char* devName) = 0;

#if defined(CVARS_WHITELIST)
	virtual ICVarsWhitelist* GetCVarsWhiteList() const = 0;
#endif // defined(CVARS_WHITELIST)

#ifndef _RELEASE
	virtual void GetCheckpointData(ICheckpointData& data) = 0;
	virtual void IncreaseCheckpointLoadCount() = 0;
	virtual void SetLoadOrigin(LevelLoadOrigin origin) = 0;
#endif

#ifndef _LIB
	// Just for DLL based version: implementation for CryStringUtils::HashString[Lower]Seed
	virtual uint32 HashStringSeed( const char* string, const uint32 seed ) = 0;
	virtual uint32 HashStringLowerSeed( const char* string, const uint32 seed ) = 0;
#endif

#ifdef DURANGO
	virtual void OnPLMEvent( EPLM_Event event ) = 0;
#endif

#if !defined(_RELEASE)
	virtual bool IsSavingResourceList() const = 0;
#endif
	
};

//JAT - this is a very important function for the dedicated server - it lets us run >1000 players per piece of server hardware
//JAT - this saves us lots of money on the dedicated server hardware
#define SYNCHRONOUS_LOADING_TICK() do { if (gEnv && gEnv->pSystem) gEnv->pSystem->SynchronousLoadingTick( __FUNC__, __LINE__ ); } while(0)

#if defined(ENABLE_LOADING_PROFILER)

struct DiskOperationInfo 
{
	DiskOperationInfo() : m_nSeeksCount(0), m_nFileOpenCount(0), m_nFileReadCount(0) , m_dOperationSize(0.), m_dOperationTime(0.)	{}
	int m_nSeeksCount;
	int m_nFileOpenCount;
	int m_nFileReadCount;
	double m_dOperationTime;
	double m_dOperationSize;

	DiskOperationInfo& operator -= (const DiskOperationInfo& rv) {
		m_nSeeksCount -= rv.m_nSeeksCount;
		m_nFileOpenCount -= rv.m_nFileOpenCount;
		m_nFileReadCount -= rv.m_nFileReadCount;
		m_dOperationSize -= rv.m_dOperationSize;
		m_dOperationTime -= rv.m_dOperationTime;
		return *this;
	}

	DiskOperationInfo& operator += (const DiskOperationInfo& rv) {
		m_nSeeksCount += rv.m_nSeeksCount;
		m_nFileOpenCount += rv.m_nFileOpenCount;
		m_nFileReadCount += rv.m_nFileReadCount;
		m_dOperationSize += rv.m_dOperationSize;
		m_dOperationTime += rv.m_dOperationTime;
		return *this;
	}

	DiskOperationInfo& operator - (const DiskOperationInfo& rv) {
		DiskOperationInfo res(*this);
		return res -= rv;
	}

	DiskOperationInfo& operator + (const DiskOperationInfo& rv) {
		DiskOperationInfo res(*this);
		return res += rv;
	}

};

struct CLoadingTimeProfiler
{
  CLoadingTimeProfiler (ISystem * pSystem, const char * szFuncName) : m_pSystem (pSystem)
  {
    m_pSystem = pSystem;
    m_pTimeContainer = m_pSystem->StartLoadingSectionProfiling(this, szFuncName);
  }

  ~CLoadingTimeProfiler ()
  {
    m_pSystem->EndLoadingSectionProfiling(this);
  }

  struct SLoadingTimeContainer * m_pTimeContainer;
  double m_fConstructorTime;
  double m_fConstructorMemUsage;

	DiskOperationInfo m_constructorInfo;

  ISystem* m_pSystem;
};

#	if defined(PS3) // for PS3 use __PRETTY_FUNCTION__, since __FUNC__ is overloaded with a macro
#		define LOADING_TIME_PROFILE_SECTION PREFAST_SUPPRESS_WARNING(6246); CLoadingTimeProfiler __section_loading_time_auto_profiler(gEnv->pSystem, __PRETTY_FUNCTION__);
#else
#		define LOADING_TIME_PROFILE_SECTION PREFAST_SUPPRESS_WARNING(6246); CLoadingTimeProfiler __section_loading_time_auto_profiler(gEnv->pSystem, __FUNC__);
#	endif
#	define LOADING_TIME_PROFILE_SECTION_NAMED(sectionName) PREFAST_SUPPRESS_WARNING(6246); CLoadingTimeProfiler __section_loading_time_auto_profiler(gEnv->pSystem, sectionName);

#else

#	define LOADING_TIME_PROFILE_SECTION
#	define LOADING_TIME_PROFILE_SECTION_NAMED(sectionName)

#endif


//////////////////////////////////////////////////////////////////////////
// CrySystem DLL Exports.
//////////////////////////////////////////////////////////////////////////
typedef ISystem* (*PFNCREATESYSTEMINTERFACE)( SSystemInitParams &initParams );


//////////////////////////////////////////////////////////////////////////
// Global environment variable.
//////////////////////////////////////////////////////////////////////////
#if defined(SYS_ENV_AS_STRUCT)
	extern SSystemGlobalEnvironment gEnv;
#else
	extern SC_API DLL_EXPORT SSystemGlobalEnvironment* gEnv;
#endif

// Summary:
//	 Gets the system interface.
inline ISystem *GetISystem()
{
	return gEnv->pSystem;
};

#if defined(MAP_LOADING_SLICING)
// Summary:
//	 Gets the system scheduler interface.
inline ISystemScheduler* GetISystemScheduler(void)
{
	return gEnv->pSystemScheduler;
};
#endif // defined(MAP_LOADING_SLICING)
//////////////////////////////////////////////////////////////////////////

// Description:
//	 This function must be called once by each module at the beginning, to setup global pointers.
extern "C" DLL_EXPORT void ModuleInitISystem( ISystem *pSystem,const char *moduleName );
extern bool g_bProfilerEnabled;
extern int g_iTraceAllocations;

// Summary:
//	 Interface of the DLL.
extern "C" 
{
	CRYSYSTEM_API ISystem* CreateSystemInterface(const SSystemInitParams &initParams );
}

// Description:
//	 Displays error message.
//	 Logs it to console and file and error message box.
//	 Then terminates execution.
#if !defined (__SPU__)
void CryFatalError(const char *, ...) PRINTF_PARAMS(1, 2);
inline void CryFatalError( const char *format,... )
{ 
	if (!gEnv || !gEnv->pSystem)
		return;

	va_list	ArgList;
	char szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	int count = vsnprintf(szBuffer, sizeof(szBuffer), format, ArgList);
	if ( count == -1 || count >=sizeof(szBuffer) )
		szBuffer[sizeof(szBuffer)-1] = '\0';
	va_end(ArgList);

	gEnv->pSystem->FatalError( "%s",szBuffer );
}
#else 
#ifndef CryFatalError 
#define CryFatalError printf
#endif 
#endif 

//////////////////////////////////////////////////////////////////////////

// Description:
//	 Displays warning message.
//	 Logs it to console and file and display a warning message box.
//	 Doesn't terminate execution.
void CryWarning(EValidatorModule, EValidatorSeverity, const char *, ...) PRINTF_PARAMS(3, 4);
inline void CryWarning( EValidatorModule module,EValidatorSeverity severity,const char *format,... )
{
	if (!gEnv || !gEnv->pSystem || !format)
		return;

	va_list	args;
	va_start(args, format);
	GetISystem()->WarningV( module,severity,0,0,format,args );
	va_end(args);
}

#ifdef EXCLUDE_CVARHELP
	#define CVARHELP(_comment)	0
#else
	#define CVARHELP(_comment)	_comment
#endif

//Provide macros for fixing cvars for release mode on consoles to enums to allow for code stripping
//Do not enable for PC, apply VF_CHEAT there if required
#if defined(CONSOLE)
	#define CONST_CVAR_FLAGS (VF_CHEAT)
#else // #if (defined(XENON) || defined(PS3))
	#define CONST_CVAR_FLAGS (VF_NULL)
#endif // #if (defined(XENON) || defined(PS3))

#if defined(_RELEASE) && defined(CONSOLE)

	# define CONSOLE_CONST_CVAR_MODE
	# define DeclareConstIntCVar(name, defaultValue) enum { name = (defaultValue) }
	# define DeclareStaticConstIntCVar(name, defaultValue) enum { name = (defaultValue) }
	# define DefineConstIntCVarName(strname, name, defaultValue, flags, help) { COMPILE_TIME_ASSERT((int)(defaultValue) == (int)(name)); }
	# define DefineConstIntCVar(name, defaultValue, flags, help) { COMPILE_TIME_ASSERT((int)(defaultValue) == (int)(name));}
	# define DefineConstIntCVar2(name, _var_, defaultValue, flags, help)
	# define DefineConstIntCVar3(name, _var_, defaultValue, flags, help) \
		{ COMPILE_TIME_ASSERT((int)(defaultValue) == (int)(_var_));}
	# define AllocateConstIntCVar(scope,name)

	# define DefineConstFloatCVar(name, flags, help)
	# define DeclareConstFloatCVar(name)
	# define DeclareStaticConstFloatCVar(name)
	# define AllocateConstFloatCVar(scope,name)

#else

	# define DeclareConstIntCVar(name, defaultValue) int name
	# define DeclareStaticConstIntCVar(name, defaultValue) static int name
	# define DefineConstIntCVarName(strname, name, defaultValue, flags, help) \
		gEnv->pConsole->Register(strname, &name, defaultValue, flags|CONST_CVAR_FLAGS, CVARHELP(help))
	# define DefineConstIntCVar(name, defaultValue, flags, help) \
		gEnv->pConsole->Register((#name), &name, defaultValue, flags|CONST_CVAR_FLAGS, CVARHELP(help), 0, false)
	# define DefineConstIntCVar2(_name, _var, _def_val, _flags, help) \
		(gEnv->pConsole == 0 ? 0 : gEnv->pConsole->Register(_name, _var, (_def_val), (_flags)|CONST_CVAR_FLAGS, CVARHELP(help), 0, false))
	# define DefineConstIntCVar3(_name, _var, _def_val, _flags, help) \
	(gEnv->pConsole == 0 ? 0 : gEnv->pConsole->Register(_name, &(_var), (_def_val), (_flags)|CONST_CVAR_FLAGS, CVARHELP(help), 0, false))
	# define AllocateConstIntCVar(scope,name) int scope:: name

	# define DefineConstFloatCVar(name, flags, help) \
		gEnv->pConsole->Register((#name), &name, name ## Default, flags|CONST_CVAR_FLAGS, CVARHELP(help), 0, false)
	# define DeclareConstFloatCVar(name) float name
	# define DeclareStaticConstFloatCVar(name) static float name
	# define AllocateConstFloatCVar(scope,name) float scope:: name
#endif

#if defined(USE_CRY_ASSERT)
static void AssertConsoleExists(void)
{
	CRY_ASSERT(gEnv->pConsole != NULL);
}
#define ASSERT_CONSOLE_EXISTS AssertConsoleExists()
#else
#define ASSERT_CONSOLE_EXISTS 0
#endif // defined(USE_CRY_ASSERT)

// the following macros allow the help text to be easily stripped out

// Summary:
//	 Preferred way to register a CVar
#define REGISTER_CVAR(_var,_def_val,_flags,_comment)															(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register((#_var), &(_var), (_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Preferred way to register a CVar with a callback
#define REGISTER_CVAR_CB(_var,_def_val,_flags,_comment,_onchangefunction)					(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register((#_var), &(_var), (_def_val), (_flags), CVARHELP(_comment), _onchangefunction))
// Summary:
//	 Preferred way to register a string CVar
#define REGISTER_STRING(_name,_def_val,_flags,_comment)														(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterString(_name,(_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Preferred way to register a string CVar with a callback
#define REGISTER_STRING_CB(_name,_def_val,_flags,_comment,_onchangefunction)			(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterString(_name,(_def_val), (_flags), CVARHELP(_comment), _onchangefunction))
// Summary:
//	 Preferred way to register an int CVar
#define REGISTER_INT(_name,_def_val,_flags,_comment)															(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterInt(_name,(_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Preferred way to register an int CVar with a callback
#define REGISTER_INT_CB(_name,_def_val,_flags,_comment,_onchangefunction)					(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterInt(_name,(_def_val), (_flags), CVARHELP(_comment), _onchangefunction))
// Summary:
//	 Preferred way to register an int64 CVar
#define REGISTER_INT64(_name,_def_val,_flags,_comment)														(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterInt64(_name,(_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Preferred way to register a float CVar
#define REGISTER_FLOAT(_name,_def_val,_flags,_comment)														(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->RegisterFloat(_name,(_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Offers more flexibility but more code is required
#define REGISTER_CVAR2(_name,_var,_def_val,_flags,_comment)												(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register(_name, _var, (_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Offers more flexibility but more code is required
#define REGISTER_CVAR2_CB(_name,_var,_def_val,_flags,_comment,_onchangefunction)	(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register(_name, _var, (_def_val), (_flags), CVARHELP(_comment), _onchangefunction))
// Summary:
//	 Offers more flexibility but more code is required, explicit address taking of destination variable
#define REGISTER_CVAR3(_name,_var,_def_val,_flags,_comment)												(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register(_name, &(_var), (_def_val), (_flags), CVARHELP(_comment)))
// Summary:
//	 Offers more flexibility but more code is required, explicit address taking of destination variable
#define REGISTER_CVAR3_CB(_name,_var,_def_val,_flags,_comment,_onchangefunction)	(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->Register(_name, &(_var), (_def_val), (_flags), CVARHELP(_comment), _onchangefunction))
// Summary:
//	 Preferred way to register a console command
#define REGISTER_COMMAND(_name,_func,_flags,_comment)															(ASSERT_CONSOLE_EXISTS, gEnv->pConsole->AddCommand(_name,_func,(_flags), CVARHELP(_comment)))

////////////////////////////////////////////////////////////////////////////////
//
// Development only cvars
// 
// N.B:
// (1) Registered as real cvars *in non release builds only*.
// (2) Can still be manipulated in release by the mapped variable, so not the same as const cvars.
// (3) Any 'OnChanged' callback will need guarding against in release builds since the cvar won't exist
// (4) Any code that tries to get ICVar* will need guarding against in release builds since the cvar won't exist
//			 
// ILLEGAL_DEV_FLAGS is a mask of all those flags which make no sense in a _DEV_ONLY or _DEDI_ONLY cvar since the
// cvar potentially won't exist in a release build.
//
#define ILLEGAL_DEV_FLAGS (VF_NET_SYNCED | VF_CHEAT | VF_CHEAT_ALWAYS_CHECK | VF_CHEAT_NOCHECK | VF_READONLY | VF_CONST_CVAR)

#if defined(_RELEASE)
#define REGISTER_CVAR_DEV_ONLY(_var,_def_val,_flags,_comment)																NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); _var = _def_val
#define REGISTER_CVAR_CB_DEV_ONLY(_var,_def_val,_flags,_comment,_onchangefunction)					NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); _var = _def_val /* _onchangefunction consumed; callback not available */
#define REGISTER_STRING_DEV_ONLY(_name,_def_val,_flags,_comment)														NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_STRING_CB_DEV_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)				NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_INT_DEV_ONLY(_name,_def_val,_flags,_comment)																NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_INT_CB_DEV_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)					NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_INT64_DEV_ONLY(_name,_def_val,_flags,_comment)															NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_FLOAT_DEV_ONLY(_name,_def_val,_flags,_comment)															NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)									/* consumed; pure cvar not available */
#define REGISTER_CVAR2_DEV_ONLY(_name,_var,_def_val,_flags,_comment)												NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); *(_var) = _def_val
#define REGISTER_CVAR2_CB_DEV_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)		NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); *(_var) = _def_val
#define REGISTER_CVAR3_DEV_ONLY(_name,_var,_def_val,_flags,_comment)												NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); _var = _def_val
#define REGISTER_CVAR3_CB_DEV_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)		NULL; COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0); _var = _def_val
#define REGISTER_COMMAND_DEV_ONLY(_name,_func,_flags,_comment)													/* consumed; command not available */
#else
#define REGISTER_CVAR_DEV_ONLY(_var,_def_val,_flags,_comment)																REGISTER_CVAR(_var,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR_CB_DEV_ONLY(_var,_def_val,_flags,_comment,_onchangefunction)					REGISTER_CVAR_CB(_var,_def_val,((_flags) | VF_DEV_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_STRING_DEV_ONLY(_name,_def_val,_flags,_comment)														REGISTER_STRING(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_STRING_CB_DEV_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)				REGISTER_STRING_CB(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT_DEV_ONLY(_name,_def_val,_flags,_comment)																REGISTER_INT(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT_CB_DEV_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)					REGISTER_INT_CB(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT64_DEV_ONLY(_name,_def_val,_flags,_comment)															REGISTER_INT64(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_FLOAT_DEV_ONLY(_name,_def_val,_flags,_comment)															REGISTER_FLOAT(_name,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR2_DEV_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR2(_name,_var,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR2_CB_DEV_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)		REGISTER_CVAR2_CB(_name,_var,_def_val,((_flags) | VF_DEV_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR3_DEV_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR3(_name,_var,_def_val,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR3_CB_DEV_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)		REGISTER_CVAR3_CB(_name,_var,_def_val,((_flags) | VF_DEV_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_COMMAND_DEV_ONLY(_name,_func,_flags,_comment)															REGISTER_COMMAND(_name,_func,((_flags) | VF_DEV_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#endif // defined(_RELEASE)
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
// Dedicated server only cvars
// 
// N.B:
// (1) Registered as real cvars in all non release builds
// (2) Registered as real cvars in release on dedi servers only, otherwise treated as DEV_ONLY type cvars (see above)
// 
#if defined(_RELEASE) && defined(DEDICATED_SERVER)
#define REGISTER_CVAR_DEDI_ONLY(_var,_def_val,_flags,_comment)															REGISTER_CVAR(_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR_CB_DEDI_ONLY(_var,_def_val,_flags,_comment,_onchangefunction)					REGISTER_CVAR_CB(_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_STRING_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_STRING(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_STRING_CB_DEDI_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)			REGISTER_STRING_CB(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT_DEDI_ONLY(_name,_def_val,_flags,_comment)															REGISTER_INT(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT_CB_DEDI_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)					REGISTER_INT_CB(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_INT64_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_INT64(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_FLOAT_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_FLOAT(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR2_DEDI_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR2(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR2_CB_DEDI_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)	REGISTER_CVAR2_CB(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR3_DEDI_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR3(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_CVAR3_CB_DEDI_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)	REGISTER_CVAR3_CB(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#define REGISTER_COMMAND_DEDI_ONLY(_name,_func,_flags,_comment)															REGISTER_COMMAND(_name,_func,((_flags) | VF_DEDI_ONLY),_comment); COMPILE_TIME_ASSERT(((_flags) & ILLEGAL_DEV_FLAGS) == 0)
#else
#define REGISTER_CVAR_DEDI_ONLY(_var,_def_val,_flags,_comment)															REGISTER_CVAR_DEV_ONLY(_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_CVAR_CB_DEDI_ONLY(_var,_def_val,_flags,_comment,_onchangefunction)					REGISTER_CVAR_CB_DEV_ONLY(_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction)
#define REGISTER_STRING_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_STRING_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_STRING_CB_DEDI_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)			REGISTER_STRING_CB_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction)
#define REGISTER_INT_DEDI_ONLY(_name,_def_val,_flags,_comment)															REGISTER_INT_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_INT_CB_DEDI_ONLY(_name,_def_val,_flags,_comment,_onchangefunction)					REGISTER_INT_CB_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction)
#define REGISTER_INT64_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_INT64_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_FLOAT_DEDI_ONLY(_name,_def_val,_flags,_comment)														REGISTER_FLOAT_DEV_ONLY(_name,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_CVAR2_DEDI_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR2_DEV_ONLY(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_CVAR2_CB_DEDI_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)	REGISTER_CVAR2_CB_DEV_ONLY(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction)
#define REGISTER_CVAR3_DEDI_ONLY(_name,_var,_def_val,_flags,_comment)												REGISTER_CVAR3_DEV_ONLY(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment)
#define REGISTER_CVAR3_CB_DEDI_ONLY(_name,_var,_def_val,_flags,_comment,_onchangefunction)	REGISTER_CVAR3_CB_DEV_ONLY(_name,_var,_def_val,((_flags) | VF_DEDI_ONLY),_comment,_onchangefunction)
#define REGISTER_COMMAND_DEDI_ONLY(_name,_func,_flags,_comment)															REGISTER_COMMAND_DEV_ONLY(_name,_func,((_flags) | VF_DEDI_ONLY),_comment)	
#endif // defined(_RELEASE)
// 
////////////////////////////////////////////////////////////////////////////////

#ifdef EXCLUDE_NORMAL_LOG				// setting this removes a lot of logging to reduced code size (useful for consoles)

	#define CryLog(...) ((void)0)
	#define CryComment(...) ((void)0)
	#define CryLogAlways(...) ((void)0)

#else // EXCLUDE_NORMAL_LOG

// Summary:
//	 Simple logs of data with low verbosity.
void CryLog(const char *, ...) PRINTF_PARAMS(1, 2);
inline void CryLog( const char *format,... )
{
	// Fran: we need these guards for the testing framework to work
	if (gEnv && gEnv->pSystem && gEnv->pLog)		
	{
		va_list args;
		va_start(args,format);
		gEnv->pLog->LogV( ILog::eMessage,format,args );
		va_end(args);
	}
}
// Notes:
//	 Very rarely used log comment.
void CryComment(const char *, ...) PRINTF_PARAMS(1, 2);
inline void CryComment( const char *format,... )
{
	// Fran: we need these guards for the testing framework to work
	if (gEnv && gEnv->pSystem && gEnv->pLog)		
	{
		va_list args;
		va_start(args,format);
		gEnv->pLog->LogV( ILog::eComment,format,args );
		va_end(args);
	}
}
// Summary:
//	 Logs important data that must be printed regardless verbosity.
void CryLogAlways(const char *, ...) PRINTF_PARAMS(1, 2);
inline void CryLogAlways( const char *format,... )
{
	// log should not be used before system is ready
	// error before system init should be handled explicitly

	// Fran: we need these guards for the testing framework to work

	if (gEnv && gEnv->pSystem && gEnv->pLog)
	{
//		assert(gEnv);
//		assert(gEnv->pSystem);

		va_list args;
		va_start(args,format);
		gEnv->pLog->LogV( ILog::eAlways,format,args );
		va_end(args);
	}
}

#endif // EXCLUDE_NORMAL_LOG

/*****************************************************
ASYNC MEMCPY FUNCTIONS
*****************************************************/

// Complex delegation required because it is not really easy to 
// export a external standalone symbol like a memcpy function when 
// building with modules. Dll pay an extra indirection cost for calling this 
// function. 
#ifndef _LIB
# define CRY_ASYNC_MEMCPY_DELEGATE_TO_CRYSYSTEM
#endif 
#define CRY_ASYNC_MEMCPY_API extern "C" 

// Note sync variable will be incremented (in calling thread) before job starts
// and decremented when job finishes. Multiple async copies can therefore be
// tied to the same sync variable, therefore wait for completion with
// while(*sync) (yield());
#if defined(CRY_ASYNC_MEMCPY_DELEGATE_TO_CRYSYSTEM)
inline void cryAsyncMemcpy(
	void* dst
	, const void* src
	, size_t size
	, int nFlags
	, volatile int* sync)
{
	GetISystem()->AsyncMemcpy(dst, src, size, nFlags, sync);
}
# else
CRY_ASYNC_MEMCPY_API void cryAsyncMemcpy(
	void* dst
	, const void* src
	, size_t size
	, int nFlags
	, volatile int* sync);
#endif



//////////////////////////////////////////////////////////////////////////
// Additional headers.
//////////////////////////////////////////////////////////////////////////
#include <FrameProfiler.h>

#endif //_CRY_SYSTEM_H_

