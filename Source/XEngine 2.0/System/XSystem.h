/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XEngine.h Visual Studio 2012 Version.
// It's An XEngine Header File.
// Contain All Direct3D Definitions And Prototypes Used In XEngine System.
// Date Creation: 21 April 2014
// Last Changing: 10 May 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

//Windows Libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"pdh.lib") 

//Direct3D Libraries (DirectX 9)
//#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"dinput.lib")
#pragma comment(lib,"dinput8.lib")

//Direct3D Libraries (DirectX 11)
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment(lib,"dxguid.lib")

//Direct3D Libraries (DirectX 10)
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")

//Windows And System Directives
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <mmsystem.h>

//Math Functions And Conversions Directives
#include <vector>

//Performance Data Helper Directives
#include <pdh.h>

//Direct3D Directives (DirectX 11)
#include <d3d11.h>
#include <d3dx11.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <d3dx10math.h>

//Direct3D Directives (DirectX 10)
#include <d3d10.h>
#include <d3dx10.h>

//Direct3D Directives (DirectX 9)
//#include <d3d9.h>
//#include <d3dx9.h>
//#include <d3dx9core.h>
//#include <d3dx9mesh.h>
#include <dinput.h>

using namespace std;