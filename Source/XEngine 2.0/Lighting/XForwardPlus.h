/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XForwardPlus.h Visual Studio 2012 Version.
// It's An XEngine Forward Plus Lighting Method Implementation Header File.
// Date Creation: 24 June 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
//                   (C)AMD Corporation. All Rights Reserved.
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>
#include <XMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXForwardPlus;

//-----------------------------------------------------------------------------
// Name: CXForwardPlus
// Desc: The Forward Plus Lighting System Implementation Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CXForwardPlus_H__)
#define AFX_CXForwardPlus_H__

class CXForwardPlus
{
public:
  //Public Directives
  CXForwardPlus(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
  virtual ~CXForwardPlus();
  void Release();

 
private:
  //Private Directives
  ID3D11Device* d_pDevice;                  //Pointer On Direct3D Device Interface
  ID3D11DeviceContext* d_pImmediateContext; //Pointer On Direct3D Immediate Context
  
protected:
  //Protected Directives
  //Constant Buffers
  #pragma pack(push,1)
  struct CB_PER_OBJECT
  {
    D3DXMATRIX m_mWorldViewProjection;
    D3DXMATRIX m_mWorldView;
    D3DXMATRIX m_mWorld;
    D3DXVECTOR4 m_MaterialAmbientColorUp;
    D3DXVECTOR4 m_MaterialAmbientColorDown;
  };

  struct CB_PER_FRAME
  {
    D3DXMATRIX m_mProjection;
    D3DXMATRIX m_mProjectionInv;
    D3DXVECTOR3 m_vCameraPos;
    float m_fAlphaTest;
    unsigned m_uNumLights;
    unsigned m_uWindowWidth;
    unsigned m_uWindowHeight;
    unsigned m_uMaxNumLightsPerTile;
  };
  #pragma pack(pop)



};
#endif //!defined(AFX_CXForwardPlus_H__)

//---------------------------------------------------------------------------