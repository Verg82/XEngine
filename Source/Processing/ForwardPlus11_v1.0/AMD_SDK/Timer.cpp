/* ============================================================

Copyright (c) 2011 Advanced Micro Devices, Inc.  All rights reserved.

Redistribution and use of this material is permitted under the following 
conditions:

Redistributions must retain the above copyright notice and all terms of this 
license.

In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERA TION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */

#include "..\\DXUT\\Core\\DXUT.h"
#include "Timer.h"

//using namespace AMD;

#if !defined( _ASSERT )
	#define _ASSERT(exp)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p){p->Release(); p=NULL;}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p){delete p; p=NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p=NULL;}
#endif

#if USE_RDTSC
__declspec(naked) LONGLONG __cdecl rdtsc_time(void)
{
   __asm rdtsc
   __asm ret
}
#endif

//-----------------------------------------------------------------------------
Timer::Timer() :
    m_LastTime(0.0),
	m_SumTime(0.0),
	m_NumFrames(0)
{
}

Timer::~Timer()
{
}

double Timer::GetTime()
{
    FinishCollection();

    return m_LastTime; 
}
double Timer::GetSumTime()
{
	return m_SumTime;
}

double Timer::GetTimeNumFrames()
{
	return m_NumFrames;
}

//-----------------------------------------------------------------------------

CpuTimer::CpuTimer() :
   Timer()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    m_freq = static_cast<double>(freq.QuadPart);

#if USE_RDTSC
    const double calibrationTime = 0.1;

    LONGLONG start, stop;
    start = rdtsc_time();
    Delay(calibrationTime);
    stop = rdtsc_time();

    m_freqRdtsc = static_cast<double>(stop - start) / calibrationTime;
#endif
}

CpuTimer::~CpuTimer()
{
}

void CpuTimer::Reset( bool bResetSum )
{
	m_LastTime = 0.0;
	if( bResetSum )
	{
		m_SumTime = 0.0;
		m_NumFrames = 0;
	}
	else
	{
		++m_NumFrames;
	}
}

void CpuTimer::Start()
{
#if USE_RDTSC
    m_startTime.QuadPart = rdtsc_time();
#else
    QueryPerformanceCounter(&m_startTime);
#endif
}

void CpuTimer::Stop()
{
    LARGE_INTEGER t;
    double freq;

#if USE_RDTSC
    t.QuadPart = rdtsc_time();
    freq = m_freqRdtsc;
#else
    QueryPerformanceCounter(&t);
    freq = m_freq;
#endif

    m_LastTime += static_cast<double>(t.QuadPart - m_startTime.QuadPart) / freq;
	m_SumTime +=  static_cast<double>(t.QuadPart - m_startTime.QuadPart) / freq;
}

void CpuTimer::Delay(double sec)
{
    LARGE_INTEGER start, stop;
    double t;

    QueryPerformanceCounter(&start);

    do
    {
        QueryPerformanceCounter(&stop);

        t = static_cast<double>(stop.QuadPart - start.QuadPart) / m_freq;
    } while (t < sec);
}

//-----------------------------------------------------------------------------

GpuTimer::GpuTimer(ID3D11Device* pDev, UINT64 freq, UINT numTimeStamps ) :
    Timer(),
    m_pDevCtx(NULL),
	m_numTimeStamps(numTimeStamps),
	m_curIssueTs(m_numTimeStamps-1),
	m_nextRetrTs(0),
	m_FrameID(0),
	
	m_CurTime(0.0)
{
    HRESULT hr;

    _ASSERT(pDev != NULL);
	_ASSERT(numTimeStamps>0);

    pDev->GetImmediateContext(&m_pDevCtx);
    _ASSERT(m_pDevCtx != NULL);

    D3D11_QUERY_DESC qd;
    qd.MiscFlags = 0;

	m_ts = new TsRecord[m_numTimeStamps];
    for (UINT i = 0; i < m_numTimeStamps; i++)
    {
		qd.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;

		hr = pDev->CreateQuery(&qd, &m_ts[i].pDisjointTS);
		_ASSERT((hr == S_OK) && (m_ts[i].pDisjointTS != NULL));

		qd.Query = D3D11_QUERY_TIMESTAMP;

        hr = pDev->CreateQuery(&qd, &m_ts[i].pStart);
        _ASSERT((hr == S_OK) && (m_ts[i].pStart != NULL));

        hr = pDev->CreateQuery(&qd, &m_ts[i].pStop);
        _ASSERT((hr == S_OK) && (m_ts[i].pStop != NULL));

		m_ts[i].state = 0;
    }
	m_CurTimeFrame.id = 0;
	m_CurTimeFrame.invalid = 1;

    freq = 0;//prevent warning
}

