/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XFps.cpp Visual Studio 2012 Version.
// This XEngine FPS Counter Implementation Source File.
// Date Creation: 21 April 2014
// Last Changing: 05 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XFps.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXFps Class
///////////////////////////////////////////////////////////////////////////////
CXFps::CXFps()
{
} //EndConstructionDirectives

CXFps::~CXFps()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXFps::Initialize()
// Desc: 
//----------------------------------------------------------------------------
void CXFps::Initialize()
{
  //Set Default Parameters
  m_Fps = 0;
  m_Count = 0;
  m_StartTime = timeGetTime();
  PDH_STATUS Status;

  //Initialize The Flag Indicating Whether This Object Can Read The System Cpu Usage Or Not
  m_CanReadCpu = true;

  //Create A Query Object To Poll Cpu Usage
  Status = PdhOpenQuery(NULL,0,&m_QueryHandle);
  if (Status != ERROR_SUCCESS)
  {
	m_CanReadCpu = false;
  }

  //Set Query Object To Poll All Cpus In The System.
  Status = PdhAddCounter(m_QueryHandle,TEXT("\\Processor(_Total)\\% processor time"),0,&m_CounterHandle);
  if (Status != ERROR_SUCCESS)
  {
	m_CanReadCpu = false;
  }
  
  m_LastSampleTime = GetTickCount(); 
  m_CpuUsage = 0;

  return;
} //EndInitializeProcedure

//-----------------------------------------------------------------------------
// Name: CXFps::Update()
// Desc: 
//-----------------------------------------------------------------------------
void CXFps::Update()
{
  m_Count++;
  if (timeGetTime() >= (m_StartTime+1000))
  {
    m_Fps = m_Count;
	m_Count = 0;
	m_StartTime = timeGetTime();
  }

  PDH_FMT_COUNTERVALUE Value; 
  if (m_CanReadCpu)
  {
	if ((m_LastSampleTime+1000) < GetTickCount())
	{
	  m_LastSampleTime = GetTickCount(); 
      PdhCollectQueryData(m_QueryHandle);
      PdhGetFormattedCounterValue(m_CounterHandle,PDH_FMT_LONG,NULL,&Value);
      m_CpuUsage = Value.longValue;
	}
  }
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CXFps::GetFps()
// Desc:
//-----------------------------------------------------------------------------
int CXFps::GetFps()
{
  return m_Fps;
} //EndGetFpsFunction

//-----------------------------------------------------------------------------
// Name: CXFps::GetCpuUsage()
// Desc:
//-----------------------------------------------------------------------------
int CXFps::GetCpuUsage()
{
  int Usage;
  if (m_CanReadCpu) Usage = (int)m_CpuUsage;
  else Usage = 0;
  return Usage;
} //EndGetCpuUsageFunction

//-----------------------------------------------------------------------------
// Name: CXFps::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CXFps::Release()
{
  if (m_CanReadCpu) PdhCloseQuery(m_QueryHandle);
} //EndReleaseProcedure

//-----------------------------------------------------------------------------