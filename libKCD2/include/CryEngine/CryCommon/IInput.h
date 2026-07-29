#ifndef __IINPUT_H__
#define __IINPUT_H__

#ifdef CRYINPUT_EXPORTS
	#define CRYINPUT_API DLL_EXPORT
#else
	#define CRYINPUT_API DLL_IMPORT
#endif

#include <platform.h>
#include <CryName.h>

struct ISystem;

enum EModifierMask
{
	eMM_None				= 0,
	eMM_LCtrl				= (1 << 0),
	eMM_LShift			= (1 << 1),
	eMM_LAlt				= (1 << 2),
	eMM_LWin				= (1 << 3),
	eMM_RCtrl				= (1 << 4),
	eMM_RShift			= (1 << 5),
	eMM_RAlt				= (1 << 6),
	eMM_RWin				= (1 << 7),
	eMM_NumLock			= (1 << 8),
	eMM_CapsLock		= (1 << 9),
	eMM_ScrollLock	= (1 << 10),

	eMM_Ctrl				= (eMM_LCtrl | eMM_RCtrl),
	eMM_Shift				= (eMM_LShift | eMM_RShift),
	eMM_Alt					= (eMM_LAlt | eMM_RAlt),
	eMM_Win					= (eMM_LWin | eMM_RWin),
	eMM_Modifiers		= (eMM_Ctrl | eMM_Shift | eMM_Alt | eMM_Win),
	eMM_LockKeys		= (eMM_CapsLock | eMM_NumLock | eMM_ScrollLock)
};


enum EDeviceId
{
	eDI_Keyboard		= 0,
	eDI_Mouse				= 1,
	eDI_XI					= 2,
	eDI_Headmounted = 3,
	eDI_Unknown			= 0xff,
};

const static int32 EFF_INVALID_DEVICE_INDEX = -1;

enum EFFEffectId
{
	eFF_Rumble_Basic = 0,
	eFF_Rumble_Frame
};

//Summary:
//	 Input Event types.
enum EInputState
{
	eIS_Unknown		= 0,
	eIS_Pressed		= (1 << 0),
	eIS_Released	= (1 << 1),
	eIS_Down			= (1 << 2),
	eIS_Changed		= (1 << 3),
	eIS_UI				= (1 << 4),
};

enum EInputDeviceType
{
	eIDT_Keyboard,
	eIDT_Mouse,
	eIDT_Joystick,
	eIDT_Gamepad,
	eIDT_Headmounted,
};

enum EInputPlatformFlags
{
	eIPF_NONE													= 0,
	eIPF_SwapFrontEndForwardAndBack		= (1 << 0),
};

#define USE_CRY_NAME_FOR_KEY_NAMES 0

#if USE_CRY_NAME_FOR_KEY_NAMES



typedef CCryName TKeyName;

#else

struct TKeyName
{
	const char *key;

	TKeyName() { key = ""; }
	TKeyName( const char *_key ) { key = _key; };
	operator const char*() const { return key; };
	bool operator<( const TKeyName &n ) const { return stricmp(key,n.key) < 0; }
	bool operator>( const TKeyName &n ) const { return stricmp(key,n.key) > 0; }
	bool operator==( const TKeyName &n ) const { return stricmp(key,n.key) == 0; }
	bool operator!=( const TKeyName &n ) const { return stricmp(key,n.key) != 0; }
	bool operator<( const char *str ) const { return stricmp(key,str) < 0; }
	bool operator>( const char *str ) const { return stricmp(key,str) > 0; }
	bool operator==( const char *str ) const { return stricmp(key,str) == 0; }
	bool operator!=( const char *str ) const { return stricmp(key,str) != 0; }
	const char *c_str() const { return key; }
};
inline bool operator==( const char *str,const TKeyName &n ) { return n == str; }
inline bool operator!=( const char *str,const TKeyName &n ) { return n != str; }
inline bool operator<( const char *str,const TKeyName &n ) { return n < str; }
inline bool operator>( const char *str,const TKeyName &n ) { return n > str; }

#endif //USE_CRY_NAME_FOR_KEY_NAMES

#define KI_KEYBOARD_BASE	0
#define KI_MOUSE_BASE			256
#define KI_XINPUT_BASE		512
#define KI_PS3_BASE				1024
#define KI_HMD_BASE				2048
#define KI_SYS_BASE				4096

enum EKeyId
{
	eKI_Escape = KI_KEYBOARD_BASE,
	eKI_1,
	eKI_2,
	eKI_3,
	eKI_4,
	eKI_5,
	eKI_6,
	eKI_7,
	eKI_8,
	eKI_9,
	eKI_0,
	eKI_Minus,
	eKI_Equals,
	eKI_Backspace,
	eKI_Tab,
	eKI_Q,
	eKI_W,
	eKI_E,
	eKI_R,
	eKI_T,
	eKI_Y,
	eKI_U,
	eKI_I,
	eKI_O,
	eKI_P,
	eKI_LBracket,
	eKI_RBracket,
	eKI_Enter,
	eKI_LCtrl,
	eKI_A,
	eKI_S,
	eKI_D,
	eKI_F,
	eKI_G,
	eKI_H,
	eKI_J,
	eKI_K,
	eKI_L,
	eKI_Semicolon,
	eKI_Apostrophe,
	eKI_Tilde,
	eKI_LShift,
	eKI_Backslash,
	eKI_Z,
	eKI_X,
	eKI_C,
	eKI_V,
	eKI_B,
	eKI_N,
	eKI_M,
	eKI_Comma,
	eKI_Period,
	eKI_Slash,
	eKI_RShift,
	eKI_NP_Multiply,
	eKI_LAlt,
	eKI_Space,
	eKI_CapsLock,
	eKI_F1,
	eKI_F2,
	eKI_F3,
	eKI_F4,
	eKI_F5,
	eKI_F6,
	eKI_F7,
	eKI_F8,
	eKI_F9,
	eKI_F10,
	eKI_NumLock,
	eKI_ScrollLock,
	eKI_NP_7,
	eKI_NP_8,
	eKI_NP_9,
	eKI_NP_Substract,
	eKI_NP_4,
	eKI_NP_5,
	eKI_NP_6,
	eKI_NP_Add,
	eKI_NP_1,
	eKI_NP_2,
	eKI_NP_3,
	eKI_NP_0,
	eKI_F11,
	eKI_F12,
	eKI_F13,
	eKI_F14,
	eKI_F15,
	eKI_Colon,
	eKI_Underline,
	eKI_NP_Enter,
	eKI_RCtrl,
	eKI_NP_Period,
	eKI_NP_Divide,
	eKI_Print,
	eKI_RAlt,
	eKI_Pause,
	eKI_Home,
	eKI_Up,
	eKI_PgUp,
	eKI_Left,
	eKI_Right,
	eKI_End,
	eKI_Down,
	eKI_PgDn,
	eKI_Insert,
	eKI_Delete,
	eKI_LWin,
	eKI_RWin,
	eKI_Apps,
	eKI_OEM_102,

