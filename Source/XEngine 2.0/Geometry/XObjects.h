/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XObjects.h Visual Studio 2012 Version.
// It's An XEngine Header File.
// Made Specially For Implementing Loading And Rendering 3D Objects.
// Date Creation: 21 April 2014
// Last Changing: 01 May 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXCubeObj;

//-----------------------------------------------------------------------------
// Name: CXCubeObj
// Desc: The Cube Object Class
//-----------------------------------------------------------------------------
#if !defined(AFX_CXCubeObj_H__)
#define AFX_CXCubeObj_H__

class CXCubeObj
{
public:
  //Public Directives
  struct SimpleVertex { XMFLOAT3 Pos; XMFLOAT4 Color; };

  CXCubeObj();
  virtual ~CXCubeObj();

  void Create(ID3D10Device* pDevice);
  void Release();
  void Render();

  void SetProjection(XMMATRIX MatProj) { g_Projection = MatProj; }
protected:
  //Protected Directives
  ID3D10Device* d_pDevice; //Pointer Onto Direct3D Device Interface

  ID3D10Effect* g_pEffect;
  ID3D10EffectTechnique* g_pTechnique;
  ID3D10InputLayout* g_pVertexLayout;
  ID3D10Buffer* g_pVertexBuffer;
  ID3D10Buffer* g_pIndexBuffer;
  ID3D10EffectMatrixVariable* g_pWorldVariable;
  ID3D10EffectMatrixVariable* g_pViewVariable;
  ID3D10EffectMatrixVariable* g_pProjectionVariable;
  XMMATRIX g_World;
  XMMATRIX g_View;
  XMMATRIX g_Projection;

};
#endif //!defined(AFX_CXCubeObj_H__)

//----------------------------------------------------------------------------