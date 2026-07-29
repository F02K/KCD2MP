/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2009.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: Interface to apply modification to an AI's handling of visual
	and auditory stimulus within his perception handler
  
 -------------------------------------------------------------------------
  History:
  - 25:05:2009: Created by Kevin Kirst

*************************************************************************/
#ifndef __I_PERCEPTIONHANDLER_MODIFIER_H__
#define __I_PERCEPTIONHANDLER_MODIFIER_H__

struct SAIEVENT;

enum EStimulusHandlerResult
{
	eSHR_Continue = 0,		// Allow perception manager to continue processing this stimulus normally
	eSHR_Ignore,			// Ignore this stimulus
	eSHR_MakeAggressive,	// Make perception manager consider this stimulus an act of aggression
};

struct IPerceptionHandlerModifier
{
	// <interfuscator:shuffle>
	virtual ~IPerceptionHandlerModifier() {}

	// Debug drawing
	virtual void DebugDraw(EntityId ownerId, float &fY) const = 0;

	// Determine if stimuli should be ignored
	// Return false if stimulus should be ignored
	virtual EStimulusHandlerResult OnVisualStimulus(SAIEVENT* pAIEvent, IAIObject *pReceiver, EntityId targetId) = 0;
	virtual EStimulusHandlerResult OnSoundStimulus(SAIEVENT* pAIEvent, IAIObject *pReceiver, EntityId targetId) = 0;
	// </interfuscator:shuffle>
};

#endif //__I_PERCEPTIONHANDLER_MODIFIER_H__
