/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XFont.h Visual Studio 2012 Version.
// This XEngine Font System Header File.
// Date Creation: 21 April 2014
// Last Changing: 08 June 2014
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
//class CXFont;

//-----------------------------------------------------------------------------
// Name: Struct CXFont
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CXFont_H__)
#define AFX_CXFont_H__

class CXFont
{ 
public:
  //Public Directives
  CXFont(ID3D11Device* Device,ID3D11DeviceContext* DevContext);
  //CXFont(const CXFont&);
  virtual ~CXFont();

  HRESULT Initialize(char*,WCHAR*);
  void Release();
  void BuildVertexArray(void*,char*,float,float);
  ID3D11ShaderResourceView* GetTexture();
  void Render(int IndexCount,D3DXVECTOR4);

  void SetWorldMat(D3DXMATRIX WorldMatrix) { m_WorldMatrix = WorldMatrix; }
  void SetViewMat(D3DXMATRIX ViewMatrix) { m_ViewMatrix = ViewMatrix; }
  void SetProjMat(D3DXMATRIX ProjMatrix) { m_ProjectionMatrix = ProjMatrix; }  

private:
  //Private Directives
  //Font Parameters Structures
  struct FontType
  {
	float Left,Right;
    int Size;
  };

  struct VertexType
  {
	D3DXVECTOR3 Position;
	D3DXVECTOR2 Texture;
  };
  
  //Font Shader Structures
  struct ConstantBufferType
  {
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Projection;
  };

  struct PixelBufferType
  {
	D3DXVECTOR4 PixelColor;
  };

  ID3D11Device* d_pDevice;            //D3D Device Pointer
  ID3D11DeviceContext* d_pDevContext; //D3D Device Context

  //Font Directives
  FontType* m_Font;
  ID3D11ShaderResourceView* m_Texture;

  bool LoadFontData(char*);
  bool LoadTexture(WCHAR*);

  //Font Shader Directives
  ID3D11VertexShader* m_VertexShader;
  ID3D11PixelShader* m_PixelShader;
  ID3D11InputLayout* m_Layout;
  ID3D11Buffer* m_ConstantBuffer;
  ID3D11SamplerState* m_SampleState;
  ID3D11Buffer* m_PixelBuffer;

  D3DXMATRIX m_WorldMatrix;
  D3DXMATRIX m_ViewMatrix;
  D3DXMATRIX m_ProjectionMatrix;
};
#endif //!defined(AFX_CXFont_H__)

//-----------------------------------------------------------------------------