#ifndef __ICRYVOICE_H__
#define __ICRYVOICE_H__

#pragma once

struct ICryVoice
{
	// <interfuscator:shuffle>
	virtual ~ICryVoice(){}
	// Mute
	// Turn voice on/off between a local and remote user. When mute is on no voice will be sent to or received from the remote user.
	// localUser	- The pad number of the local user.
	// remoteUser	-	The CryUserID of the remote user.
	// mute				- Mute on/off
	virtual	void	Mute(uint32 localUser, CryUserID remoteUser, bool mute) = 0;

	// IsMuted
	// Has voice between a local and remote user been muted by game.
	// localUser	- The pad number of the local user.
	// remoteUser	-	The CryUserID of the remote user.
	// return			- Will return true if the game has set mute to true.
	virtual	bool	IsMuted(uint32 localUser, CryUserID remoteUser) = 0;

	// MuteExternally
	// Add remote user to the local user's global mute list via the external SDK. Users added to the global mute list
	// will persist between games/cross titles/power cycles.
	// localUser	- The pad number of the local user.
	// remoteUser	-	The CryUserID of the remote user.
	// mute				- Mute on/off
	virtual	void	MuteExternally(uint32 localUser, CryUserID remoteUser, bool mute) = 0;

	// IsMutedExternally
	// Has voice between a local and remote user muted via the external SDK's GUI.
	// Microsoft TCR's say that a user must have no indication that they have been muted with the Xbox GUI
	// so this function must not be used for that purpose.
	// localUser	- The pad number of the local user.
	// remoteUser	-	The CryUserID of the remote user.
	// return			- Will return true if the user has set mute to true via the external SDK's GUI.
	virtual	bool	IsMutedExternally(uint32 localUser, CryUserID remoteUser) = 0;

	// IsSpeaking
	// Is the remote user currently speaking to the local user.
	// localUser	- The pad number of the local user.
	// remoteUser	-	The CryUserID of the remote user.
	// return			- true if voice data is currently being received from the remote user to the local user.
	virtual bool	IsSpeaking(uint32 localUser, CryUserID remoteUser) = 0;

	// IsMicrophoneConnected
	// Does the user have a microphone connected
	// userID			- The CryUserID of the user
	// return			- true if a microphone is connected
	virtual bool	IsMicrophoneConnected(CryUserID userID) = 0;


	// EnablePushToTalk
	// Enables Push-to-talk functionality for microphone. Currently only supported on PC.
	// localUser	- The pad number of the local user.
	// bSet				- Enabled true or false.
	virtual void	EnablePushToTalk(uint32 localUser, bool bSet) = 0;

	// IsPushToTalkedEnabled
	// Is Push-to-talk mode enabled?
	// return			- true or false
	virtual bool	IsPushToTalkEnabled(uint32 localUser) = 0;

	// PushToTalkButtonDown
	// Mark the push-to-talk button as depressed
	// localUser	- The pad number of the local user.
	virtual void	PushToTalkButtonDown(uint32 localUser) = 0;
	
	// PushToTalkButtonUp
	// Mark the push-to-talk button as released
	// localUser	- The pad number of the local user.
	virtual void	PushToTalkButtonUp(uint32 localUser) = 0;

	// SetVoicePlaybackVolume
	// Set the playback volume for voice communications
	// localUser	- The pad number of the local user.
	// volume			- The volume. 0.0 to 1.0
	virtual void SetVoicePlaybackVolume(uint32 localUser, float volume) = 0;

	// GetVoicePlaybackVolume
	// Get the playback volume for voice communications
	// localUser	- The pad number of the local user.
	// return			- volume. 0.0 to 1.0
	virtual float GetVoicePlaybackVolume(uint32 localUser) = 0;
	// </interfuscator:shuffle>
};

#endif // __ICRYVOICE_H__