	// Mouse.
	eKI_Mouse1 = KI_MOUSE_BASE,
	eKI_Mouse2,
	eKI_Mouse3,
	eKI_Mouse4,
	eKI_Mouse5,
	eKI_Mouse6,
	eKI_Mouse7,
	eKI_Mouse8,
	eKI_MouseWheelUp, 
	eKI_MouseWheelDown,
	eKI_MouseX,
	eKI_MouseY,
	eKI_MouseZ,
	eKI_MouseLast,
	
	// X360 controller.
	eKI_XI_DPadUp = KI_XINPUT_BASE,
	eKI_XI_DPadDown,
	eKI_XI_DPadLeft,
	eKI_XI_DPadRight,
	eKI_XI_Start,
	eKI_XI_Back,
	eKI_XI_ThumbL,
	eKI_XI_ThumbR,
	eKI_XI_ShoulderL,
	eKI_XI_ShoulderR,
	eKI_XI_A,
	eKI_XI_B,
	eKI_XI_X,
	eKI_XI_Y,
	eKI_XI_TriggerL,
	eKI_XI_TriggerR,
	eKI_XI_ThumbLX,
	eKI_XI_ThumbLY,
	eKI_XI_ThumbLUp,
	eKI_XI_ThumbLDown,
	eKI_XI_ThumbLLeft,
	eKI_XI_ThumbLRight,
	eKI_XI_ThumbRX,
	eKI_XI_ThumbRY,
	eKI_XI_ThumbRUp,
	eKI_XI_ThumbRDown,
	eKI_XI_ThumbRLeft,
	eKI_XI_ThumbRRight,
	eKI_XI_TriggerLBtn,
	eKI_XI_TriggerRBtn,
	eKI_XI_Connect,			// should be deprecated because all devices can be connected, use eKI_SYS_ConnectDevice instead
	eKI_XI_Disconnect,	// should be deprecated because all devices can be disconnected, use eKI_SYS_DisconnectDevice instead

	// PS3 controller.
	eKI_PS3_Select = KI_PS3_BASE,
	eKI_PS3_L3,
	eKI_PS3_R3,
	eKI_PS3_Start,
	eKI_PS3_Up,
	eKI_PS3_Right,
	eKI_PS3_Down,
	eKI_PS3_Left,
	eKI_PS3_L2,
	eKI_PS3_R2,
	eKI_PS3_L1,
	eKI_PS3_R1,
	eKI_PS3_Triangle,
	eKI_PS3_Circle,
	eKI_PS3_Cross,
	eKI_PS3_Square,
	eKI_PS3_StickLX,
	eKI_PS3_StickLY,
	eKI_PS3_StickRX,
	eKI_PS3_StickRY,
	eKI_PS3_RotX,
	eKI_PS3_RotY,
	eKI_PS3_RotZ,
	eKI_PS3_RotX_KeyL,
	eKI_PS3_RotX_KeyR,
	eKI_PS3_RotZ_KeyD,
	eKI_PS3_RotZ_KeyU,

	// Orbis specific
	eKI_Orbis_Touch,

	// Head mounted devices (Oculus Rift, Cinemizer, etc...)
	eKI_HMD_Pitch=KI_HMD_BASE,
	eKI_HMD_Yaw,
	eKI_HMD_Roll,
	eKI_HMD_Last, // Must remain the last symbol of HMD devices.

	// Normal inputs should be added above
	// eKI_SYS_COMMIT and below will be ignored by input blocking functionality
	eKI_SYS_Commit = KI_SYS_BASE,
	eKI_SYS_ConnectDevice,
	eKI_SYS_DisconnectDevice,

	// Terminator.
	eKI_Unknown		= 0xffffffff,
};

struct STouchEvent
{
	EDeviceId	deviceId;
	uint8			deviceIndex;
	Vec2			pos;
	uint8			id;
};

struct ITouchEventListener
{
	virtual ~ITouchEventListener() {}
	virtual void OnTouchEvent(const STouchEvent& event) = 0;
};

struct SInputSymbol;
// Description:
//	 InputEvents are generated by input system and dispatched to all event listeners.
struct SInputEvent
{
	EDeviceId		deviceId;		// Which device did the event originate from.
	EInputState	state;				// Type of input event.
	wchar_t			inputChar;	// If event is UI (Contains translated input char)
	TKeyName		keyName;		// Human readable name of the event.
	EKeyId			keyId;			// Device-specific id corresponding to the event.
	int					modifiers;	// Key modifiers enabled at the time of this event.
	float				value;		// Value associated with the event.
	SInputSymbol*	pSymbol;		// Input symbol the event originated from.
	uint8			  deviceIndex;	//Controller index

	SInputEvent()
	{
		deviceId	= eDI_Unknown;
		state			= eIS_Unknown;
		keyId			= eKI_Unknown;
		modifiers	= eMM_None;
		value			= 0;
		keyName   = "";
		pSymbol		= 0;
		deviceIndex = 0;
	}
	void GetMemoryUsage(ICrySizer *pSizer ) const{}
};

// Description:
// SFFTriggerOutputData are force feedback signals send to an input controller's triggers.
struct SFFTriggerOutputData
{
	struct Initial
	{
		enum Value
		{
			ZeroIt = 0,
			Default
		};
	};

	struct Flags
	{
		enum Value
		{
			LeftTouchToActivate		= 1 << 0,
			RightTouchToActivate	= 1 << 1,
		};
	};

	float  leftGain, rightGain;
	uint16 leftEnv, rightEnv;
	uint32 flags; 

