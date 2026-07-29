////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2012.
//
////////////////////////////////////////////////////////////////////////////

#ifndef IVertexAnimation_h
#define IVertexAnimation_h

struct IVertexAnimation
{
	// <interfuscator:shuffle>
	virtual ~IVertexAnimation() { }

	virtual void SetFrameWeightByName(const ISkin* pISkin, const char* name, float weight) = 0;
	virtual void ClearAllFramesWeight() = 0;
	// </interfuscator:shuffle>
};

#endif // IVertexAnimation_h
