/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XForwardPlus.cpp Visual Studio 2012 Version.
// It's An XEngine Forward Plus Lighting Method Implementation Source File.
// Date Creation: 24 June 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2015
//                   (C)AMD Corporation. All Rights Reserved.
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XForwardPlus.h>

//using namespace ForwardPlus;

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXForwardPlus Class
//////////////////////////////////////////////////////////////////////////////
CXForwardPlus::CXForwardPlus(ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
{
  //Set Default Parameters
  d_pDevice = pDevice;            //To Appropriate Device Pointer To Class
  d_pImmediateContext = pContext; //To Appropriate Context Pointer To Class



} //EndConstructionDirectives

CXForwardPlus::~CXForwardPlus()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CXForwardPlus::Release()
// Desc: 
//------------------------------------------------------------------------------
void CXForwardPlus::Release()
{


  //NULL Immediate Context Pointer
  d_pImmediateContext = NULL;

  //NULL Device Pointer
  d_pDevice = NULL;               
} //EndReleaseProcedure

//---------------------------------------------------------------------------