	// -------------------------------------------------------------------------
	SFFTriggerOutputData() 	{ Init(Initial::ZeroIt); }
	SFFTriggerOutputData(Initial::Value v) 	{ Init(v); }
	SFFTriggerOutputData(bool leftTouchToActivate, bool rightTouchToActivate, float lTrigger, float rTrigger, uint16 lTriggerEnv, uint16 rTriggerEnv) :
		leftGain(lTrigger), rightGain(rTrigger), leftEnv(lTriggerEnv), rightEnv(rTriggerEnv)
	{
		SetFlag(Flags::LeftTouchToActivate, leftTouchToActivate);
		SetFlag(Flags::RightTouchToActivate, rightTouchToActivate);
	}

	// -------------------------------------------------------------------------
	void Init(Initial::Value v)
	{
		if (v == Initial::ZeroIt)
		{
			flags = 0;
			leftGain = rightGain = 0.f;
			leftEnv = rightEnv = 0;
		}
		else if (v == Initial::Default)
		{
			flags = 0;
			leftGain = rightGain = 1.f;
			leftEnv = rightEnv = 4;
		}
	}

	void operator += (const SFFTriggerOutputData& operand2)
	{
		leftGain += operand2.leftGain;
		rightGain += operand2.rightGain;
		// DARIO_TODO: check what to do with envelopes in this case
		leftEnv = max(leftEnv, operand2.leftEnv);
		rightEnv = max(rightEnv, operand2.rightEnv);
		// DARIO_TODO: check what to do with the touch required in this case
		SetFlag(Flags::LeftTouchToActivate, IsFlagEnabled(Flags::LeftTouchToActivate) || operand2.IsFlagEnabled(Flags::LeftTouchToActivate));
		SetFlag(Flags::RightTouchToActivate, IsFlagEnabled(Flags::RightTouchToActivate) || operand2.IsFlagEnabled(Flags::RightTouchToActivate));
	}

	ILINE float GetClampedLeftGain() const 	{ return clamp(leftGain, 0.0f, 1.0f); }
	ILINE float GetClampedRightGain() const { return clamp(rightGain, 0.0f, 1.0f); }

	bool IsFlagEnabled(Flags::Value f) const { return (flags & f) != 0; }

private:
	void EnableFlag(Flags::Value f) { flags |= f; }
	void DisableFlag(Flags::Value f) { flags &= ~f; }
	void SetFlag(Flags::Value f, bool b) { if (b) EnableFlag(f); else DisableFlag(f); }
	
};

// Description:
//	 SFFOutputEvents are force feedback signals send to an input controller.
struct SFFOutputEvent
{
	EDeviceId		deviceId;	// Which device will receive the event.
	EFFEffectId	eventId;
	float				amplifierS, amplifierA;
	float				timeInSeconds;

	SFFTriggerOutputData triggerData;

	SFFOutputEvent() : triggerData(SFFTriggerOutputData::Initial::Default)
	{
		deviceId	= eDI_Unknown;
		eventId	= eFF_Rumble_Basic;
		amplifierS = 1.0f;
		amplifierA = 1.0f;
		timeInSeconds = 0.3f;
	}
	
	SFFOutputEvent(EDeviceId id, EFFEffectId event, SFFTriggerOutputData::Initial::Value triggerInitValue, float time = 1.0f, float ampA = 1.0f, float ampB = 1.0f ) :
		deviceId(id), eventId(event), timeInSeconds(time), amplifierS(ampA), amplifierA(ampB),
		triggerData(triggerInitValue)
	{}

	SFFOutputEvent(EDeviceId id, EFFEffectId event, const SFFTriggerOutputData & triggerData, float time = 1.0f, float ampA = 1.0f, float ampB = 1.0f ) :
		deviceId(id), eventId(event), timeInSeconds(time), amplifierS(ampA), amplifierA(ampB),
		triggerData(triggerData)
	{}
};

struct SInputSymbol
{
	// Summary:
	//	 Input symbol types.
	enum EType
	{
		Button,		// state == press/hold/release -- value = 0, 1
		Toggle,		// transition state with a press.
		RawAxis,	// state == change -- value = movement of axis.
		Axis,		// state == change -- value = -1.0 to 1.0
		Trigger,	// state == change -- value = 0.0 to 1.0
	};

	SInputSymbol(uint32 devSpecId_, EKeyId keyId_, const TKeyName& name_, EType type_, uint32 user_ = 0)
		: devSpecId(devSpecId_)
		,	keyId(keyId_), name(name_)
		, state(eIS_Unknown)
		, type(type_)
		, value(0.0f)
		, user(user_)
		, deviceIndex(0)
	{
	}

	void PressEvent(bool pressed)
	{
		if (pressed)
		{
			state = eIS_Pressed;
			value = 1.0f;
		}
		else
		{
			state = eIS_Released;
			value = 0.0f;
		}
	}
	void ChangeEvent(float v)
	{
		state = eIS_Changed;
		value = v;
	}
	void AssignTo(SInputEvent& event, int modifiers=0)
	{
		event.pSymbol = this;
		event.deviceId= deviceId;
		event.modifiers= modifiers;
		event.state		= state;
		event.value		= value;
		event.keyName = name;
		event.keyId		= keyId;
		//event.deviceIndex = deviceIndex; //symbol does not know device index, but needs to cache it for hold events
	}

	const EKeyId		keyId;				// External id for fast comparison.
	const TKeyName	name;					// Human readable name of the event.
	const uint32		devSpecId;		// Device internal id of this symbol (we will use it to look it up).
	EInputState			state;				// Current state.
	const EType			type;					// Type of this symbol.
	float						value;				// Current value.
	uint32					user;					// Type dependent value (toggle-mask for toggles).
	EDeviceId				deviceId;			// Which device does the symbol belong to.
	uint8						deviceIndex;	// Device index - controller 1/2 etc
};

typedef std::vector<SInputSymbol*> TInputSymbols;
struct SInputBlockData
{
	// Summary:
	//	 Data used to block input symbol from firing event if matched

	SInputBlockData(const EKeyId keyId_, 
									const float fBlockDuration_, 
									const bool bAllDeviceIndices_ = true, 
									const uint8 deviceIndex_ = 0)
		: keyId(keyId_)
		, fBlockDuration(fBlockDuration_)
		, bAllDeviceIndices(bAllDeviceIndices_)
		, deviceIndex(deviceIndex_)
	{
	}

