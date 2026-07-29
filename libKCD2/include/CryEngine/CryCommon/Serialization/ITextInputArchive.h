#pragma once
#include "Serialization/IArchive.h"
#include "CryExtension/ICryUnknown.h"
#include "CryExtension/CryCreateClassInstance.h"

namespace Serialization {

class ITextInputArchive : public ICryUnknown, public IArchive
{
public:
	CRYINTERFACE_DECLARE(ITextInputArchive, 0x1845738b1dcc4168, 0xb440dba776b460c9)

	using IArchive::operator();

	virtual bool LoadFileUsingCRT(const char* filename) = 0;
	virtual bool AttachMemory(const char* buffer, size_t size) = 0;

protected:
	ITextInputArchive(int caps) : IArchive(caps) {}
};

inline boost::shared_ptr<ITextInputArchive> CreateTextInputArchive()
{
	boost::shared_ptr<ITextInputArchive> pArchive;
	CryCreateClassInstance(MAKE_CRYGUID(0x7a83a1c890054608, 0x9f8447a4b0ad6c3b), pArchive);
	return pArchive;
}

}