GpuTimer::~GpuTimer()
{
    for (UINT i = 0; i < m_numTimeStamps; i++)
    {
		SAFE_RELEASE(m_ts[i].pDisjointTS);
        SAFE_RELEASE(m_ts[i].pStart);
        SAFE_RELEASE(m_ts[i].pStop);
    }
	SAFE_DELETE_ARRAY(m_ts);

    SAFE_RELEASE(m_pDevCtx);
}

void GpuTimer::Reset( bool bResetSum )
{
	FinishCollection();
	m_FrameID = (m_FrameID+1) & 0x3FFFFFFF;

	if( bResetSum )
	{
		WaitIdle();
		m_CurTimeFrame.invalid = 1;
		m_CurTime   = 0.0;
		m_LastTime  = 0.0;
		m_SumTime   = 0.0;
		m_NumFrames = 0;
	}
}

void GpuTimer::Start()
{
	if( ++m_curIssueTs == m_numTimeStamps )
		m_curIssueTs = 0;

	if( 0 != m_ts[m_curIssueTs].data.startIssued )
	{
		_ASSERT( false && "CPU stall required! This should never happen. Please increase GpuTimer::NumTimeStamps in Timer.h" );
		CollectData( m_curIssueTs, TRUE );
	}

	m_ts[m_curIssueTs].data.frameID = m_FrameID;
	m_ts[m_curIssueTs].data.startIssued = 1;
	m_ts[m_curIssueTs].data.stopIssued = 0;
	m_pDevCtx->Begin(m_ts[m_curIssueTs].pDisjointTS);
	m_pDevCtx->End(m_ts[m_curIssueTs].pStart);
}

void GpuTimer::Stop()
{
	// check if timestamp start has been issued but no stop yet
	_ASSERT( (m_ts[m_curIssueTs].data.startIssued == 1) && (m_ts[m_curIssueTs].data.stopIssued == 0) );

	m_ts[m_curIssueTs].data.stopIssued = 1;
    m_pDevCtx->End(m_ts[m_curIssueTs].pStop);
	m_pDevCtx->End(m_ts[m_curIssueTs].pDisjointTS);
}

void GpuTimer::WaitIdle()
{
	while( m_nextRetrTs != m_curIssueTs )
	{
		CollectData( m_nextRetrTs, true );

		if( ++m_nextRetrTs == m_numTimeStamps )
			m_nextRetrTs = 0;
	}

	// retrieve the current Ts
	CollectData( m_nextRetrTs, true );
	if( ++m_nextRetrTs == m_numTimeStamps )
		m_nextRetrTs = 0;

    if( 0 == m_CurTimeFrame.invalid )
	{
		m_LastTime = m_CurTime;
		m_SumTime +=  m_CurTime;
		++m_NumFrames;
	}
}

void GpuTimer::FinishCollection()
{
	// retrieve all available timestamps
	while( CollectData(m_nextRetrTs) )
	{
		if( ++m_nextRetrTs == m_numTimeStamps )
			m_nextRetrTs = 0;
	}
}