	float						fBlockDuration;			// How long will still be blocked for
	const EKeyId		keyId;							// External id for fast comparison.
	uint8						deviceIndex;				// Device index - controller 1/2 etc
	bool						bAllDeviceIndices;	// True to block all device indices of deviceID type, otherwise uses deviceIndex

};

// Description:
//	 Input event listeners registered to input system and receive input events when they are generated.
struct IInputEventListener
{
	// <interfuscator:shuffle>
	virtual ~IInputEventListener(){}
	// Summary:
	//	 Called every time input event is generated.
	// Return Value:
	//	 If it returns True then the broadcasting of this event should be aborted and the rest of input 
	//	 listeners should not receive this event.
	virtual bool OnInputEvent( const SInputEvent &event ) = 0;
	virtual bool OnInputEventUI( const SInputEvent &event ) {	return false;	}

	// Summary:
	//	 Used to sort the listeners based on priority
	// Return Value:
	//	 It returns priority associated with the listener (Higher the priority, the earlier
	//	 it will be processed relative to other listeners, default = 0)
	virtual int GetPriority() const { return 0; }
	// </interfuscator:shuffle>
};

struct IFFParams
{
	EDeviceId deviceId;
	EFFEffectId effectId;
	float strengthA, strengthB;
	float timeInSeconds;
	SFFTriggerOutputData triggerData;

	IFFParams() : strengthA(0), strengthB(0), timeInSeconds(0), triggerData(SFFTriggerOutputData::Initial::ZeroIt)
	{
		effectId = eFF_Rumble_Basic;
		deviceId = eDI_Unknown;
	}
};

//////////////////////////////////////////////////////////////////////////
// KINECT

#define KIN_SKELETON_COUNT 6
#define KIN_SKELETON_INVALID_TRACKING_ID KIN_SKELETON_COUNT

enum KIN_SKELETON_TRACKING_STATE
{
	KIN_SKELETON_NOT_TRACKED = 0,
	KIN_SKELETON_POSITION_ONLY,
	KIN_SKELETON_TRACKED
};

enum KIN_SKELETON_POSITION_INDEX
{
	KIN_SKELETON_POSITION_HIP_CENTER = 0,
	KIN_SKELETON_POSITION_SPINE,
	KIN_SKELETON_POSITION_SHOULDER_CENTER,
	KIN_SKELETON_POSITION_HEAD,
	KIN_SKELETON_POSITION_SHOULDER_LEFT,
	KIN_SKELETON_POSITION_ELBOW_LEFT,
	KIN_SKELETON_POSITION_WRIST_LEFT,
	KIN_SKELETON_POSITION_HAND_LEFT,
	KIN_SKELETON_POSITION_SHOULDER_RIGHT,
	KIN_SKELETON_POSITION_ELBOW_RIGHT,
	KIN_SKELETON_POSITION_WRIST_RIGHT,
	KIN_SKELETON_POSITION_HAND_RIGHT,
	KIN_SKELETON_POSITION_HIP_LEFT,
	KIN_SKELETON_POSITION_KNEE_LEFT,
	KIN_SKELETON_POSITION_ANKLE_LEFT,
	KIN_SKELETON_POSITION_FOOT_LEFT,
	KIN_SKELETON_POSITION_HIP_RIGHT,
	KIN_SKELETON_POSITION_KNEE_RIGHT,
	KIN_SKELETON_POSITION_ANKLE_RIGHT,
	KIN_SKELETON_POSITION_FOOT_RIGHT,
	KIN_SKELETON_POSITION_HAND_REFINED_LEFT,
	KIN_SKELETON_POSITION_HAND_REFINED_RIGHT,
	KIN_SKELETON_POSITION_COUNT
};

#define KIN_IDENTITY_MAX_ENROLLMENT_COUNT 8

enum KIN_IDENTITY_ENROLLMENT
{
	KIN_IDENTITY_ENROLLMENT_INDEX_CALL_IDENTIFY = 0xFFFFFFFF,
	KIN_IDENTITY_ENROLLMENT_INDEX_UNKNOWN       = 0xFFFFFFFE,
	KIN_IDENTITY_ENROLLMENT_INDEX_BUSY          = 0xFFFFFFFC,
	KIN_IDENTITY_ENROLLMENT_INDEX_FAILURE       = 0xFFFFFFFB,
};


enum KIN_SKELETON_POSITION_TRACKING_STATE
{
	KIN_SKELETON_POSITION_NOT_TRACKED = 0,
	KIN_SKELETON_POSITION_INFERRED,
	KIN_SKELETON_POSITION_TRACKED
};

struct SHandPositions
{
	Vec3 vLeftHand; 
	Vec3 vRightHand; 
};

struct SKinSkeletonRawData
{
	KIN_SKELETON_TRACKING_STATE eTrackingState;
	DWORD dwTrackingId;
	DWORD dwEnrollmentIndex;
	DWORD dwUserIndex;
	SHandPositions screenHands; // x and y range from -1 to +1 - z from 0 to 1
	Vec4 vPosition;
	Vec4 vSkeletonPositions[KIN_SKELETON_POSITION_COUNT];

	KIN_SKELETON_POSITION_TRACKING_STATE eSkeletonPositionTrackingState[KIN_SKELETON_POSITION_COUNT];
	DWORD dwQualityFlags;
};

//Interface for different joint filtering types
class ISkeletonFilter
{
public:
	// <interfuscator:shuffle>
	// default virtual destructor
	virtual ~ISkeletonFilter() {}

	//Resets all internal state of the filter
	virtual void Reset() = 0;

	//Apply smoothing to current skeleton data
	virtual void Update( const SKinSkeletonRawData& pSkeletonData, const float fDeltaTime ) = 0;

	//Retrieve smoothed/filtered joints
	virtual Vec4* GetFilteredJoints() = 0;
	// </interfuscator:shuffle>
};

struct SKinSkeletonDefaultData
{
	//Alignment data updated each time skeleton is detected
	Vec4 vPosition;
	Vec4 vSkeletonPositions[KIN_SKELETON_POSITION_COUNT];
};

