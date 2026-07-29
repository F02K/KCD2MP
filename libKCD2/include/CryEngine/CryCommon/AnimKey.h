////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001.
// -------------------------------------------------------------------------
//  File name:   animkey.cpp
//  Version:     v1.00
//  Created:     22/4/2002 by Timur.
//  Compilers:   Visual C++ 7.0
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __animkey_h__
#define __animkey_h__

#if _MSC_VER > 1000
#pragma once
#endif

#include <IConsole.h> // <> required for Interfuscator
#include <ICryPak.h> // <> required for Interfuscator

struct ISound;

enum EAnimKeyFlags
{
	//! This key is selected in track view.
	AKEY_SELECTED = 0x01,
};

//! Interface to animation key.
//! Not real interface though...
//! No virtuals for optimization reason.
struct IKey
{
	float time;
	int flags;

	// compare keys.
	bool operator<( const IKey &key ) const { return time < key.time; }
	bool operator==( const IKey &key ) const { return time == key.time; }
	bool operator>( const IKey &key ) const { return time > key.time; }
	bool operator<=( const IKey &key ) const { return time <= key.time; }
	bool operator>=( const IKey &key ) const { return time >= key.time; }
	bool operator!=( const IKey &key ) const { return time != key.time; }

protected:
	//! Protect from direct instantiation of this class.
	//! Only derived classes can be created,
	IKey() :time(0),flags(0) {};
};

/** I2DBezierKey used in float tracks.
		Its x component actually represents kinda time-warping curve.
*/
struct I2DBezierKey : public IKey
{
	Vec2 value;
};

/** ITcbKey used in all TCB tracks.
*/
struct ITcbKey : public IKey
{
	// Values.
	float fval[4];
	// Key controls.
	float tens;         //!< Key tension value.
  float cont;         //!< Key continuity value.
  float bias;         //!< Key bias value.
  float easeto;       //!< Key ease to value.
  float easefrom;     //!< Key ease from value.

	//! Protect from direct instantiation of this class.
	//! Only derived classes can be created,
	ITcbKey() {
		fval[0] = 0; fval[1] = 0; fval[2] = 0; fval[3] = 0;
		tens = 0, cont = 0, bias = 0, easeto = 0, easefrom = 0;
	};

	void SetFloat( float val ) { fval[0] = val;	};
	void SetVec3( const Vec3 &val ) 
	{ 
		fval[0] = val.x;			fval[1] = val.y;			fval[2] = val.z;
	};
	void SetQuat( const Quat &val ) 
	{ 
		fval[0] = val.v.x;	fval[1] = val.v.y;	fval[2] = val.v.z;	fval[3] = val.w;
	};

	ILINE void SetValue( float val )			 { SetFloat(val);	}
	ILINE void SetValue( const Vec3 &val ) { SetVec3(val); }
	ILINE void SetValue( const Quat &val ) { SetQuat(val); }

	float GetFloat() const { return *((float*)fval); };
	Vec3 GetVec3() const 
	{ 
		Vec3 vec;	vec.x = fval[0];	vec.y = fval[1];	vec.z = fval[2];
		return vec;
	};
	Quat GetQuat() const 
	{ 
		Quat quat;
		quat.v.x = fval[0];	quat.v.y = fval[1];	quat.v.z = fval[2];	quat.w = fval[3];
		return quat;
	};
	ILINE void GetValue( float& val ) { val = GetFloat(); };
	ILINE void GetValue( Vec3& val )  { val = GetVec3(); };
	ILINE void GetValue( Quat& val )  { val = GetQuat(); };
};

/** IEntityKey used in Entity track.
*/
struct IEventKey : public IKey
{
	const char* event;
	const char* eventValue;
	union {
		const char* animation;
		const char* target;
	};
	union {
		float value;
		float duration;
	};
	bool bNoTriggerInScrubbing;

	IEventKey()
	{
		event = "";
		eventValue = "";
		animation = "";
		duration = 0;
		bNoTriggerInScrubbing = false;
	}
};

/** ISelectKey used in Camera selection track or Scene node.
*/
struct ISelectKey : public IKey
{
	char szSelection[128];	//!< Node name.
	float fDuration;
	float fBlendTime;
	
	ISelectKey()
	{
		fDuration = 0;
		fBlendTime = 0;
		szSelection[0] = '\0'; // empty string.
	}
};

/** ISequenceKey used in sequence track.
*/
struct ISequenceKey : public IKey
{
	char szSelection[128];	//!< Node name.
	float fDuration;
	float fStartTime;
	float fEndTime;
	bool bOverrideTimes;
	bool  bDoNotStop;
	
	ISequenceKey()
	{
		szSelection[0] = '\0'; // empty string.
		fDuration = 0;
		fStartTime = 0;
		fEndTime = 0;
		bOverrideTimes = false;
		bDoNotStop = false; // default crysis behaviour
	}
};

