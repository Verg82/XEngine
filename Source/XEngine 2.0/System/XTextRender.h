/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XTextRender.h Visual Studio 2012 Version.
// This XEngine Text Render System Header File.
// Date Creation: 21 April 2014
// Last Changing: 09 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>
#include <XMethods.h>
#include <XFont.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXText;

//-----------------------------------------------------------------------------
// Name: Struct CXText
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CXText_H__)
#define AFX_CXText_H__

class CXText
{ 
public:
  //Public Directives
  CXText(ID3D11Device* Device,ID3D11DeviceContext* DevContext);
  //CXText(const CXText&);
  virtual ~CXText();
  void Release();

  void Initialize(int,int,D3DXMATRIX);
  HRESULT SetText(char* Text,int PositionX,int PositionY,float Red,float Green,float Blue);
  bool Render(D3DXMATRIX,D3DXMATRIX);
private:
  //Private Directives
  struct SentenceType
  {
	ID3D11Buffer *VertexBuffer,*IndexBuffer;
	int VertexCount,IndexCount,MaxLength;
	float Red,Green,Blue;
  };

  struct VertexType
  {
	D3DXVECTOR3 Position;
	D3DXVECTOR2 Texture;
  };

  ID3D11Device* d_pDevice;            //D3D Device Pointer
  ID3D11DeviceContext* d_pDevContext; //D3D Device Context
  CXFont* m_Font;                     //Font Object
  int m_ScreenWidth,m_ScreenHeight;
  D3DXMATRIX m_BaseViewMatrix;
  SentenceType* m_Sentence;

  void ReleaseSentence(SentenceType**);
  bool RenderSentence(SentenceType*,D3DXMATRIX,D3DXMATRIX);
  bool InitializeSentence(SentenceType**,int);
  bool UpdateSentence(SentenceType*,char*,int,int,float,float,float);
};
#endif //!defined(AFX_CXText_H__)

//---------------------------------------------------------------------------