struct SKinSkeletonFrame
{
	int64 liTimeStamp;
	DWORD dwFrameNumber;
	DWORD dwFlags;
	Vec4 vFloorClipPlane;
	Vec4 vNormalToGravity;
	SKinSkeletonRawData skeletonRawData[KIN_SKELETON_COUNT];

	// add extra skeleton frame data after this point, all data
	// before needs to be reflected in the Xbox Kinect sync app!
	SKinSkeletonDefaultData skeletonDefaultData[KIN_SKELETON_COUNT];
};


enum KIN_GRIP_TYPE
{
	KIN_GRIP_ANALOG_STICK,
	KIN_GRIP_ARC,
	KIN_GRIP_GEARSHIFT,
	KIN_GRIP_HOVERTIME,
	KIN_GRIP_INVALID
};

struct SKinRailStateAnalogStick
{
	float fAngle; //Radians
	float fDistance;

	SKinRailStateAnalogStick():fAngle(0.f), fDistance(0.f){}
};

struct SKinRailStateArc
{
	float fProgress; // Range 0 - 1
	float fDenormalizedProgress; // progress in radians 0 - pi

	SKinRailStateArc():fProgress(0.f), fDenormalizedProgress(0.f){}
};

struct SKinRailStateGearShift
{
	int nIndex;
	float fToothProgress;
	float fSpineProgress;
	float fSpineProgressAtToothCommit;

	SKinRailStateGearShift():nIndex(0), fToothProgress(0.f), fSpineProgress(0.f), fSpineProgressAtToothCommit(0.f){}
};

struct SKinRailStateHoverTime
{
	float fTimePassed;
	float fPercentageComplete;

	SKinRailStateHoverTime():fTimePassed(0.f), fPercentageComplete(0.f){}
};

struct SKinRailState
{
	bool bAttached; // True if playes is going over this grip
	bool bCommited; // True if the player has committed in going on a certain rail (check the rail id)
	int nGripId;
	int nRailId;
	KIN_GRIP_TYPE eGripType; // Use this enum to see which struct you need to check. Other data will be invalid
	SKinRailStateAnalogStick analogState;
	SKinRailStateArc arcState;
	SKinRailStateGearShift gearShiftState;
	SKinRailStateHoverTime hovertimeState;

	SKinRailState():bAttached(false), bCommited(false), nGripId(-1), nRailId(-1), eGripType(KIN_GRIP_INVALID){}
};


struct SKinGripShape
{
	float fFastAttachRadius;
	float fSlowAttachRadius;
	float fApproachRadius;
	float fSlowDetachRadius;
};


struct SKinBodyShapeHandles
{
	Vec3 vOrigin; // origin of the PHIZ space
	Vec3 vForward; // Forward vector in the body space as it's positioned in cam space
	Vec3 vRight; // Right vector in the the body space as it's positioned in cam space
	Vec3 vUp; // Up vector in the the body space as it's positioned in cam space
	Vec3 vRightShoulder; // Position of right shoulder in camera space
	Vec3 vLeftShoulder; // Position of left shoulder in camera space
	Vec3 vRefinedRightHand; // Position of the refined right-hand position in camera space
	Vec3 vRefinedLeftHand; // Position of the refined left-hand position in camera space
};

enum EKinIdentityMessageId
{
	E_KinIdentityMessageId_Frame_Processed, // This message-type identifier is sent every time the system has completed processing a single frame, during the identification or enrollment process
	E_KinIdentityMessageId_Id_Complete, // This message-type identifier is sent when the entire identification or enrollment process is complete
};

enum EKinIdentityOperationId
{
	E_KinIdentityOperationId_None, // A no-op or an invalid value. The identity engine should never generate a message with this value. 
	E_KinIdentityOperationId_Identify, // A pending IdentityIdentify operation
	E_KinIdentityOperationId_Enroll, // A pending IdentityEnroll operation
	E_KinIdentityOperationId_Tuner, // This value is available only if the skeleton tracking part of the Kinect tuner is tested when you captured a XED file
};

enum EKinIdentityQuality
{
	EKinIdentityQuality_FACE_DETECT_FAILURE			= 0x00000001, 
	EKinIdentityQuality_USER_BODY_TURNED				= 0x00000002,
	EKinIdentityQuality_USER_NOT_UPRIGHT				= 0x00000004,
	EKinIdentityQuality_USER_OCCLUDED_FACE			= 0x00000008,
	EKinIdentityQuality_USER_OCCLUDED_BODY			= 0x00000010,
	EKinIdentityQuality_USER_FAR_AWAY						= 0x00000020,
	EKinIdentityQuality_USER_CLOSE							= 0x00000040,
	EKinIdentityQuality_USER_CLIPPED_AT_LEFT		= 0x00000080,
	EKinIdentityQuality_USER_CLIPPED_AT_RIGHT		= 0x00000100,
	EKinIdentityQuality_USER_CLIPPED_AT_TOP			= 0x00000200,
	EKinIdentityQuality_USER_CLIPPED_AT_BOTTOM	= 0x00000400,
	EKinIdentityQuality_ENVIRONMENT_TOO_DARK		= 0x00000800,
	EKinIdentityQuality_ENVIRONMENT_TOO_BRIGHT	= 0x00001000,
};

struct SKinIdentityMessageFrameProcessed
{
	DWORD dwQualityFlags; // EKinIdentityQuality flags
};

enum EKinIdentityMessageCompleteResult
{
	EKinIdentityMessageCompleteResult_Ok,
	EKinIdentityMessageCompleteResult_Busy,
	EKinIdentityMessageCompleteResult_LostTrack,
	EKinIdentityMessageCompleteResult_QualityIssue,
	EKinIdentityMessageCompleteResult_Abort,
};

struct SKinIdentityMessageComplete
{
	EKinIdentityMessageCompleteResult eResult;
	DWORD dwEnrollementIndex;
	bool bProfileMatched;
};

struct SKinIdentityMessage
{
	EKinIdentityMessageId messageId;
	EKinIdentityOperationId operationId;
	DWORD dwTrackingId;
	DWORD dwSkeletonFrameNumber;
	union
	{
		SKinIdentityMessageFrameProcessed frameProcessed;
		SKinIdentityMessageComplete complete;
	}Data;
};

