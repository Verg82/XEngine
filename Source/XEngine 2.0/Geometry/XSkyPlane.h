/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyPlane.h Visual Studio 2012 Version.
// It's An XEngine SkyPlane Perturbed Clouds Implementation Header File.
// Date Creation: 21 April 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>
#include <XMethods.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXSkyPlane;

//-----------------------------------------------------------------------------
// Name: CXSkyPlane
// Desc: The Sky Plane Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CXSkyPlane_H__)
#define AFX_CXSkyPlane_H__

class CXSkyPlane
{
public:
  //Public Directives
  CXSkyPlane(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
  virtual ~CXSkyPlane();
  void Release();

  void Render(D3DXMATRIX,D3DXMATRIX,D3DXMATRIX);
  HRESULT Create(WCHAR*,WCHAR*);
  void SetTransformCoord(D3DXVECTOR3 CamPos) { VecPos = CamPos; }
private:
  //Private Directives
  ID3D11Device* d_pDevice;                  //Pointer On Direct3D Device Interface
  ID3D11DeviceContext* d_pImmediateContext; //Pointer On Direct3D Immediate Context
  
  ID3D11BlendState* m_AlphaBlendState;
  D3DXMATRIX m_pWorldViewProj;             //World\View\Projection Matrix Transformations
  D3DXVECTOR3 VecPos;                      //Camera Position Vector In Space

  HRESULT CreateSkyPlane(int,float,float,float,int);
  HRESULT CreateSkyPlaneShader();
protected:
  //Protected Directives
  //SkyPlane Structures
  struct SkyPlaneType
  {
	float x,y,z;
	float tu,tv;
  };

  //SkyPlane Vertex Structure
  struct VertexType
  {
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 Texture;
  };

  //SkyPlane Shader Structures
  struct MatrixBufferType
  {
	D3DXMATRIX World;
	D3DXMATRIX Pos;
	D3DXMATRIX View;
	D3DXMATRIX Projection;
  };

  struct SkyBufferType
  {
	float Translation;
	float Scale;
	float Brightness;
	float Padding;
  };

  //SkyPlane Directives
  SkyPlaneType* m_SkyPlane;
  int m_VertexCount,m_IndexCount;
  ID3D11Buffer *m_VertexBuffer,*m_IndexBuffer;
  float m_Scale,m_Brightness,m_Translation;
  ID3D11ShaderResourceView *m_CloudTexture,*m_PerturbTexture;

  //SkyPlane Shader Directives
  ID3D11VertexShader* m_VertexShader;
  ID3D11PixelShader* m_PixelShader;
  ID3D11InputLayout* m_Layout;
  ID3D11SamplerState* m_SampleState;
  ID3D11Buffer* m_MatrixBuffer;
  ID3D11Buffer* m_SkyBuffer;
};
#endif //!defined(AFX_CXSkyPlane_H__)

//---------------------------------------------------------------------------