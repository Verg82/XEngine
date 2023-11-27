/******************************************************************************
// It's An Unofficial Version XEngine v.2.0.0.1
// XMethods.cpp
// It's The XEngine Useful Control Methods Header File.
// Made Specially For Implementing Useful Engine System Manipulations Parameters.
// Date Creation: 21 April 2014
// Last Changing: 09 May 2014
// Licensed By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Exporting Procedures Or Helper Functions
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Name: FloatToDWord()
// Desc: Helper Function To Stuff A FLOAT Into A DWORD Argument
//-----------------------------------------------------------------------------
DWORD FloatToDWord(FLOAT F) 
{ 
  return *((DWORD*)&F); 
} //EndFloatToDWordFunction

//-----------------------------------------------------------------------------
// Name: Log()
// Desc: Log Event Procedure
//-----------------------------------------------------------------------------
void Log(LPCSTR Str)
{ 
  FILE *LogFile = fopen("XEngine.log","a"); //Open File For Append Mode
  if (LogFile == NULL) return;
  fprintf(LogFile,Str);
  //fputs(Str,LogFile);
  fclose(LogFile); 
} //EndLogProcedure

//-----------------------------------------------------------------------------
// Name: ShowMessage()
// Desc: 
//-----------------------------------------------------------------------------
void ShowMessage(LPCWSTR Str)
{ 
  MessageBox(NULL,Str,L"Information",MB_OK);
} //EndShowMessageProcedure

//-----------------------------------------------------------------------------
// Name: CompileShaderFromFile()
// Desc: 
//----------------------------------------------------------------------------
HRESULT CompileShaderFromFile(WCHAR* szFileName,LPCSTR szEntryPoint,LPCSTR szShaderModel,ID3DBlob** ppBlobOut)
{
  //Set Default Parameters  
  HRESULT HRes = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #if defined(DEBUG) || defined(_DEBUG)
  //Set The D3DCOMPILE_DEBUG Flag To Embed Debug Information In The Shaders.
  //Setting This Flag Improves The Shader Debugging Experience, But Still Allows 
  //The Shaders To Be Optimized And To Run Exactly The Way They Will Run In 
  //The Release Configuration Of This Program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

  ID3DBlob* pErrorBlob;
  HRes = D3DX11CompileFromFile(szFileName,NULL,NULL,szEntryPoint,szShaderModel, 
  dwShaderFlags,0,NULL,ppBlobOut,&pErrorBlob,NULL);
  if (FAILED(HRes))
  {
    if (pErrorBlob != NULL) Log((char*)pErrorBlob->GetBufferPointer());
    ReleaseClass(pErrorBlob);
    return HRes;
  }
  ReleaseClass(pErrorBlob);
  return S_OK;
}  //EndCompileShaderFromFileFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXDeltaTimer Class
///////////////////////////////////////////////////////////////////////////////
CXDeltaTimer::CXDeltaTimer()
{
} //EndConstructionDirectives

CXDeltaTimer::~CXDeltaTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXDeltaTimer::UpdateTimer()
// Desc: Execute Each Frame To Establish Time Base
//-----------------------------------------------------------------------------
void CXDeltaTimer::UpdateTimer()
{
  CurrentTime = timeGetTime();
  DeltaTime = (float)((CurrentTime-LastTime)*0.001f);
  LastTime = CurrentTime;
} //EndUpdateTimerProcedure

//-----------------------------------------------------------------------------
// Name: CXDeltaTimer::GetTime()
// Desc:
//-----------------------------------------------------------------------------
float CXDeltaTimer::GetTime()
{
  return DeltaTime;
} //EndGetTimeFunction

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXTimer Class
///////////////////////////////////////////////////////////////////////////////
CXTimer::CXTimer()
{
} //EndConstructionDirectives

CXTimer::~CXTimer()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXTimer::Start()
// Desc: 
//----------------------------------------------------------------------------
bool CXTimer::Start()
{
  //Check To See If This System Supports High Performance Timers
  QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
  if (m_Frequency == 0)
  {
    return false;
  }

  //Find Out How Many Times The Frequency Counter Ticks Every Millisecond
  m_TicksPerMs = (float)(m_Frequency/1000);
  QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
  return true;
} //EndStartFunction

//-----------------------------------------------------------------------------
// Name: CXTimer::Update()
// Desc: 
//-----------------------------------------------------------------------------
void CXTimer::Update()
{
  INT64 CurrentTime;
  float TimeDifference;
  
  QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
  TimeDifference = (float)(CurrentTime-m_StartTime);
  m_FrameTime = TimeDifference/m_TicksPerMs;
  m_StartTime = CurrentTime;
  return;
} //EndUpdateProcedure

//-----------------------------------------------------------------------------
// Name: CXTimer::GetTime()
// Desc:
//-----------------------------------------------------------------------------
float CXTimer::GetTime()
{
  return m_FrameTime;
} //EndGetTimeFunction

//-----------------------------------------------------------------------------