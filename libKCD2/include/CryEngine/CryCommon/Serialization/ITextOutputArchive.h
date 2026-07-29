#pragma once
#include "Serialization/IArchive.h"
#include "CryExtension/ICryUnknown.h"
#include "CryExtension/CryCreateClassInstance.h"

namespace Serialization {

class ITextOutputArchive : public ICryUnknown, public IArchive
{
	CRYINTERFACE_DECLARE(ITextOutputArchive, 0xa273d6157a8b4f0d, 0x80ad6c8031bbfbf3)
public:
	virtual bool SaveFileUsingCRT(const char* filename) = 0;

	// buffer is a null-terminated string
	virtual const char* GetBuffer() const = 0;
	virtual size_t GetBufferLength() const = 0;

	// by default nested structres are put in one line, unless the line length is
	// longer than 'textWidth'
	virtual void SetTextWidth(int textWidth) = 0;

	using IArchive::operator();
protected:
	ITextOutputArchive(int caps) : IArchive(caps) {}
};

inline boost::shared_ptr<ITextOutputArchive> CreateTextOutputArchive()
{
	boost::shared_ptr<ITextOutputArchive> pArchive;
	CryCreateClassInstance(MAKE_CRYGUID(0xd1f14adbc4e74e49, 0x9cea55d80a55cbdb), pArchive);
	return pArchive;
}

}