/** ISoundKey used in sound track.
*/
struct ISoundKey : public IKey
{
	char pszFilename[128];
	float fVolume;
	unsigned char nPan;
	float inRadius;
	float outRadius;
	bool bStream;	//!< Stream sound from disk.
	bool b3DSound; //!< 3D or Stereo sound.
	bool bLoop; //!< Loop sound.
	bool bVoice; //!< Voice sound.
	bool bLipSync; //!< Automatically play corresponding FSQ
	float fStartTime;
	float fEndTime;
	int nSoundLength;
	char description[32];
	Vec3 customColor;
	bool bUseCustomColor;

	ISoundKey()
	{
		b3DSound				= false;
		inRadius				= 0;
		outRadius				= 0;
		bStream					= false;
		bLoop						= false;
		bVoice					= false;
		bLipSync        = true;
		pszFilename[0]	= 0;
		fVolume					= 1.0f;
		nPan						= 127;
		fStartTime		=	0;
		fEndTime		= 0;
		nSoundLength = 0;
		description[0]	= 0;
		bUseCustomColor = false;
		customColor.zero();
	}
};

/** ICharacterKey used in Character animation track.
*/
struct ICharacterKey : public IKey
{
	char m_animation[64];	//!< Name of character animation.
	float m_duration;		//!< Duration in seconds of this animation.
	float m_startTime;	//!< Start time of this animtion (Offset from begining of animation).
	float m_endTime;		//!< End time of this animation (can be smaller than the duration).
	float m_speed;			//!< Speed multiplier for this key.
	bool	m_bLoop;			//!< True if animation must be looped.
	bool  m_bBlendGap;  //!< True if gap to next animation should be blended
	bool	m_bUnload;		//!< Unload after sequence is finished
	bool	m_bInPlace;   // Play animation in place (Do not move root).

	float GetValidEndTime() const
	{
		float endTime = m_endTime;
		if(endTime == 0 || endTime > m_duration)
			endTime = m_duration;
		return endTime;
	}

	float GetActualDuration() const
	{
		float endTime = GetValidEndTime();
		float speed = m_speed;
		if(speed == 0)
			speed = 1.0f;

		return (endTime - m_startTime) / speed;
	}

	ICharacterKey()	
	{
		m_animation[0] = '\0'; 
		m_duration = 0; 
		m_endTime = 0; 
		m_startTime = 0; 
		m_speed = 1;
		m_bLoop = false;
		m_bBlendGap = false;
		m_bUnload = false;
		m_bInPlace = false;
	}
};


/** IMannequinKey used in Mannequin animation track.
*/
struct IMannequinKey : public IKey
{
	char m_fragmentName[64];	//!< Name of character animation.
	char m_tags[64];	//!< Name of character animation.
	int m_priority;
	float m_duration;		//!< Duration in seconds of this animation.
	
	IMannequinKey()	
	{
		m_fragmentName[0] = '\0';
		m_tags[0] = '\0';
		m_duration = 0;
		m_priority = 0;
	}
};

/** IExprKey used in expression animation track.
*/
struct IExprKey : public IKey
{
	IExprKey()
	{
		pszName[0]=0;
		fAmp=1.0f;
		fBlendIn=0.5f;
		fHold=1.0f;
		fBlendOut=0.5f;
	}
	char pszName[128];	//!< Name of morph-target
	float fAmp;
	float fBlendIn;
	float fHold;
	float fBlendOut;
};

/** IConsoleKey used in Console track, triggers console commands and variables.
*/
struct IConsoleKey : public IKey
{
	char command[64];

	IConsoleKey()
	{
		command[0] = '\0';
	}
};

enum EMusicKeyType
{
	eMusicKeyType_SetMood=0,
	eMusicKeyType_VolumeRamp
};

/** IMusicKey used in music track.
*/
struct IMusicKey : public IKey
{
	EMusicKeyType eType;
	char szMood[64];
	float fTime;
	float fDuration;
	char description[32];
	IMusicKey()
	{
		eType=eMusicKeyType_SetMood;
		szMood[0]=0;
		fTime=0.0f;
		fDuration=0.0f;
		description[0]=0;
	}
};

/** IFaceSeqKey used in face sequence track.
*/
struct IFaceSeqKey : public IKey
{
	char szSelection[128];	//!< Node name.
	float fDuration;
	
	IFaceSeqKey()
	{
		fDuration = 0;
		szSelection[0] = '\0'; // empty string.
	}
};

struct ILookAtKey : public IKey
{
	char szSelection[128];	//!< Node name.
	float fDuration;
	char lookPose[128];
	float smoothTime;
	
	ILookAtKey()
	{
		fDuration = 0;
		szSelection[0] = '\0'; // empty string.
		smoothTime = 0.2f;
		lookPose[0] = 0x00;
	}
};

