////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2010.
// -------------------------------------------------------------------------
//  File name:   IStereoRenderer.h
//  Created:     18/11/2010 by Nicolas Schulz.
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __ISTEREORENDERER_H__
#define __ISTEREORENDERER_H__

#pragma once


enum StereoDevice
{
	STEREO_DEVICE_NONE = 0,
	STEREO_DEVICE_FRAMECOMP = 1,
	STEREO_DEVICE_HDMI = 2,
	STEREO_DEVICE_DRIVER = 3,  // nvidia and amd drivers
	STEREO_DEVICE_DUALHEAD = 4,
	STEREO_DEVICE_COUNT,

	STEREO_DEVICE_DEFAULT = 100  // auto-detect device
};

enum StereoMode
{
	STEREO_MODE_NO_STEREO = 0,	// stereo disabled
	STEREO_MODE_DUAL_RENDERING = 1,
	STEREO_MODE_POST_STEREO = 2,  // extract from depth
	STEREO_MODE_COUNT,
};

enum StereoOutput
{
	STEREO_OUTPUT_STANDARD = 0,
	STEREO_OUTPUT_IZ3D = 1,
	STEREO_OUTPUT_CHECKERBOARD = 2,
	STEREO_OUTPUT_ABOVE_AND_BELOW = 3,
	STEREO_OUTPUT_SIDE_BY_SIDE = 4,
	STEREO_OUTPUT_LINE_BY_LINE = 5,
	STEREO_OUTPUT_ANAGLYPH = 6,
	STEREO_OUTPUT_VR_HEADSET = 7,
	STEREO_OUTPUT_COUNT,
};

enum StereoDeviceState
{
	STEREO_DEVSTATE_OK = 0,
	STEREO_DEVSTATE_UNSUPPORTED_DEVICE,
	STEREO_DEVSTATE_REQ_1080P,
	STEREO_DEVSTATE_REQ_FRAMEPACKED,
	STEREO_DEVSTATE_BAD_DRIVER,
	STEREO_DEVSTATE_REQ_FULLSCREEN
};


struct IStereoRenderer
{
	// <interfuscator:shuffle>
	virtual ~IStereoRenderer(){}

	virtual StereoDevice GetDevice() = 0;
	virtual StereoDeviceState GetDeviceState() = 0;
	
	virtual bool GetStereoEnabled() = 0;
	virtual float GetStereoStrength() = 0;
	virtual float GetMaxSeparationScene(bool half=true) = 0;
	virtual float GetZeroParallaxPlaneDist() = 0;

	virtual StereoMode		GetStereoMode() = 0;
	virtual StereoOutput	GetStereoOutput() = 0;
	virtual ITexture*			GetLeftRenderTarget() = 0;
	virtual ITexture*			GetRightRenderTarget() = 0;

	virtual void GetNVControlValues(bool &stereoEnabled, float &stereoStrength) = 0;
	// </interfuscator:shuffle>
};

#endif
