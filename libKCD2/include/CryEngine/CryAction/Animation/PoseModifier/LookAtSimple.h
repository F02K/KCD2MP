#ifndef LookAtSimple_h
#define LookAtSimple_h

#include <CryExtension/Impl/ClassWeaver.h>

namespace AnimPoseModifier {

class CLookAtSimple :
	public IAnimationPoseModifier
{
private:
	struct State
	{
		int32 jointId;
		Vec3 jointOffsetRelative;
		Vec3 targetGlobal;
		f32 weight;
	};

public:
	CRYINTERFACE_BEGIN()
		CRYINTERFACE_ADD(IAnimationPoseModifier)
	CRYINTERFACE_END()

	CRYGENERATE_CLASS(CLookAtSimple, "AnimationPoseModifier_LookAtSimple", 0xba7e2a809970435f, 0xb6679c08df616d74);

public:
	void SetJointId(uint32 id) { m_state.jointId = id; }
	void SetJointOffsetRelative(const Vec3& offset) { m_state.jointOffsetRelative = offset; }

	void SetTargetGlobal(const Vec3& target) { m_state.targetGlobal = target; }

	void SetWeight(f32 weight) { m_state.weight = weight; }

private:
	bool ValidateJointId(ICharacterModelSkeleton* pModelSkeleton);

	// IAnimationPoseModifier
public:
	virtual bool Prepare(const SAnimationPoseModifierParams& params);
	virtual bool Execute(const SAnimationPoseModifierParams& params);
	virtual void Synchronize();

	void GetMemoryUsage(ICrySizer* pSizer) const
	{
		pSizer->AddObject(this, sizeof(*this));
	}

private:
	State m_state;
	State m_stateExecute;
} _ALIGN(32);

} // namespace AnimPoseModifier

#endif // LookAtSimple_h
