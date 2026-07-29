////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2010
// -------------------------------------------------------------------------
//  Created by Francesco
//
////////////////////////////////////////////////////////////////////////////

#ifndef CHEAT_PROTECTION_H
#define CHEAT_PROTECTION_H

#pragma once

// Exports methods for encryption. Do not remove.
// In doubt, ask Marco Corbetta.
#ifdef ENABLE_CHEAT_PROTECTION
#define CHEAT_PROTECTION_EXPORT __declspec(dllexport)
#include "SolidShield/SolidSDK.h"
#else
#define CHEAT_PROTECTION_EXPORT
#endif

#endif