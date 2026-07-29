////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   ILMSerializationManager.h
//  Version:     v1.00
//  Created:     02/07/2003 by Sergiy Migdalskiy
//  Compilers:   Visual Studio.NET
//  Description: LightMap Serialization interface
// -------------------------------------------------------------------------
//  History:
//	 02/07/2003  Extracted declaration from I3dEngine.h to be able to easy
//               modify it
////////////////////////////////////////////////////////////////////////////
#include DEVIRTUALIZE_HEADER_FIX(ILMSerializationManager.h)

#ifndef _CRY_COMMON_LM_SERIALIZATION_MANAGER_HDR_
#define _CRY_COMMON_LM_SERIALIZATION_MANAGER_HDR_

#include <IEntitySystem.h>

// Forward declarations.
struct RenderLMData;
struct IRenderNode;
struct sFalseLightSerializationStructure;
struct LMGenParam ;
struct TexCoord2Comp;

// Summary:
//	 Interface for lightmap serialization.
UNIQUE_IFACE struct ILMSerializationManager
{
	// <interfuscator:shuffle>
	virtual ~ILMSerializationManager(){}

	virtual void Release() = 0;

	//
	virtual bool ApplyLightmapfile( const char *pszFileName, IRenderNode ** pIGLMs, int32 nIGLMNumber ) = 0;
	//

	virtual bool Load( const char *pszFileName, const bool cbNoTextures ) = 0;

	//
	virtual unsigned int InitLMUpdate(const char *pszFilePath, const bool bAppend ) = 0;

	//
	virtual bool LoadFalseLight(const char *pszFileName, IRenderNode ** pIGLMs, int32 nIGLMNumber) = 0;

	//
	virtual unsigned int SaveFalseLight(const char *pszFileName, const int nObjectNumber,const sFalseLightSerializationStructure* pFalseLightList) = 0;

	//
	virtual unsigned int Save( const char *pszFileName, LMGenParam sParams, const bool cbAppend = false ) = 0;
	//

	// Arguments:
	//	 indwWidth			-
	//	 indwHeight			-
	//	 pGLM_IDs			-
	//	 nGLM_IDNumber		-
	//	 _pColorLerp4		- If !=0 this memory is copied.
	//	 _pHDRColorLerp4	-
	//	 _pDomDirection3	- If !=0 this memory is copied.
	//	 _pOcclusion		-
	//	 _pRAE				-
	//	 strDirPath			-
	//	 pTextureID			-
	virtual RenderLMData *UpdateLMData(const uint32 indwWidth, const uint32 indwHeight, const std::pair<int32,int32>* pGLM_IDs, const int32 nGLM_IDNumber, uint8 *_pColorLerp4, uint8 *_pHDRColorLerp4, uint8 *_pDomDirection3, uint8 *_pOcclusion, uint8 *_pRAE, const char* strDirPath, int32* pTextureID) = 0;
	//
	virtual void AddTexCoordData( const TexCoord2Comp* pTexCoords, const int32 nTexCoordNumber, const int iGLM_ID_UsingTexCoord,const int iGLM_Idx_SubObj, const uint32 indwHashValue, const EntityId* pOcclIDsFirst,const EntityId* pOcclIDsSecond, const int32 nOcclIDNumber, const int8 nFirstOcclusionChannel ) = 0;

	//
	virtual void RescaleTexCoordData(const int iGLM_ID_UsingTexCoord,const int iGLM_Idx_SubObj, const f32 fScaleU, const f32 fScaleV ) = 0;

	// Summary:
	//	 Used for rebuild changes feature.
	// Return Value:
	//	 0x12341234 if this object wasn't in the list.
	virtual uint32 GetHashValue( const std::pair<int32,int32> iniGLM_ID_UsingTexCoord ) const=0;

	virtual bool ExportDLights(const char *pszFileName, const CDLight **ppLights, uint32 iNumLights, bool bNewZip = true) const = 0;
	// </interfuscator:shuffle>
};

#endif