typedef bool(*KinIdentifyCallback)(const SKinIdentityMessage& message, void* pData);

typedef void* KIN_SPEECH_HANDLE;

enum KIN_SPEECH_CONFIDENCE
{
	KIN_SPEECH_CONFIDENCE_LOW,
	KIN_SPEECH_CONFIDENCE_NORMAL,
	KIN_SPEECH_CONFIDENCE_HIGHT,
};

#define KIN_SPEECH_EVENT_START_STREAM        0x00000001
#define KIN_SPEECH_EVENT_END_STREAM          0x00000002
#define KIN_SPEECH_EVENT_SOUND_START         0x00000004
#define KIN_SPEECH_EVENT_SOUND_END           0x00000008
#define KIN_SPEECH_EVENT_PHRASE_START        0x00000010
#define KIN_SPEECH_EVENT_HYPOTHESIS          0x00000020
#define KIN_SPEECH_EVENT_RECOGNITION         0x00000040
#define KIN_SPEECH_EVENT_FALSE_RECOGNITION   0x00000080
#define KIN_SPEECH_EVENT_INTERFERENCE        0x00000100
#define KIN_SPEECH_EVENT_BOOKMARK            0x00000200
#define KIN_SPEECH_EVENT_RETAINEDAUDIO       0x00000400

#define KIN_SPEECH_ALL_RECOGNITION_EVENTS   (KIN_SPEECH_EVENT_HYPOTHESIS | KIN_SPEECH_EVENT_RECOGNITION | KIN_SPEECH_EVENT_FALSE_RECOGNITION)
#define KIN_SPEECH_ALL_EVENTS 0xFFFFFFFF

/*struct KIN_SPEECH_INIT_PROPERTIES
{
	KIN_SPEECH_LANGUAGE Language;
	KIN_SPEECH_MICTYPE Microp
};*/

struct KIN_SPEECH_ELEMENT
{
	unsigned long ulAudioTimeOffset;
	unsigned long ulAudioSizeTime;
	unsigned long ulAudioStreamOffset;
	unsigned long ulAudioSizeBytes;
	unsigned long ulRetainedStreamOffset;
	unsigned long ulRetainedSizeBytes;
	wchar_t* pcwszDisplayText;
	wchar_t* pcwszLexicalForm;
	wchar_t* pcwszPronunciation;
	KIN_SPEECH_CONFIDENCE eRequiredConfidence;
	KIN_SPEECH_CONFIDENCE eActualConfidence;
	float fSREngineConfidence;
};

struct KIN_SPEECH_GRAMMAR
{
	unsigned long ulId;
	KIN_SPEECH_HANDLE hGrammar;
};

struct IKinectInputAudioListener;
struct IKinectInputListener;

struct IKinectInput
{
	// <interfuscator:shuffle>
	virtual ~IKinectInput(){};

	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool IsEnabled() = 0;
#ifndef DURANGO
	virtual void RegisterInputListener(IKinectInputListener* pInputListener, const char* name) = 0;
	virtual void UnregisterInputListener(IKinectInputListener* pInputListener) = 0;
#else
	virtual void RegisterKinectAudioListener(IKinectInputAudioListener* pInputListener, const char* name) = 0;
	virtual void UnregisterKinectAudioListener(IKinectInputAudioListener* pInputListener) = 0;
	virtual bool IsGrammarInitialized() const = 0;
#endif

virtual const char* GetUserStatusMessage() = 0;
	// Summary:
	//		Adds an Arc rail - The most common used way for menu navigation
	// Return value:
	//		Returns true if the rail is successfully added
	virtual bool RegisterArcRail(int gripId, int railId, const Vec2& vScreenPos, const Vec3& vDir, float fLenght, float fDeadzoneLength, float fToleranceConeAngle) = 0;

	virtual void UnregisterArcRail(int gripId) = 0;

	// Summary:
	//		Adds a Hover time item. fTimeToCommit is the time it from when the player is hovering over the item up until it starts counting
	// Return value:
	//		Returns true if the rail is successfully added
	virtual bool RegisterHoverTimeRail(int gripId, int railId, const Vec2& vScreenPos, float fHoverTime, float fTimeTillCommit, SKinGripShape* pGripShape = NULL) = 0;

	// Summary:
	//	Unregister a specific grip. You can only remove the full grip, not one rail at a time
	virtual void UnregisterHoverTimeRail(int gripId) = 0;

	// Summary:
	//		Clears all rails
	virtual void UnregisterAllRails() = 0;

	// Summary:
	//		Describes the body space coordinate system relative to camera space
	virtual bool GetBodySpaceHandles(SKinBodyShapeHandles& bodyShapeHandles) = 0;

	// Summary;:
	//		Enables/Disables seated skeleton tracking
	virtual void EnableSeatedSkeletonTracking(bool bValue) = 0;

	// Summary;:
	//		Get raw skeleton data
	virtual bool GetSkeletonRawData(uint32 iUser, SKinSkeletonRawData &skeletonRawData) const = 0;

	// Summary;:
	//		Get default skeleton data
	virtual bool GetSkeletonDefaultData(uint32 iUser, SKinSkeletonDefaultData &skeletonDefaultData) const = 0;

	// Summary;:
	//		Get the skeleton that is closest to the camera
	virtual uint32 GetClosestTrackedSkeleton() const = 0;

	// Summary:
	//		Draws skeleton, depth & image buffer info on screen
	virtual void DebugDraw() = 0;

	// Summary:
	//		Enable wave gesture tracking
	virtual void EnableWaveGestureTracking(bool bEnable) = 0;

	// Summary:
	//		Get wave progress
	virtual float GetWaveGestureProgress(DWORD* pTrackingId) = 0;

	// Summary:
	//		Detected intent to play
	virtual bool IdentityDetectedIntentToPlay(DWORD dwTrackingId) = 0; 

	// Summary:
	//		Call this function to start identifying a player
	virtual bool IdentityIdentify(DWORD dwTrackingId, KinIdentifyCallback callbackFunc, void* pData) = 0;

	// Summary:
	//		Enable the Kinect speech features
	virtual bool SpeechEnable() = 0;

	// Summary:
	//		Disable the Kinect speech features
	virtual void SpeechDisable() = 0;

