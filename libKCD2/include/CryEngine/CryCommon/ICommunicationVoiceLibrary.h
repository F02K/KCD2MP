#pragma once

#ifndef ICommunicationVoiceLibrary_h
#define ICommunicationVoiceLibrary_h

struct ICommunicationVoiceLibrary
{
	// <interfuscator:shuffle>
	virtual ~ICommunicationVoiceLibrary(){}
	virtual uint32 GetGroupNames(const char* libraryName, uint32 maxCount, const char** names) const = 0;
	// </interfuscator:shuffle>
};

#endif // ICommunicationVoiceLibrary_h
