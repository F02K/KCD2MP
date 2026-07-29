#ifndef _RENDER_OBJECT_DEFINES_
#define _RENDER_OBJECT_DEFINES_

#pragma once

//////////////////////////////////////////////////////////////////////
// CRenderObject::m_customFlags
#define COB_FADE_CLOAK_BY_DISTANCE						(1<<0)			// 1
#define COB_CUSTOM_POST_EFFECT								(1<<1)			// 2
#define COB_IGNORE_HUD_INTERFERENCE_FILTER    (1<<2)			// 4
#define COB_IGNORE_HEAT_AMOUNT								(1<<3)			// 8
#define COB_POST_3D_RENDER										(1<<4)			// 0x10
#define COB_IGNORE_CLOAK_REFRACTION_COLOR			(1<<5)			// 0x20
#define COB_HUD_REQUIRE_DEPTHTEST							(1<<6)			// 0x40
#define COB_CLOAK_INTERFERENCE								(1<<7)			// 0x80
#define COB_CLOAK_HIGHLIGHT										(1<<8)			// 0x100
#define COB_HUD_DISABLEBLOOM									(1<<9)			// 0x200
#define COB_DISABLE_MOTIONBLUR										(1<<10)			// 0x400

#endif // _RENDER_OBJECT_DEFINES_