bool GpuTimer::CollectData(UINT idx, BOOL stall)
{
	if( !m_ts[idx].data.stopIssued )
		return false;
	// start collecting data from a new frame?
	if( m_ts[idx].data.frameID != m_CurTimeFrame.id )
	{
		// if frametimes collected are valid: write them into m_time
		// so m_time always contains the most recent valid timing data
		if( 0 == m_CurTimeFrame.invalid )
		{
			m_LastTime = m_CurTime;
			m_SumTime +=  m_CurTime;
			++m_NumFrames;
		}

		// start collecting time data of the next frame
		m_CurTime = 0.0;
		m_CurTimeFrame.id = m_ts[idx].data.frameID;
		m_CurTimeFrame.invalid = 0;
	}

	// if we want to retrieve the next timing data NOW the CPU will stall
	// increase NumTimeStamps in Timer.h to prevent this from happening
	if( stall )
	{
		HRESULT hr;
		UINT64 start, stop;

		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT tsd;
		do
		{
			hr = m_pDevCtx->GetData(m_ts[idx].pDisjointTS, &tsd, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0);
		} while (hr == S_FALSE);

		_ASSERT(hr == S_OK);

		do
		{
			hr = m_pDevCtx->GetData(m_ts[idx].pStart, &start, sizeof(UINT64), 0);
		} while (hr == S_FALSE);

		_ASSERT(hr == S_OK);

		do
		{
			hr = m_pDevCtx->GetData(m_ts[idx].pStop, &stop, sizeof(UINT64), 0);
		} while (hr == S_FALSE);

		_ASSERT(hr == S_OK);

		if( tsd.Disjoint || ((start & 0xFFFFFFFF) == 0xFFFFFFFF) || ((stop & 0xFFFFFFFF) == 0xFFFFFFFF) )
		{
			// mark current frametime as invalid
			m_CurTimeFrame.invalid = 1;
		}
		else
		{
			m_CurTime += static_cast<double>(stop - start) / static_cast<double>(tsd.Frequency);
		}

		m_ts[idx].state = 0;
		return true;
	}

	// finally try collecting the available data, return false if it was not yet available
	UINT64 start, stop;

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT tsd;
	if( S_FALSE == m_pDevCtx->GetData(m_ts[idx].pDisjointTS, &tsd, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0) )
	{
		return false;
	}
	if( S_FALSE == m_pDevCtx->GetData(m_ts[idx].pStart, &start, sizeof(UINT64), 0) )
	{
		return false;
	}

	if( S_FALSE == m_pDevCtx->GetData(m_ts[idx].pStop, &stop, sizeof(UINT64), 0) )
	{
		return false;
	}

	// all data was available, so evaluate times
	if( tsd.Disjoint || ((start & 0xFFFFFFFF) == 0xFFFFFFFF) || ((stop & 0xFFFFFFFF) == 0xFFFFFFFF) )
	{
		// mark current frametime as invalid
		m_CurTimeFrame.invalid = 1;
	}
	else
	{
		UINT64 dt = (stop - start);
		m_CurTime += static_cast<double>(dt) / static_cast<double>(tsd.Frequency);
	}

	m_ts[idx].state = 0;
	return true;
}

//-----------------------------------------------------------------------------

GpuCpuTimer::GpuCpuTimer(ID3D11Device* pDev) :
    CpuTimer()
{
    HRESULT hr;

    _ASSERT(pDev != NULL);

    pDev->GetImmediateContext(&m_pDevCtx);
    _ASSERT(m_pDevCtx != NULL);

    D3D11_QUERY_DESC qd;
    qd.Query = D3D11_QUERY_EVENT;
    qd.MiscFlags = 0;

    hr = pDev->CreateQuery(&qd, &m_pEvent);
    _ASSERT((hr == S_OK) && (m_pEvent != NULL));
}

GpuCpuTimer::~GpuCpuTimer()
{
    SAFE_RELEASE(m_pEvent);
    SAFE_RELEASE(m_pDevCtx);
}

void GpuCpuTimer::Start()
{
    WaitIdle();
    CpuTimer::Start();
}

void GpuCpuTimer::Stop()
{
    WaitIdle();
    CpuTimer::Stop();
}

void GpuCpuTimer::WaitIdle()
{
    m_pDevCtx->End(m_pEvent);

    HRESULT hr;
    BOOL data;

    do
    {
        hr = m_pDevCtx->GetData(m_pEvent, &data, sizeof(BOOL), 0);
    } while (hr == S_FALSE);

    _ASSERT(hr == S_OK);
}

//-----------------------------------------------------------------------------
// convenience timer functions
//-----------------------------------------------------------------------------

TimingEvent::TimingEvent( ) :
	m_name(NULL),
	m_nameLen(0),
	m_used( false ),
	m_parent(NULL),
	m_firstChild(NULL),
	m_next(NULL)
{
	m_gpu = (NULL != TimerEx::Instance().GetDevice()) ? new GpuTimer( TimerEx::Instance().GetDevice(), 0, 16 ) : NULL;
}

TimingEvent::~TimingEvent()
{
	SAFE_DELETE( m_gpu );
	SAFE_DELETE_ARRAY(m_name);
}

void TimingEvent::SetName(LPCWSTR name)
{
	size_t len_req = wcslen(name)+1;
	if( len_req > m_nameLen )
	{
		SAFE_DELETE_ARRAY(m_name);
		int len_alloc = 32 + (len_req&0xFFFFFFE0);	// round up to next multiple of 32 so when reused we don't have to realloc too often
		m_name = new WCHAR[len_alloc];
		m_nameLen = len_alloc;
	}
	wcscpy_s( m_name, m_nameLen, name);
}

LPCWSTR TimingEvent::GetName( )
{
	return m_name;
}

void TimingEvent::Start( )
{
	m_used = true;
	if( NULL != m_gpu ) m_gpu->Start();
	m_cpu.Start();
}