//! Discrete (non-interpolated) float key.
struct IDiscreteFloatKey: public IKey
{
	float m_fValue;

	void SetValue(float fValue)
	{
		m_fValue=fValue;
	}

	IDiscreteFloatKey()
	{
		m_fValue=-1.0f;
	}
};

//! A key for the capture track.
struct ICaptureKey : public IKey
{
	float duration;
	float timeStep;
	char folder[ICryPak::g_nMaxPath];
	bool once;
	char prefix[ICryPak::g_nMaxPath/4];
	int bufferToCapture;
	
	const char * GetFormat() const
	{ return format;	}

	void FormatJPG()
	{ format = "jpg"; }
	void FormatBMP()
	{ format = "bmp"; }
	void FormatTGA()
	{ format = "tga"; }
	void FormatHDR()
	{ format = "hdr"; }

	ICaptureKey() : IKey(), duration(0), timeStep(0.033f), once(false), bufferToCapture(0)
	{ 
		folder[0] = '\0';
		prefix[0] = '\0';
		FormatTGA(); 
		ICVar *pCaptureFolderCVar = gEnv->pConsole->GetCVar("capture_folder");
		if(pCaptureFolderCVar != NULL  && pCaptureFolderCVar->GetString())
			strcpy_s(folder, pCaptureFolderCVar->GetString());
		ICVar *pCaptureFilePrefixCVar = gEnv->pConsole->GetCVar("capture_file_prefix");
		if(pCaptureFilePrefixCVar != NULL  && pCaptureFilePrefixCVar->GetString())
			strcpy_s(prefix, pCaptureFilePrefixCVar->GetString());
		ICVar *pCaptureFileFormatCVar = gEnv->pConsole->GetCVar("capture_file_format");
		if(pCaptureFileFormatCVar != NULL )
			format = pCaptureFileFormatCVar->GetString();
	}

	ICaptureKey(const ICaptureKey& other) : IKey(other),
																					duration(other.duration), timeStep(other.timeStep),
																					once(other.once), format(other.format),
																					bufferToCapture(other.bufferToCapture)
	{
		strcpy_s(folder, other.folder);
		strcpy_s(prefix, other.prefix);
	}

private:
	const char * format;
};

//! Boolean key.
struct IBoolKey : public IKey
{
	IBoolKey() {};
};

//! Comment Key.
struct ICommentKey : public IKey
{
	enum ETextAlign
	{
		eTA_Left	=	0,
		eTA_Center	= 	BIT(1),
		eTA_Right	=	BIT(2)
	};

	//-----------------------------------------------------------------------------
	//!
	ICommentKey()
	:m_duration(1.f),m_size(1.f),m_align(eTA_Left)
	{
		sprintf_s(m_strFont,sizeof(m_strFont),"default");
		m_color = Vec3(1.f,1.f,1.f);
	}

	//-----------------------------------------------------------------------------
	//!
	ICommentKey(const ICommentKey& other)
		:IKey(other)
	{ 
		m_strComment = other.m_strComment;
		m_duration = other.m_duration;
		strcpy_s(m_strFont, other.m_strFont);
		m_color = other.m_color;
		m_size = other.m_size;
		m_align = other.m_align;
	}

	string	m_strComment;
	float	m_duration;

	char	m_strFont[64];
	Vec3	m_color;
	float	m_size;
	ETextAlign	m_align;
};

//-----------------------------------------------------------------------------
//!
struct IScreenFaderKey : public IKey
{
	//-----------------------------------------------------------------------------
	//!
	enum EFadeType { eFT_FadeIn = 0, eFT_FadeOut = 1 };
	enum EFadeChangeType {	eFCT_Linear = 0, eFCT_Square=1, eFCT_CubicSquare=2, eFCT_SquareRoot=3, eFCT_Sin=4 };

	//-----------------------------------------------------------------------------
	//!
	IScreenFaderKey():IKey(),m_fadeTime(2.f),m_bUseCurColor(true),m_fadeType(eFT_FadeOut),m_fadeChangeType(eFCT_Linear)
	{
		m_fadeColor = Vec4(0,0,0,1);
		m_strTexture[0] = '\0';
	}

	//-----------------------------------------------------------------------------
	//!
	IScreenFaderKey(const IScreenFaderKey& other)
		:IKey(other),m_fadeTime(other.m_fadeTime),m_bUseCurColor(other.m_bUseCurColor),m_fadeType(other.m_fadeType),m_fadeChangeType(other.m_fadeChangeType)
	{
		m_fadeColor = other.m_fadeColor;
		strcpy_s(m_strTexture, other.m_strTexture);
	}

	//-----------------------------------------------------------------------------
	//!
	float		m_fadeTime;
	Vec4		m_fadeColor;
	char		m_strTexture[64];
	bool		m_bUseCurColor;
	EFadeType	m_fadeType;
	EFadeChangeType m_fadeChangeType;
};

#endif // __animkey_h__
