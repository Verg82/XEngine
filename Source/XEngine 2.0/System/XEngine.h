/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XEngine.h Visual Studio 2012 Version.
// It's An XEngine Header File.
// Contain General 3D Engine Initialization Parameters And Instructions.
// Date Creation: 21 April 2014
// Last Changing: 23 April 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
//#pragma once
#include <XSystem.h>
#include <XMethods.h>
#include <XCamera.h>
#include <XFps.h>
#include <XTextRender.h>
#include <XSkyBox.h>
#include <XSkyMap.h>
#include <XSkyPlane.h>
#include <XForwardPlus.h>

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Constants
////////////////////////////////////////////////////////////////////////////////
const bool VSYNC_ENABLED = false;

////////////////////////////////////////////////////////////////////////////////
// Classes Used By This Header
////////////////////////////////////////////////////////////////////////////////
//class C3DEngine;

//-----------------------------------------------------------------------------
// Name: C3DEngine
// Desc: The XEngine 3D Render Class
//-----------------------------------------------------------------------------
#if !defined(AFX_3DRENDERX_H__)
#define AFX_3DRENDERX_H__

class C3DRenderX
{
public:
  //Public Directives
  C3DRenderX(HWND WinCtrl,HWND WndInput);
  virtual ~C3DRenderX();
  void Release();

  HRESULT CreateDirect3D();
  void CreateSceneObjects();
  void Render();

  //Additional Render Directives
  void SetWindowed(BOOL State) { bWindowed = State; }
  void SetVSync(BOOL State) { bVSyncEnabled = State; }
  void SetScreenResolution(int Width,int Height) { OutWidth = Width; OutHeight = Height; }
  void SetCamera(bool State);

  ID3D11Device* GetDevice() { return m_pDevice; }
  ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
  void GetVideoCardInfo(char*,int&);

  //void SetProjection(D3DXMATRIX MatProj) { g_Projection = MatProj; }
  //void SetSwapEffect(D3DSWAPEFFECT Value) { SwapEffect = Value; }
  //void SetAutoDepthStencil(BOOL State) { bAutoDepthStencil = State; }
  //void SetAutoDepthStencilFormat(D3DFORMAT Value) { AutoDepthStencilFormat = Value; }
  //void SetBackBufferCount(UINT Value) { BackBufferCount = Value; }
  //void SetMultiSampleType(D3DMULTISAMPLE_TYPE Value) { MultiSampleType = Value; }
  //void SetMultiSampleQuality(DWORD Value) { MultiSampleQuality = Value; }
  //void SetPresentationInterval(UINT Value) { PresentationInterval = Value; } 

private:
  //Private Directives
  D3D_DRIVER_TYPE m_pDriverType;               //Direct3D Driver Type
  D3D_FEATURE_LEVEL m_pFeatureLevel;           //Direct3D Feature Level
  ID3D11Device* m_pDevice;                     //Direct3D Device Interface (DirectX 11)
  ID3D11DeviceContext* m_pImmediateContext;    //Direct3D Immediate Context
  IDXGISwapChain* m_pSwapChain;                //Direct3D Swap Chain
  ID3D11RenderTargetView* m_pRenderTargetView; //Direct3D Render Target
  
  //Videocard Description Directives
  int m_VideoCardMemory;                       //Store Videocard Memory
  char m_VideoCardDescription[128];            //Store Videocard Description

  ID3D11Texture2D* m_pDepthStencilBuffer;
  ID3D11DepthStencilState* m_pDepthStencilState;
  ID3D11DepthStencilView* m_pDepthStencilView;
  ID3D11RasterizerState* m_pRasterState;

  ID3D11DepthStencilState* m_DepthDisabledStencilState;
  ID3D11BlendState* m_AlphaEnableBlendingState;
  ID3D11BlendState* m_AlphaDisableBlendingState;
  ID3D11RasterizerState* m_pRasterStateNoCulling;

  HWND m_pHWnd;                                //Output Window Handle
  HWND m_pHWndInput;                           //Input Devices Window Handle

  //D3D Param Directives
  BOOL bWindowed;
  bool bVSyncEnabled;   //Store The VSync Setting
  int OutHeight;
  int OutWidth;

  //D3DSWAPEFFECT       SwapEffect;
  //BOOL                bAutoDepthStencil;
  //D3DFORMAT           AutoDepthStencilFormat;
  //UINT                BackBufferCount;
  //D3DMULTISAMPLE_TYPE MultiSampleType;
  //DWORD               MultiSampleQuality;
  //UINT                PresentationInterval;

  //XEngine Objects Directives
  CXSkyBox* SkyBox;     //SkyBox Object
  CXSkyMap* SkyMap;     //SkyMap Object
  CXSkyPlane* SkyPlane; //SkyPlane Object
  CXCamera* Camera;     //Camera Object

  //Perfomance Counter Directives
  CXFps* m_Fps;        //FPS And Perfomance Counter  
  CXText* m_FpsText;   //FPS Text Output Object
  CXText* m_CpuText;   //CPU Text Output Object
  bool UpdateFps(int);  
  bool UpdateCpu(int);
  
  //D3D Rendering Directives
  void BeginScene();
  void EndScene();
  void EnableZBuffer(bool State);
  void EnableAlphaBlending(bool State);
  void EnableCulling(bool State);
public:
  //Public Directives
  //XEngine Objects Directives

protected:
  //Protected Directives
};
#endif //!defined(AFX_3DRENDERX_H__)

//-----------------------------------------------------------------------------