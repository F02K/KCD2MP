#ifndef QTANGENT_H
#define QTANGENT_H
#pragma once

namespace QTangent {

// Computes a QTangent from a frame and reflection scalar representing the
// tangent space.
// Will also ensure the resulting QTangent is suitable for 16bit quantization.
ILINE Quat FromFrameReflection(Quat frame, const float reflection)
{
	frame.v = -frame.v;
	if (frame.w < 0.0f)
		frame = -frame;

	// Make sure w is never 0 by applying the smallest possible bias.
	// This is needed in order to have sign() never return 0 in the shaders.
	static const float BIAS_16BIT = 1.0f / 32767.0f;
	static const float BIAS_SCALE_16BIT = sqrtf(1.0f - BIAS_16BIT*BIAS_16BIT);
	if (frame.w < BIAS_16BIT && frame.w > -BIAS_16BIT)
	{
		frame *= BIAS_SCALE_16BIT;
		frame.w = BIAS_16BIT;
	}

	if (reflection < 0.0f)
		frame = -frame;

	return frame;
}

ILINE Quat FromFrameReflection(const Matrix33& frame, const float reflection)
{
	Quat quat(frame);
	quat.Normalize();
	return FromFrameReflection(quat, reflection);
}

ILINE Quat FromFrameReflection16Safe(Matrix33 frame, const float reflection)
{
	frame.OrthonormalizeFast();
	if (!frame.IsOrthonormalRH(0.1f))
		frame.SetIdentity();

	return FromFrameReflection(frame, reflection);
}

ILINE void ToTangentBitangentReflection(const Quat& qtangent, Vec3& tangent, Vec3& bitangent, float& reflection)
{
	tangent = qtangent.GetColumn0();
	bitangent = qtangent.GetColumn1();
	reflection = qtangent.w < 0.0f ? -1.0f : +1.0f;
}

} // namespace QTangent

// Auxiliary helper functions

#include <IIndexedMesh.h> // <> required for Interfuscator

ILINE Vec4sf QuatF32ToQuatI16(const Quat& quat)
{
	int16f qx = tPackF2B(quat.v.x);
	int16f qy = tPackF2B(quat.v.y);
	int16f qz = tPackF2B(quat.v.z);
	int16f qw = tPackF2B(quat.w);

	Vec4sf packed;
#ifdef PS3
	packed.x = qw;
	packed.y = qz;
	packed.z = qy;
	packed.w = qx;
#else
	packed.x = qx;
	packed.y = qy;
	packed.z = qz;
	packed.w = qw;
#endif
	return packed;
}

ILINE Quat QuatI16ToQuatF32(const Vec4sf& quat)
{
	f32 qx = tPackB2F(quat.x);
	f32 qy = tPackB2F(quat.y);
	f32 qz = tPackB2F(quat.z);
	f32 qw = tPackB2F(quat.w);

	Quat depacked;
#ifdef PS3
	depacked.v.x = qw;
	depacked.v.y = qz;
	depacked.v.z = qy;
	depacked.w = qx;
#else
	depacked.v.x = qx;
	depacked.v.y = qy;
	depacked.v.z = qz;
	depacked.w = qw;
#endif
	return depacked;
}

ILINE Quat MeshTangentFrameToQTangent(const Vec4sf& tangent, const Vec4sf& bitangent)
{
#ifdef PS3
	Vec3 tangent32(tPackB2F(tangent.w), tPackB2F(tangent.z), tPackB2F(tangent.y));
	tangent32.Normalize();
	Vec3 bitangent32(tPackB2F(bitangent.w), tPackB2F(bitangent.z), tPackB2F(bitangent.y));
	bitangent32.Normalize();
	f32 reflection = tPackB2F(tangent.x);
#else
	Vec3 tangent32(tPackB2F(tangent.x), tPackB2F(tangent.y), tPackB2F(tangent.z));
	tangent32.Normalize();
	Vec3 bitangent32(tPackB2F(bitangent.x), tPackB2F(bitangent.y), tPackB2F(bitangent.z));
	bitangent32.Normalize();
	f32 reflection = tPackB2F(tangent.w);
#endif

	Matrix33 frame;
	frame.SetRow(0, tangent32);
	frame.SetRow(1, bitangent32);
	frame.SetRow(2, tangent32.Cross(bitangent32).GetNormalized());
	return QTangent::FromFrameReflection16Safe(frame, reflection);
}

ILINE bool MeshTangentsFrameToQTangents(
	const Vec4sf* pTangent, const uint tangentStride,
	const Vec4sf* pBitangent, const uint bitangentStride, const uint count,
	Vec4sf* pQTangents, const uint qtangentStride)
{
	Quat qtangent;
	for (uint i=0; i<count; ++i)
	{
		qtangent = MeshTangentFrameToQTangent(*pTangent, *pBitangent);
		*pQTangents = QuatF32ToQuatI16(qtangent);

		pTangent = (const Vec4sf*)(((const uint8*)pTangent) + tangentStride);
		pBitangent = (const Vec4sf*)(((const uint8*)pBitangent) + bitangentStride);
		pQTangents = (Vec4sf*)(((uint8*)pQTangents) + qtangentStride);
	}
	return true;
}

#endif // QTANGENT_H
