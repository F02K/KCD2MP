#pragma once

struct IPhyicalWorld;
class CCamera;

// this object implements rendering of debug information
// through IPhysRenderer (CryPhysics) with IRenderer (XRender)
struct IPhysicsDebugRenderer
{
	// <interfuscator:shuffle>
	virtual ~IPhysicsDebugRenderer(){}
	virtual void UpdateCamera(const CCamera& camera) = 0;
	virtual void DrawAllHelpers(IPhysicalWorld* world) = 0;
	virtual void DrawEntityHelpers(IPhysicalEntity* entity, int helperFlags) = 0;
	virtual void Flush(float dt) = 0;
	// </interfuscator:shuffle>
};