void TimingEvent::Stop( )
{
	m_cpu.Stop();
	if( NULL != m_gpu ) m_gpu->Stop();
	m_used = true;
}

double TimingEvent::GetTime	( TimerType type, bool stall )
{
	switch( type )
	{
	case ttCpu:
		return m_cpu.GetTime();
	case ttGpu:
		if( NULL != m_gpu )
		{
			if( stall )
				m_gpu->WaitIdle();
			return m_gpu->GetTime();
		}
		// else fallthrough
	default: 
		return 0.0f;
	}
}

double TimingEvent::GetAvgTime( TimerType type, bool stall )
{
	switch( type )
	{
	case ttCpu:
		return m_cpu.GetSumTime()/m_cpu.GetTimeNumFrames();
	case ttGpu:
		if( NULL != m_gpu )
		{
			if( stall )
				m_gpu->WaitIdle();
			return m_gpu->GetSumTime()/m_gpu->GetTimeNumFrames();
		}
		// else fallthrough
	default: 
		return 0.0f;
	}	
}

TimingEvent* TimingEvent::GetTimer(LPCWSTR timerId)
{
	size_t len = wcslen(timerId);
	size_t seperator = wcscspn( timerId, L"/|\\" );

	if( seperator<len )
	{
		LPWSTR idCopy = new WCHAR[len+1];
		wcscpy_s( idCopy, len+1, timerId );
		idCopy[seperator] = 0;

		TimingEvent* te = m_firstChild;
		while( te )
		{
			if( !wcscmp(idCopy, te->m_name ) )
			{
				te = te->GetTimerRec( &idCopy[seperator+1] );
				delete[] idCopy;
				return te;
			}
			te = te->m_next;
		}

		delete[] idCopy;
	}
	else
	{
		TimingEvent* te = m_firstChild;
		while( te )
		{
			if( !wcscmp(timerId, te->m_name ) )
				return te;
			te = te->m_next;
		}
	}
	return NULL;
}

TimingEvent* TimingEvent::GetParent( )
{
	return m_parent;
}

TimingEvent* TimingEvent::GetFirstChild( )
{
	return m_firstChild;
}

TimingEvent* TimingEvent::GetNextTimer( )
{
	return m_next;
}

// when this function is called we know we're working on a copy of the name, so we can "destruct" it
TimingEvent* TimingEvent::GetTimerRec( LPWSTR timerId ) 
{
	size_t len = wcslen(timerId);
	size_t seperator = wcscspn( timerId, L"/|\\" );
	if( seperator<len )
		timerId[seperator] = 0;

	TimingEvent* te = m_firstChild;
	while( te )
	{
		if( !wcscmp(timerId, te->m_name ) )
		{
			if( seperator<len )
				te = te->GetTimerRec( &timerId[seperator+1] );

			return te;
		}
		te = te->m_next;
	}

	return NULL;
}

TimingEvent* TimingEvent::FindLastChildUsed()
{
	TimingEvent* ret = NULL;
	TimingEvent* te = m_firstChild;
	while( te )
	{
		if( te->m_used )
			ret = te;
		te = te->m_next;
	}
	return ret;
}
//-----------------------------------------------------------------------------

TimerEx::TimerEx( ) : 
   m_pDev(NULL),
   m_Current(NULL),
   m_Unused(NULL)
{
};

TimerEx::~TimerEx()
{
	_ASSERT( "Stop() not called for every Start(...)" && (m_Current == NULL) );

	Destroy();
}

void TimerEx::DeleteTimerTree( TimingEvent* te )
{
	// first delete all children
	while( NULL != te )
	{
		DeleteTimerTree( te->m_firstChild );
				
		TimingEvent* tmp = te;
		te = te->m_next;
		delete tmp;
	}
}

void TimerEx::Init( ID3D11Device* pDev )
{
	m_pDev = pDev;
}

void TimerEx::Destroy( )
{
	// delete all unused
	TimingEvent* te = m_Unused;
	while( NULL != te )
	{
		TimingEvent* tmp = te;
		te = te->m_next;
		delete tmp;
	}
	m_Unused = NULL;

	// delete all used
	DeleteTimerTree( m_Root );
	m_Root = NULL;

	m_pDev = NULL;
}

