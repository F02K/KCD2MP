
#ifndef __CRELIGHTSHAPE_H__
#define __CRELIGHTSHAPE_H__

//===========================================================================

UNIQUE_IFACE class CRELightShape : public CRendElementBase
{
	public:
		
		CRELightShape()
		{
			mfSetType( eDATA_LightShape );
			mfUpdateFlags( FCEF_TRANSFORM );
		}
		
		virtual ~CRELightShape();

		virtual void mfPrepare(bool bCheckOverflow);  
		virtual bool mfDraw( CShader* ef, SShaderPass* sfm );

		virtual void GetMemoryUsage(ICrySizer *pSizer) const 
		{
			pSizer->AddObject(this, sizeof(*this));
		}
};

#endif	// __CRELIGHTSHAPE_H__
