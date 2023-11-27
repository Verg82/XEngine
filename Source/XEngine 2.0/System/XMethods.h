/******************************************************************************
// It's An Unofficial Version XEngine v.2.0.0.1
// XMethods.h
// It's The XEngine Useful Control Methods Header File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 21 April 2014
// Last Changing: 09 May 2014
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSystem.h>

////////////////////////////////////////////////////////////////////////////////
// Directives For Release Classes
////////////////////////////////////////////////////////////////////////////////
//Directive For Release Class
#define ReleaseClass(_Ptr) { if (_Ptr) { (_Ptr)->Release(); (_Ptr) = NULL; } }
//Directive For Delete Class
#define DeleteClass(_Ptr) { if (_Ptr) { delete(_Ptr); (_Ptr) = NULL; } }
//Directive For Delete Array
#define DeleteArray(_Ptr) { if (_Ptr) { delete[](_Ptr); (_Ptr) = NULL; } }

////////////////////////////////////////////////////////////////////////////////
// Conversions Definitions
////////////////////////////////////////////////////////////////////////////////
//#ifndef NOMINMAX
#ifndef Max
#define Max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef Min
#define Min(a,b) (((a) < (b)) ? (a) : (b))
#endif
//#endif  

///////////////////////////////////////////////////////////////////////////////
// Exporting Procedures And Functions
///////////////////////////////////////////////////////////////////////////////
void Log(LPCSTR Str);
void ShowMessage(LPCWSTR Str);
DWORD FloatToDWord(FLOAT F);
HRESULT CompileShaderFromFile(WCHAR* szFileName,LPCSTR szEntryPoint,LPCSTR szShaderModel,ID3DBlob** ppBlobOut);

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class CXDeltaTimer;
//class CXTimer;

//-----------------------------------------------------------------------------
// Name: Struct CXDeltaTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CXDeltaTimer_H__)
#define AFX_CXDeltaTimer_H__

class CXDeltaTimer
{ 
public:
  //Public Directives
  CXDeltaTimer();
  virtual ~CXDeltaTimer();
  void UpdateTimer();
  float GetTime();
private:
  //Private Directives
  DWORD CurrentTime; //Current Timer Value
  DWORD LastTime;    //Previous Timer Value
  float DeltaTime;   //Time Elapsed Since Last Frame
};
#endif //!defined(AFX_CXDeltaTimer_H__)

//-----------------------------------------------------------------------------
// Name: Struct CXTimer
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CXTimer_H__)
#define AFX_CXTimer_H__

class CXTimer
{ 
public:
  //Public Directives
  CXTimer();
  virtual ~CXTimer();
  //CXTimer(const CXTimer&);

  bool Start();
  void Update();
  float GetTime();
private:
  //Private Directives
  INT64 m_Frequency;
  float m_TicksPerMs;
  INT64 m_StartTime;
  float m_FrameTime;
};
#endif //!defined(AFX_CXTimer_H__)

//-----------------------------------------------------------------------------