void TimerEx::Reset( TimingEvent* te, bool bResetSum )
{
	TimingEvent* prev = NULL;
	while( NULL != te )
	{
		// recursion
		Reset( te->m_firstChild, bResetSum );

		// reset the timer event
		te->m_cpu.Reset( bResetSum );
		if( NULL != te->m_gpu ) te->m_gpu->Reset( bResetSum );

		if( te->m_used || !bResetSum )
		{
			//if it was used this frame: just reset
			te->m_used = false;
			prev = te;
			te = te->m_next;
		}

		if( bResetSum && !te->m_used )
		{
			te->m_used = false;
			// if it was not used this frame			
			//move to unused timer list
			TimingEvent* tmp = te;
			te = te->m_next;

			if( NULL == prev )
			{
				if( NULL == tmp->m_parent )
					m_Root = tmp->m_next;
				else
					tmp->m_parent->m_firstChild = tmp->m_next;
			}
			else
				prev->m_next = tmp->m_next;

			tmp->m_parent = NULL;
			tmp->m_next = m_Unused;
			m_Unused = tmp;
		}
	}
}

void TimerEx::Reset( bool bResetSum )
{
	_ASSERT( "init not called or called with NULL" && (m_pDev != NULL) );
	_ASSERT( "Stop() not called for every Start(...)" && (m_Current == NULL) );

	if( NULL != m_Root )
		Reset( m_Root, bResetSum );
}

void TimerEx::Start( LPCWSTR timerId )
{
	_ASSERT( "init not called or called with NULL" && (m_pDev != NULL) );

	TimingEvent* te = (NULL == m_Current) ? GetTimer(timerId) : m_Current->GetTimer(timerId);
	if( NULL == te )
	{
		// create new timer event
		if( NULL == m_Unused )
		{
			te = new TimingEvent();
		}
		else
		{
			te = m_Unused;
			m_Unused = te->m_next;
			te->m_next = NULL;
		}

		te->SetName( timerId );
		te->m_parent = m_Current;

		// now look where to insert it
		TimingEvent* lu = NULL;
		if( NULL == m_Current )
		{
			TimingEvent* tmp = m_Root;
			while( tmp )
			{
				if( tmp->m_used )
					lu = tmp;
				tmp = tmp->m_next;
			}
		}
		else
		{
			lu = m_Current->FindLastChildUsed();
		}

		if( NULL != lu )
		{
			te->m_next = lu->m_next;
			lu->m_next = te;
		}
		else
		{
			if( NULL == m_Current )
			{
				te->m_next = m_Root;
				m_Root = te;
			}
			else
			{
				te->m_next = m_Current->m_firstChild;
				m_Current->m_firstChild = te;
			}

		}
	}

	m_Current = te;
	m_Current->Start();
}

void TimerEx::Stop( )
{
	_ASSERT( "init not called or called with NULL" && (m_pDev != NULL) );
	_ASSERT( "Start(...) not called before Stop()" && (m_Current != NULL) );

	m_Current->Stop();
	m_Current = m_Current->m_parent;
}
	
double TimerEx::GetTime( TimerType type, LPCWSTR timerId, bool stall )
{
	_ASSERT( "init not called or called with NULL" && (m_pDev != NULL) );

	TimingEvent* te = NULL;

	if( NULL != m_Current )
		te = m_Current->GetTimer( timerId );

	if( NULL == te )
		te = GetTimer( timerId );
	
	return ( NULL != te ) ? te->GetTime(type, stall) : 0.0;
}

double TimerEx::GetAvgTime( TimerType type, LPCWSTR timerId, bool stall )
{
	_ASSERT( "init not called or called with NULL" && (m_pDev != NULL) );

	TimingEvent* te = NULL;

	if( NULL != m_Current )
		te = m_Current->GetTimer( timerId );

	if( NULL == te )
		te = GetTimer( timerId );
	
	return ( NULL != te ) ? te->GetAvgTime(type, stall) : 0.0;
}


TimingEvent* TimerEx::GetTimer( LPCWSTR timerId )
{
	if( NULL == timerId )
		return m_Root;

	_ASSERT( "init not called" && (m_pDev != NULL) );

	size_t len = wcslen(timerId);
	size_t seperator = wcscspn( timerId, L"/|\\" );
	if( seperator < len )
	{
		LPWSTR idCopy = new WCHAR[len+1];
		wcscpy_s( idCopy, len+1, timerId );
		idCopy[seperator] = 0;
		TimingEvent* te = m_Root;
		while( te )
		{
			if( !wcscmp(idCopy, te->m_name ) )
			{
				te = te->GetTimerRec( &idCopy[seperator+1] );
				delete[] idCopy;
				return te;
			}
			te = te->m_next;
		}
		delete[] idCopy;
	}
	else
	{
		TimingEvent* te = m_Root;
		while( te )
		{
			if( !wcscmp(timerId, te->m_name ) )
				return te;
			te = te->m_next;
		}
	}
	return NULL;
}