	// Summary:
	//		Enables / Disables a particular grammar
	virtual void SetEnableGrammar(const string & grammarName, bool enable) = 0;

	// Summary:
	//		Set the kinect speech event interests
	virtual bool KinSpeechSetEventInterest(unsigned long ulEvents) = 0;

	// Summary:
	//		Load the default grammar
	virtual bool KinSpeechLoadDefaultGrammar() = 0;

	// Summary:
	//		Start voice recognition
	virtual bool KinSpeechStartRecognition() = 0;

	// Summary:
	//		Stop voice recognition
	virtual void KinSpeechStopRecognition() = 0;

	// Summary:
	//		Updates stored alignment data to current skeletal data
	virtual void UpdateSkeletonAlignment(uint32 skeletonIndex){}
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////
struct IKinectInputListener
{
	// <interfuscator:shuffle>
	virtual ~IKinectInputListener() {}
	virtual bool OnKinectRawInputEvent(const SKinSkeletonFrame& skeletonFrame) = 0;
	virtual void OnRailProgress(const SKinRailState& railState) = 0;
	virtual void OnVoiceCommand(string voiceCommand) = 0;
	virtual void OnKinectClosestSkeletonChanged(uint32 skeletonIndex) = 0;
	virtual void OnKinectSkeletonMoved(uint32 skeletonIndex, float distanceMoved) = 0;
	// </interfuscator:shuffle>
};

struct IKinectInputAudioListener
{
	enum eGrammaChange
	{
		eGC_Enabled = 0,
		eGC_Disabled,
		eGC_NotLoaded
	};

	virtual ~IKinectInputAudioListener() {}
	virtual void OnVoiceCommand(const char * voiceCommand) = 0;
	virtual void OnGrammarChange(const char * grammarName, eGrammaChange change) = 0;
};
	
//////////////////////////////////////////////////////////////////////////
///Natural Point Device Interfaces

//////////////////////////////////////////////////////////////////////////
struct NP_RawData
{
	float fNPRoll;
	float fNPPitch;
	float fNPYaw;

	float fNPX;
	float fNPY;
	float fNPZ;
};

//////////////////////////////////////////////////////////////////////////
struct INaturalPointInput
{
	// <interfuscator:shuffle>
	virtual ~INaturalPointInput(){};

	virtual bool Init() = 0;
	virtual void Update() = 0;
	virtual bool IsEnabled() = 0;

	virtual void Recenter() = 0;

	// Summary;:
	//		Get raw skeleton data
	virtual bool GetNaturalPointData(NP_RawData &npRawData) const = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////

struct IInputDevice
{
	// <interfuscator:shuffle>
	// Summary:
	//	 Implements virtual destructor just for safety.
	virtual ~IInputDevice(){}

	virtual const char* GetDeviceName() const	= 0;
	virtual EDeviceId GetDeviceId() const	= 0;
	virtual int GetDeviceIndex() const = 0;
	// Initialization.
	virtual bool	Init() = 0;
	virtual void  PostInit() = 0;
	// Update.
	virtual void	Update(bool bFocus) = 0;
	// Summary:
	//	 Sets force feedback .
	// Return Value:
	//	 True if successful.
	virtual bool SetForceFeedback(IFFParams params) = 0;
	// Summary:
	//	 Checks for key pressed and held.
	virtual bool	InputState(const TKeyName& key, EInputState state) = 0;
	// Summary:
	//	 Sets/unsets DirectInput to exclusive mode.
	virtual bool	SetExclusiveMode(bool value) = 0;
	// Summary:
	//	 Clears the key (pressed) state.
	virtual void	ClearKeyState() = 0;
	// Summary:
	//	 Clears analog position state.
	virtual void	ClearAnalogKeyState(TInputSymbols& clearedSymbols) = 0;
	virtual const char* GetKeyName(const SInputEvent& event) const = 0;
	virtual const char* GetKeyName(const EKeyId keyId) const = 0;
	virtual char GetInputCharAscii(const SInputEvent& event) = 0;
	virtual const wchar_t *GetOSKeyName(const SInputEvent& event) = 0;
	virtual SInputSymbol* LookupSymbol(EKeyId id) const = 0;
	virtual const SInputSymbol*	GetSymbolByName(const char* name) const = 0;
	virtual bool IsOfDeviceType( EInputDeviceType type ) const = 0;
	virtual void Enable(bool enable) = 0;
	virtual bool IsEnabled() const = 0;
	virtual void OnLanguageChange() = 0;
	//! Dead zone settings for input devices where this is relevant (i.e. controllers with analog sticks)
	virtual void SetDeadZone(float fThreshold) = 0; // between 0 and 1
	virtual void RestoreDefaultDeadZone() = 0;
	// </interfuscator:shuffle>
};

// Description:
//	 Interface to the Input system.
//	 The input system give access and initialize Keyboard,Mouse and Joystick SubSystems.
// Summary:
//	 Main Input system interface.
struct IInput
{
	// <interfuscator:shuffle>
	virtual ~IInput(){}
	// Summary:
	//	 Registers new input events listener.
	virtual void AddEventListener( IInputEventListener *pListener ) = 0;
	virtual void RemoveEventListener( IInputEventListener *pListener ) = 0;

	// Description:
	//	 Registers new console input event listeners. console input listeners receive all events, no matter what.
	virtual void AddConsoleEventListener( IInputEventListener *pListener ) = 0;
	virtual void RemoveConsoleEventListener( IInputEventListener *pListener ) = 0;

	virtual bool AddTouchEventListener( ITouchEventListener *pListener, const char* name ) = 0;
	virtual void RemoveTouchEventListener( ITouchEventListener *pListener ) = 0;

	// Description:
	//	 Registers an exclusive listener which has the ability to filter out events before they arrive at the normal
	//	 listeners.
	virtual void SetExclusiveListener( IInputEventListener *pListener ) = 0;
	virtual IInputEventListener *GetExclusiveListener() = 0;

	virtual bool AddInputDevice(IInputDevice* pDevice) = 0;
	// Description:
	//	 Registers an exclusive listener which has the ability to filter out events before they arrive at the normal
	//	 listeners.
	virtual void EnableEventPosting ( bool bEnable ) = 0;
	virtual bool IsEventPostingEnabled () const = 0;
	virtual void PostInputEvent( const SInputEvent &event, bool bForce=false ) = 0;
	virtual void PostTouchEvent( const STouchEvent &event, bool bForce=false ) = 0;

