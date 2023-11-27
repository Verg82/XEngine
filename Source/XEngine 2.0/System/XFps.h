/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XFps.h Visual Studio 2012 Version.
// This XEngine FPS Counter Implementation Header File.
// Date Creation: 21 April 2014
// Last Changing: 05 June 2014
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
//class CXFps;

//-----------------------------------------------------------------------------
// Name: Struct CXFps
// Desc:
//-----------------------------------------------------------------------------
#if !defined(AFX_CXFps_H__)
#define AFX_CXFps_H__

class CXFps
{ 
public:
  //Public Directives
  CXFps();
  //CXFps(const CXFps&);
  virtual ~CXFps();
  void Release();

  void Initialize();
  void Update();
  int GetFps();
  int GetCpuUsage();
private:
  //Private Directives
  //FPS Counters Directives
  int m_Fps,m_Count;
  unsigned long m_StartTime;

  //CPU Usage Counters Directives
  bool m_CanReadCpu;
  HQUERY m_QueryHandle;
  HCOUNTER m_CounterHandle;
  unsigned long m_LastSampleTime;
  long m_CpuUsage;
};
#endif //!defined(AFX_CXFps_H__)

//-----------------------------------------------------------------------------