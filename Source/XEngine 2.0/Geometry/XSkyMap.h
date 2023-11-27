/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyMap.h Visual Studio 2012 Version.
// It's An XEngine SkyMap Implementation Header File.
// Date Creation: 21 April 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>
#include <XMethods.h>

//////////////////////////////////////////////////////////////////////////////
// Global Definitions Or Prototypes
//////////////////////////////////////////////////////////////////////////////
const D3D11_INPUT_ELEMENT_DESC g_aSkyMapVertexLayout[] =
{
  { "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, 
  { "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },  
  { "NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0}
};

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXSkyMap;

//-----------------------------------------------------------------------------
// Name: CXSkyMap
// Desc: The SkyMap Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CXSkyMap_H__)
#define AFX_CXSkyMap_H__

class CXSkyMap
{
public:
  //Public Directives
  CXSkyMap(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
  virtual ~CXSkyMap();
  void Release();

  void Render();
  HRESULT CreateSkyMap(WCHAR*);
  void SetWorldViewProj(D3DXMATRIX* pWorldViewProj) { m_pWorldViewProj = pWorldViewProj; }
  void SetTransformCoord(D3DXVECTOR3 CamPos) { VecPos = CamPos; }

private:
  //Private Directives
  ID3D11Device* d_pDevice;                  //Pointer On Direct3D Device Interface
  ID3D11DeviceContext* d_pImmediateContext; //Pointer On Direct3D Immediate Context
  
  //SkyMap Sphere Directives
  D3DXMATRIX* m_pWorldViewProj;             //World\View\Projection Matrix Transformations
  D3DXMATRIX m_pSphereWorld;                //SkyMap Sphere World Matrix
  D3DXVECTOR3 VecPos;                       //Camera Position Vector In Space
  int NumSphereVertices;
  int NumSphereFaces;
  ID3D11Buffer* m_pSIB;                     //SkyMap Sphere Index Buffer
  ID3D11Buffer* m_pSVB;                     //SkyMap Sphere Vertex Buffer
  ID3D11ShaderResourceView* m_pSMResView;   //SkyMap Resource View (Cube Texture)
  ID3D11VertexShader* m_pVertexShader;
  ID3D11PixelShader* m_pPixelShader;
  ID3D11SamplerState* m_pSam;
  ID3D11InputLayout* m_pVertexLayout;
  ID3D11Buffer* m_pcbVSPerObject;
  ID3D11DepthStencilState* m_pDepthStencilState;
  ID3D11RasterizerState* RSCullNone;

  void UpdateSkyMap(int LatLines, int LongLines);
protected:
  //Protected Directives
  //SkyMap Vertex Structure
  struct SKYMAP_VERTEX { D3DXVECTOR3 Pos; };
  struct CB_VS_PER_OBJECT { D3DXMATRIX m_WorldViewProj; }; 
  CB_VS_PER_OBJECT pVSPerObject;
};
#endif //!defined(AFX_CXSkyMap_H__)

//-----------------------------------------------------------------------------