	// Description:
	//	 For direct key processing (e.g. win proc functions)
	//   currently only used by durango metro launcher
	virtual void ProcessKey(uint32 key, bool pressed, wchar_t unicode, bool repeat) = 0;

	// Description:
	//	 Posts a force feedback / rumble output event.
	virtual void ForceFeedbackEvent( const SFFOutputEvent &event ) = 0;
	// Description:
	//	 Sets a filter so that only one device can output force feedback
	virtual void ForceFeedbackSetDeviceIndex( int index ) = 0;

	// Summary
	//	 Initializes input system.
	// Note:
	//	 Required params should be passed through constructor
	virtual bool	Init() = 0;
	// Summary
	//	 Post Initialization called at end of initialization
	virtual void  PostInit() = 0;
	// Description:
	//	 Updates Keyboard, Mouse and Joystick. Sets bFocus to true if window has focus and input is enabled.
	virtual void	Update(bool bFocus) = 0;
	// Summary:
	//	 Clears all subsystems.
	virtual void	ShutDown() = 0;

	// See also:
	//	 IInputDevice::SetExclusive
	virtual void	SetExclusiveMode(EDeviceId deviceId, bool exclusive,void *hwnd=0) = 0;

	// See also:
	//	 IInputDevice::InputState
	virtual bool	InputState(const TKeyName& key, EInputState state) = 0;

	// Description:
	//	 Converts an input event to the key name. The function should internally dispatch to all managed 
	//   input devices and return the first recognized event.
	// Arguments:
	//	 event - Input event to translate into a name.
	virtual const char* GetKeyName(const SInputEvent& event) const = 0;

	// Description:
	//	 Converts an input event to the key name. The function should internally dispatch to all managed 
	//   input devices and return the first recognized keyId.
	// Arguments:
	//	 keyId - Input keyId to translate into a name.
	// Return Value:
	//	 Translated key name
	virtual const char* GetKeyName(EKeyId keyId) const = 0;

	// Description:
	//	 Gets an input char translated to ascii from the event. The function should internally dispatch to all managed 
	//   input devices and return the first recognized event.
	// Arguments:
	//	 event - Input event to translate into a name.
	virtual char GetInputCharAscii(const SInputEvent& event) = 0;

	// Summary:
	//	 Lookups a symbol for a given symbol and key ids.
	virtual SInputSymbol* LookupSymbol( EDeviceId deviceId, int deviceIndex, EKeyId keyId ) = 0;

	// Summary:
	//	 Looks up a symbol for a key name
	virtual const SInputSymbol*	GetSymbolByName(const char* name) const = 0;

	// Summary:
	//	 Gets OS Keyname.
	// Arguments:
	//	 event - Input event to translate into a name.
	virtual const wchar_t* GetOSKeyName(const SInputEvent& event) = 0;

	// Summary:
	//	 Clears key states of all devices.
	virtual void ClearKeyState() = 0;

	// Summary:
	//	 Clears analog key states of all devices.
	virtual void ClearAnalogKeyState() = 0;

	// Summary:
	//	 Re-triggers pressed keys. 
	// Note:
	//	 Used for transitioning action maps.
	virtual void RetriggerKeyState() = 0;

	// Summary:
	//	 Gets if we are currently re-triggering. 
	// Note:
	//	 Needed to filter out actions.
	virtual bool Retriggering() = 0;

	// Description:
	//	 Queries to see if this machine has some kind of input device connected.
	virtual bool HasInputDeviceOfType( EInputDeviceType type ) = 0;

	// Summary:
	//	 Gets the currently pressed modifiers.
	virtual int GetModifiers() const = 0;

	// Summary:
	//	 Tells devices whether to report input or not.
	virtual void EnableDevice( EDeviceId deviceId, bool enable) = 0;

	//! Dead zone settings for input devices where this is relevant (i.e. controllers with analog sticks)
	virtual void SetDeadZone(float fThreshold) = 0; // between 0 and 1
	virtual void RestoreDefaultDeadZone() = 0;

	// Summary:
	//	 Returns EInputPlatformFlags
	virtual uint32 GetPlatformFlags() const = 0;

	// Summary:
	//	 Adds or updates blocking input if SInputBlockData::fBlockDuration exceeds previously blocking time
	// Return Value:
	//	 True if successfully added inputBlocker or updated existed input blocker's remaining blocking time
	virtual bool SetBlockingInput(const SInputBlockData& inputBlockData) = 0;

	// Summary:
	//	 Removes blocking input
	// Return Value:
	//	 True if found and removed, false otherwise
	virtual bool RemoveBlockingInput(const SInputBlockData& inputBlockData) = 0;

	// Summary:
	//	 Checks if the specified input is currently being blocked
	// Return Value:
	//	 True if specified input is currently being blocked, false otherwise
	virtual bool HasBlockingInput(const SInputBlockData& inputBlockData) const = 0;

	// Summary:
	//	 Gets the number of inputs currently being blocked
	// Return Value:
	//	 Number of inputs currently being blocked
	virtual int GetNumBlockingInputs() const = 0;

	// Summary:
	//	 Clears all the inputs being blocked
	// Return Value:
	//	 None
	virtual void ClearBlockingInputs() = 0;

	// Summary:
	//	 Checks if the input specified should be blocked
	// Return Value:
	//	 True if input specified should be blocked, false otherwise
	virtual bool ShouldBlockInputEventPosting(const EKeyId keyId, const EDeviceId	deviceId, const uint8 deviceIndex) const = 0;

	//////////////////////////////////////////////////////////////////////////
	// KINECT
	virtual IKinectInput* GetKinectInput() = 0;

	//////////////////////////////////////////////////////////////////////////
	// TrackIR
	virtual INaturalPointInput* GetNaturalPointInput() = 0;
	// </interfuscator:shuffle>
};



#ifdef __cplusplus
extern "C" {
#endif

typedef IInput  (*  CRY_PTRCREATEINPUTFNC(ISystem *pSystem, void* hwnd));

CRYINPUT_API IInput *CreateInput(ISystem *pSystem, void* hwnd);

#ifdef __cplusplus
};
#endif


#endif //__IINPUT_H__
