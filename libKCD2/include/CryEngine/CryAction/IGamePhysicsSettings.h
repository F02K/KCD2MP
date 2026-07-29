#ifndef __IGAMEPHYSICSSETTINGS_H__
#define __IGAMEPHYSICSSETTINGS_H__

#pragma once

struct IGamePhysicsSettings
{
	virtual ~IGamePhysicsSettings() {}

	virtual const char* GetCollisionClassName(unsigned int bitIndex) = 0;
};

#endif
