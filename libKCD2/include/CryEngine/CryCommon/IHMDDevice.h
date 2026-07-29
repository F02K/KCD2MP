////////////////////////////////////////////////////////////////////////////
//
//  CryEngine Source File.
//  Copyright (C), Crytek, 1999-2013.
// -------------------------------------------------------------------------
//  File name:   IHMDDevice.h
//  Version:     v1.00
//  Description: Common interface for head mounted devices.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef _I_HMD_DEVICE_H_
#define _I_HMD_DEVICE_H_

#pragma once

#include <Cry_Math.h>


struct IHMDDevice
{
public:
	enum DeviceType
	{
		Null=0,
		OculusRift,
		Cinemizer,
		NumberOfHMDeviceTypes,
	};

	struct DeviceInfo
	{
		char hmdProductName[32];
		char hmdManufacturer[32];
		unsigned int hmdVersion;

		char sensorProductName[32];
		char sensorManufacturer[32];
		unsigned int sensorVersion;

		char serialNumber[20];
	};

	struct DisplayInfo
	{
		unsigned int hResolution;
		unsigned int vResolution;

		float hScreenSize;
		float vScreenSize;
		float vScreenCenter;
		float eyeToScreenDistance;
		float lensSeparationDistance;
		float interpupillaryDistance;

		float distortionK[4];
	};

	struct SensorInfo
	{
		float maxAcceleration;
		float maxRotationRate;
		float maxMagneticField;
	};

public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	virtual DeviceType GetType() const = 0;
	virtual bool IsUsable() const = 0;

	virtual void GetDeviceInfo(DeviceInfo& info) const = 0;
	virtual void GetDisplayInfo(DisplayInfo& info) const = 0;
	virtual void GetSensorInfo(SensorInfo& info) const = 0;

	virtual void ResetSensor() = 0;

	virtual Quat GetOrientation() const = 0;
	virtual Vec3 GetAcceleration() const = 0;
	virtual Vec3 GetAngularVelocity() const = 0;

	virtual void EnablePrediction(float delta, bool enablePrediction, bool enablePredictionFilter) = 0;
	virtual Quat GetPredictedOrientation() const = 0;

	virtual void TickLatencyTester() = 0;

protected:
	virtual ~IHMDDevice() {}
};


#endif //#ifndef _I_HMD_DEVICE_H_
