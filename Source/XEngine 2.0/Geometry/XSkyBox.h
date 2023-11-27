/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyBox.h Visual Studio 2012 Version.
// It's An XEngine SkyBox Header File.
// Date Creation: 21 April 2014
// Last Changing: 10 May 2014
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
const D3D11_INPUT_ELEMENT_DESC g_aVertexLayout[] =
{
  //{ "POSITION", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
  { "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, 
  { "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },  
  { "NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,0,20,D3D11_INPUT_PER_VERTEX_DATA,0}
};

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXSkyBox;

//-----------------------------------------------------------------------------
// Name: CXSkyBox
// Desc: The SkyBox Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CXSKYBOX_H__)
#define AFX_CXSKYBOX_H__

class CXSkyBox
{
public:
  //Public Directives
  CXSkyBox(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
  virtual ~CXSkyBox();
  void Release();

  void Render();
  HRESULT CreateBox(float CubeSize);
  void UpdateBox(const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
  ID3D11Texture2D* GetD3D10EnvironmentMap() { return m_pEnvironmentMap; }
  ID3D11ShaderResourceView* GetD3D10EnvironmentMapRV() { return m_pEnvironmentRV; }
  void SetD3D11EnvironmentMap(ID3D11Texture2D* pCubeTexture) { m_pEnvironmentMap = pCubeTexture; }
  void SetWorldViewProj(D3DXMATRIX* pWorldViewProj) { m_pWorldViewProj = pWorldViewProj; }

private:
  //Private Directives
  ID3D11Device* d_pDevice;                  //Pointer On Direct3D Device Interface
  ID3D11DeviceContext* d_pImmediateContext; //Pointer On Direct3D Immediate Context
  D3DXMATRIX* m_pWorldViewProj;             //Matrix Transformations
protected:
  //Protected Directives
  //SkyBox Vertex Structure
  struct SKYBOX_VERTEX { D3DXVECTOR4 Pos; };
  struct CB_VS_PER_OBJECT { D3DXMATRIX m_WorldViewProj; }; 
  
  ID3D11Texture2D* m_pEnvironmentMap;
  ID3D11ShaderResourceView* m_pEnvironmentRV;
  ID3D11VertexShader* m_pVertexShader;
  ID3D11PixelShader* m_pPixelShader;
  ID3D11SamplerState* m_pSam;
  ID3D11InputLayout* m_pVertexLayout;
  ID3D11Buffer* m_pcbVSPerObject;
  ID3D11Buffer* m_pVB;
  ID3D11DepthStencilState* m_pDepthStencilState;
  float m_fCubeSize; //Sky Cube Size
};
#endif //!defined(AFX_CXSKYBOX_H__)

//-----------------------------------------------------------------------------