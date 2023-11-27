/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XEngine.cpp Visual Studio 2012 Version.
// It's An XEngine Source File.
// Contain General 3D Engine Initialization Parameters And Instructions.
// Date Creation: 21 April 2014
// Last Changing: 24 April 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XEngine.h>

///////////////////////////////////////////////////////////////////////////////
// External DLL Definitions And Prototypes 
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#define export extern "C" __declspec(dllexport)
#else
#define export __declspec(dllexport)
#endif

#ifdef __cplusplus
#define import extern "C" __declspec(dllimport)
#else
#define import __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
// Exporting DLL External Definitions And Prototypes 
///////////////////////////////////////////////////////////////////////////////
export void XEngineInit(HWND WndOut,HWND WndInput);
export HRESULT AcceptDirect3D();
export HRESULT AcceptOpenGL();
export void RenderDirect3D();
export void RenderOpenGL();
export void ReleaseDirect3D();
export void SetWindowedMode(BOOL Windowed);
export void SetScreenResolution(int Width,int Height);
export void ConnectCamera(bool State);

////////////////////////////////////////////////////////////////////////////////
// Global Variables Or Prototypes
////////////////////////////////////////////////////////////////////////////////
C3DRenderX* m_pRenderX = NULL;  //Global XEngine Render Interface

//-----------------------------------------------------------------------------
// Name: XEngineInit()
// Desc: XEngine Initialization
//-----------------------------------------------------------------------------
void XEngineInit(HWND WndOut,HWND WndInput)
{
  if (m_pRenderX == NULL) m_pRenderX = new C3DRenderX(WndOut,WndInput);
} //EndXEngineInitProcedure

//-----------------------------------------------------------------------------
// Name: AcceptDirect3D()
// Desc: Accept XEngine Direct3D Parameters
//-----------------------------------------------------------------------------
HRESULT AcceptDirect3D()
{
  //Set Default Parameters
  HRESULT m_pResult = S_OK; //Set Result By Default

  //If RenderX Interface Initialized Then Create And Accept Direct3D Paramters
  if (m_pRenderX == NULL) m_pResult = E_FAIL;
  else m_pResult = m_pRenderX->CreateDirect3D();

  return m_pResult;
} //EndAcceptDirect3DFunction

//-----------------------------------------------------------------------------
// Name: AcceptOpenGL()
// Desc: Accept XEngine OpenGL Parameters
//-----------------------------------------------------------------------------
HRESULT AcceptOpenGL()
{
  //Set Default Parameters
  HRESULT m_pResult = E_FAIL; //Set Result By Default
  //OpenGL Is Not Supported In This Version XEngine
  return m_pResult;
} //EndAcceptOpenGLFunction

//-----------------------------------------------------------------------------
// Name: RenderDirect3D()
// Desc: Direct3D Render Scene And All Objects
//-----------------------------------------------------------------------------
void RenderDirect3D()
{
  if (m_pRenderX != NULL) m_pRenderX->Render();
} //EndRenderDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: RenderOpenGL()
// Desc: OpenGL Render Scene And All Objects
//-----------------------------------------------------------------------------
void RenderOpenGL()
{
  //OpenGL Is Not Supported In This Version XEngine
} //EndRenderOpenGLProcedure

//-----------------------------------------------------------------------------
// Name: SetWindowedMode()
// Desc: 
//-----------------------------------------------------------------------------
void SetWindowedMode(BOOL Windowed)
{
  if (m_pRenderX != NULL) m_pRenderX->SetWindowed(Windowed);
} //EndSetWindowedModeProcedure

//-----------------------------------------------------------------------------
// Name: SetScreenResolution()
// Desc: 
//-----------------------------------------------------------------------------
void SetScreenResolution(int Width,int Height)
{
  if (m_pRenderX != NULL) m_pRenderX->SetScreenResolution(Width,Height);
} //EndSetScreenResolutionProcedure

//-----------------------------------------------------------------------------
// Name: ConnectCamera()
// Desc: 
//-----------------------------------------------------------------------------
void ConnectCamera(bool State)
{  
  if (m_pRenderX != NULL) m_pRenderX->SetCamera(State);
} //EndConnectCameraProcedure

//-----------------------------------------------------------------------------
// Name: ReleaseDirect3D()
// Desc: Release XEngine Direct3D Render
//-----------------------------------------------------------------------------
void ReleaseDirect3D()
{
  //Free RenderX Interface
  if (m_pRenderX != NULL) 
  {
    ReleaseClass(m_pRenderX); 
	DeleteClass(m_pRenderX);
  }
} //EndReleaseDirect3DProcedure

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For C3DRenderX Class
///////////////////////////////////////////////////////////////////////////////
C3DRenderX::C3DRenderX(HWND WinCtrl,HWND WndInput)
{
  //Set Default Parameters
  m_pHWnd = WinCtrl;                        //Set Apropritate Output Window Handle To Class
  m_pHWndInput = WndInput;                  //Set Apropritate Input Window Handle For The Input Devices
  m_pDriverType = D3D_DRIVER_TYPE_NULL;     //Set Driver Type To Null By Default
  m_pFeatureLevel = D3D_FEATURE_LEVEL_11_0; //Set Direct3D Feature Level By Default
  m_pDevice = NULL;                         //Set Direct3D Device To Null By Default
  m_pImmediateContext = NULL;               //Set Direct3D Immediate Context To Null By Default
  m_pSwapChain = NULL;                      //Set Direct3D Swap Chain To Null By Default
  m_pRenderTargetView = NULL;               //Set Direct3D Render Target To Null By Default

  m_pDepthStencilBuffer = NULL;
  m_pDepthStencilState = NULL;
  m_pDepthStencilView = NULL;
  m_pRasterState = NULL;
  m_pRasterStateNoCulling = NULL;

  //Set Default Output D3D Parameters
  OutWidth = 640;
  OutHeight = 480;
  bWindowed = TRUE;
  bVSyncEnabled = VSYNC_ENABLED;

  //Set Default Parameters For XEngine Objects
  Camera = NULL;   //NULL Camera Object
  SkyBox = NULL;   //NULL SkyBox Object
  SkyMap = NULL;   //NULL SkyMap Object
  SkyPlane = NULL; //NULL SkyPlane Object

  m_Fps = NULL;     //NULL FPS And Perfomance Counter By Default  
  m_FpsText = NULL; //NULL Output Fps Text Object By Default   
  m_CpuText = NULL; //NULL Output Cpu Text Object By Default 

} //EndConstructionDirectives

C3DRenderX::~C3DRenderX()
{  
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: C3DRenderX::CreateDirect3D()
// Desc: Create Direct3D Interface
//-----------------------------------------------------------------------------
HRESULT C3DRenderX::CreateDirect3D()
{   
  //Set Default Parameters
  HRESULT m_pResult = S_OK; //Set Result By Default
  UINT CreateDeviceFlags = 0;
  UINT NumModes,Numerator,Denominator,StringLength;
  IDXGIFactory* Factory;
  IDXGIAdapter* Adapter;
  IDXGIOutput* AdapterOutput;
  DXGI_MODE_DESC* DisplayModeList;
  DXGI_ADAPTER_DESC AdapterDesc;

  //Create A DirectX Graphics Interface Factory
  m_pResult = CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&Factory);
  if (FAILED(m_pResult))
  {
    Log("C3DRenderX::CreateDirect3D - Error Get DirectX Interface Factory..");
  }

  //Use The Factory To Create An Adapter For The Primary Graphics Interface (Video Card)
  m_pResult = Factory->EnumAdapters(0,&Adapter);
  if (FAILED(m_pResult))
  {
	Log("C3DRenderX::CreateDirect3D - Error Get Graphic Adapters..");
  }

  //Enumerate The Primary Adapter Output (Monitor)
  m_pResult = Adapter->EnumOutputs(0,&AdapterOutput);
  if (FAILED(m_pResult))
  {
	Log("C3DRenderX::CreateDirect3D - Error Get Output Adapters..");
  }

  //Get The Number Of Modes That Fit The DXGI_FORMAT_R8G8B8A8_UNORM Display Format 
  //For The Adapter Output (Monitor)
  m_pResult = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_ENUM_MODES_INTERLACED,&NumModes,NULL);
  if (FAILED(m_pResult))
  {
	Log("C3DRenderX::CreateDirect3D - Error Get Display Mode List For Output Adapters..");
  }

  //Create A List To Hold All The Possible Display Modes For 
  //This Monitor/Video Card Combination
  DisplayModeList = new DXGI_MODE_DESC[NumModes];
  if (!DisplayModeList)
  {
	Log("C3DRenderX::CreateDirect3D - Error Set Display Mode List Description..");
  }

  //Now Fill The Display Mode List Structures
  m_pResult = AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
  DXGI_ENUM_MODES_INTERLACED,&NumModes,DisplayModeList);
  if (FAILED(m_pResult))
  {
    Log("C3DRenderX::CreateDirect3D - Error Get Display Mode List..");
  }

  //Now Go Through All The Display Modes And Find The One That Matches The Screen Width And Height
  //When A Match Is Found Store The Numerator And Denominator Of The Refresh Rate For That Monitor.
  for (int I = 0; I < NumModes; I++)
  {
	if (DisplayModeList[I].Width == (unsigned int)OutWidth)
	{
	  if(DisplayModeList[I].Height == (unsigned int)OutHeight)
	  {
	    Numerator = DisplayModeList[I].RefreshRate.Numerator;
		Denominator = DisplayModeList[I].RefreshRate.Denominator;
	  }
	}
  }

  //We Now Have The Numerator And Denominator For The Tefresh Tate. The Last Thing 
  //We Will Retrieve Using The Adapter Is The Name Of The Video Card And The Amount 
  //Of Memory On The Video Card.
  //Get The Adapter (Video Card) Description
  m_pResult = Adapter->GetDesc(&AdapterDesc);
  if (FAILED(m_pResult))
  {
	Log("C3DRenderX::CreateDirect3D - Error Get Display Adapter Description..");
  }

  //Store The Dedicated Video Card Memory In Megabytes
  m_VideoCardMemory = (int)(AdapterDesc.DedicatedVideoMemory/1024/1024);

  //Convert The Name Of The Video Card To A Character Array And Store It
  int Error = wcstombs_s(&StringLength,m_VideoCardDescription,128,AdapterDesc.Description,128);
  if (Error != 0)
  {
	Log("C3DRenderX::CreateDirect3D - Error Get Display Adapter Name..");
  }

  //Now That We Have Stored The Numerator And Denominator For The Refresh Rate 
  //And The Video Card Information We Can Release The Structures And Interfaces Used To Get That Information
  DeleteArray(DisplayModeList); //Release The Display Mode List
  ReleaseClass(AdapterOutput); //Release The Adapter Output
  ReleaseClass(Adapter); //Release The Adapter
  ReleaseClass(Factory); //Release The Factory

  //#ifdef _DEBUG
  //CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  //#endif

  D3D_DRIVER_TYPE DriverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,D3D_DRIVER_TYPE_WARP,D3D_DRIVER_TYPE_REFERENCE, };
  UINT NumDriverTypes = ARRAYSIZE(DriverTypes);
  //UINT NumDriverTypes = sizeof(DriverTypes)/sizeof(DriverTypes[0]);

  D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0, };
  UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

  DXGI_SWAP_CHAIN_DESC SwapDesc;

  //Initialize The Swap Chain Description
  ZeroMemory(&SwapDesc,sizeof(SwapDesc));

  //Set To A Single Back Buffer
  SwapDesc.BufferCount = 1;

  //Set The Width And Height Of The Back Buffer
  SwapDesc.BufferDesc.Width = OutWidth;
  SwapDesc.BufferDesc.Height = OutHeight;

  //Set Regular 32-Bit Surface For The Back Buffer
  SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  //Set The Refresh Rate Of The Back Buffer
  if (bVSyncEnabled)
  {
	SwapDesc.BufferDesc.RefreshRate.Numerator = Numerator;
	SwapDesc.BufferDesc.RefreshRate.Denominator = Denominator;
  }
  else
  {
	SwapDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapDesc.BufferDesc.RefreshRate.Denominator = 1;
  }

  //Set The Usage Of The Back Buffer
  SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  //Set The Handle For The Window To Render To
  SwapDesc.OutputWindow = m_pHWnd;

  //Turn Multisampling Off
  SwapDesc.SampleDesc.Count = 1;
  SwapDesc.SampleDesc.Quality = 0;

  //Set To Full Screen Or Windowed Mode
  SwapDesc.Windowed = bWindowed;

  //Set The Scan Line Ordering And Scaling To Unspecified
  SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  //Discard The Back Buffer Contents After Presenting
  SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  //Don't Set The Advanced Flags
  //SwapDesc.Flags = 0;

  for(UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; DriverTypeIndex++)
  {
    m_pDriverType = DriverTypes[DriverTypeIndex];
    m_pResult = D3D11CreateDeviceAndSwapChain(NULL,/*D3D_DRIVER_TYPE_HARDWARE*/m_pDriverType,NULL,
	CreateDeviceFlags,FeatureLevels,NumFeatureLevels,D3D11_SDK_VERSION,&SwapDesc,
	&m_pSwapChain,&m_pDevice,&m_pFeatureLevel,&m_pImmediateContext);
    if (SUCCEEDED(m_pResult)) break;
  }
    
  if (FAILED(m_pResult)) return m_pResult;

  //Create A Render Target View
  ID3D11Texture2D* pBackBuffer;

  //Get The Pointer To The Back Buffer
  m_pResult = m_pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&pBackBuffer);
  if (FAILED(m_pResult)) return m_pResult;

  //Create The Render Target View With The Back Buffer Pointer
  m_pResult = m_pDevice->CreateRenderTargetView(pBackBuffer,NULL,&m_pRenderTargetView);
  
  //Release Pointer To The Back Buffer As We No Longer Need It
  pBackBuffer->Release();
  pBackBuffer = NULL;
  if (FAILED(m_pResult)) return m_pResult;

  //Describe Depth/Stencil Buffer
  D3D11_TEXTURE2D_DESC DepthStencilDesc;
  DepthStencilDesc.Width = OutWidth;
  DepthStencilDesc.Height = OutHeight;
  DepthStencilDesc.MipLevels = 1;
  DepthStencilDesc.ArraySize = 1;
  DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilDesc.SampleDesc.Count = 1;
  DepthStencilDesc.SampleDesc.Quality = 0;
  DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  DepthStencilDesc.CPUAccessFlags = 0; 
  DepthStencilDesc.MiscFlags = 0;

  //Create The Texture For The Depth Buffer Using The Filled Out Description
  m_pResult = m_pDevice->CreateTexture2D(&DepthStencilDesc,NULL,&m_pDepthStencilBuffer);
  if (FAILED(m_pResult)) return m_pResult;

  //Initailze The Depth Stencil View.
  D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
  DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  DepthStencilViewDesc.Texture2D.MipSlice = 0;

  //Create The Depth Stencil View
  m_pResult = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer,/*&DepthStencilViewDesc*/NULL,&m_pDepthStencilView);
  if (FAILED(m_pResult)) return m_pResult;

  //Bind The Render Target View And Depth Stencil Buffer To The Output Render Pipeline.
  m_pImmediateContext->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);

  //Setup The Viewport
  D3D11_VIEWPORT VwPort;
  VwPort.Width = (FLOAT)OutWidth;
  VwPort.Height = (FLOAT)OutHeight;
  VwPort.MinDepth = 0.0f;
  VwPort.MaxDepth = 1.0f;
  VwPort.TopLeftX = 0;
  VwPort.TopLeftY = 0;

  //Create The Viewport
  m_pImmediateContext->RSSetViewports(1,&VwPort);

  //Setup The Raster Description Which Will Determine How And What Polygons Will Be Drawn
  D3D11_RASTERIZER_DESC RasterDesc;
  RasterDesc.AntialiasedLineEnable = false;
  RasterDesc.CullMode = D3D11_CULL_BACK;
  RasterDesc.DepthBias = 0;
  RasterDesc.DepthBiasClamp = 0.0f;
  RasterDesc.DepthClipEnable = true;
  RasterDesc.FillMode = D3D11_FILL_SOLID;
  RasterDesc.FrontCounterClockwise = false;
  RasterDesc.MultisampleEnable = false;
  RasterDesc.ScissorEnable = false;
  RasterDesc.SlopeScaledDepthBias = 0.0f;

  //Create The Rasterizer State From The Description We Just Filled Out
  m_pResult = m_pDevice->CreateRasterizerState(&RasterDesc,&m_pRasterState);
  if (FAILED(m_pResult)) return m_pResult;

  //Now Set The Rasterizer State
  m_pImmediateContext->RSSetState(m_pRasterState);

  //Setup A Raster Description Which Turns Off Back Face Culling
  RasterDesc.AntialiasedLineEnable = false;
  RasterDesc.CullMode = D3D11_CULL_NONE;
  RasterDesc.DepthBias = 0;
  RasterDesc.DepthBiasClamp = 0.0f;
  RasterDesc.DepthClipEnable = true;
  RasterDesc.FillMode = D3D11_FILL_SOLID;
  RasterDesc.FrontCounterClockwise = false;
  RasterDesc.MultisampleEnable = false;
  RasterDesc.ScissorEnable = false;
  RasterDesc.SlopeScaledDepthBias = 0.0f;

  //Create the no culling rasterizer state.
  m_pResult = m_pDevice->CreateRasterizerState(&RasterDesc,&m_pRasterStateNoCulling);
  if (FAILED(m_pResult)) return m_pResult;

  //Clear The Second Depth Stencil State Before Setting The Parameters
  D3D11_DEPTH_STENCIL_DESC DepthDisabledStencilDesc;
  ZeroMemory(&DepthDisabledStencilDesc,sizeof(DepthDisabledStencilDesc));

  //Now Create A Second Depth Stencil State Which Turns Off The Z Buffer For 2D Rendering
  //The Only Difference Is That DepthEnable Is Set To False, All Other Parameters Are 
  //The Same As The Other Depth Stencil State
  DepthDisabledStencilDesc.DepthEnable = false;
  DepthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DepthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
  DepthDisabledStencilDesc.StencilEnable = true;
  DepthDisabledStencilDesc.StencilReadMask = 0xFF;
  DepthDisabledStencilDesc.StencilWriteMask = 0xFF;
  DepthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  DepthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  DepthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  DepthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  DepthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  DepthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  //Create The State Using The Device.
  m_pResult = m_pDevice->CreateDepthStencilState(&DepthDisabledStencilDesc,&m_DepthDisabledStencilState);
  if (FAILED(m_pResult)) return m_pResult;
  
  //Clear The Blend State Description
  D3D11_BLEND_DESC BlendStateDescription;
  ZeroMemory(&BlendStateDescription,sizeof(D3D11_BLEND_DESC));

  //Create An Alpha Enabled Blend State Description
  BlendStateDescription.RenderTarget[0].BlendEnable = TRUE;
  BlendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
  BlendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  BlendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  BlendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  BlendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

  //Create The Blend State Using The Description
  m_pResult= m_pDevice->CreateBlendState(&BlendStateDescription,&m_AlphaEnableBlendingState);
  if (FAILED(m_pResult)) return m_pResult;

  //Modify The Description To Create An Alpha Disabled Blend State Description
  BlendStateDescription.RenderTarget[0].BlendEnable = FALSE;

  //Create The Blend State Using The Description
  m_pResult = m_pDevice->CreateBlendState(&BlendStateDescription,&m_AlphaDisableBlendingState);
  if (FAILED(m_pResult)) return m_pResult;
	



  //Create XEngine Scene Objects
  CreateSceneObjects();

  return S_OK;
} //EndCreateDirect3DProcedure

//-----------------------------------------------------------------------------
// Name: C3DRenderX::CreateSceneObjects()
// Desc: Render The Scene And Objects
//-----------------------------------------------------------------------------
void C3DRenderX::CreateSceneObjects()
{  
  HRESULT HRes;

  //Create Camera Object
  if (Camera == NULL) 
  {
    Camera = new CXCamera(m_pDevice,m_pHWndInput,OutWidth,OutHeight);
	Camera->SetCameraPos(D3DXVECTOR3(0.0f,0.0f,0.0f)); //Set Player Camera Position In Space
	Camera->SetCameraSpeed(70.0f);                     //Set Player Camera Speed
    Camera->SetMouseSpeed(0.3f);                       //Set Mouse Speed
  }

  //Create The Fps And Perfomance Counter Object
  m_Fps = new CXFps;
  if (!m_Fps)
  {
	Log("C3DRenderX::CreateSceneObjects - Error Initialize FPS And Perfomance Counter Object");
	m_Fps = NULL;
  } 
  else 
  {
    //Initialize The FPS And Perfomance Counter Object
	m_Fps->Initialize();
  }

  //Create FPS Output Text Object
  if (m_FpsText == NULL)
  {
    m_FpsText = new CXText(m_pDevice,m_pImmediateContext);  
	m_FpsText->Initialize(OutWidth,OutHeight,Camera->WorldMatrix());
  }

  //Create CPU Output Text Object
  if (m_CpuText == NULL)
  {
    m_CpuText = new CXText(m_pDevice,m_pImmediateContext);  
	m_CpuText->Initialize(OutWidth,OutHeight,Camera->WorldMatrix());
  }


 // DXGI_SURFACE_DESC SurfaceDesc;
 // ZeroMemory(&SurfaceDesc,sizeof(SurfaceDesc));
 // SurfaceDesc.Height = OutHeight;
 // SurfaceDesc.Width = OutWidth;

  //Create SkyBox Object
/*  if (SkyBox == NULL) 
  {
    SkyBox = new CXSkyBox(m_pDevice,m_pImmediateContext);
    SkyBox->CreateBox(50);
	SkyBox->UpdateBox(&SurfaceDesc);
  } */

  //Create SkyMap Object
  if (SkyMap == NULL) 
  {
    SkyMap = new CXSkyMap(m_pDevice,m_pImmediateContext);
    SkyMap->CreateSkyMap(L"\Textures\\SkyMaps\\GrandCanyon.dds");
  }

  //Create SkyPlane Object
  if (SkyPlane == NULL) 
  {
    SkyPlane = new CXSkyPlane(m_pDevice,m_pImmediateContext);
	SkyPlane->Create(L"\Textures\\Clouds\\Cloud0001.dds",L"\Textures\\Clouds\\Perturb0001.dds");
  }

} //EndCreateSceneObjectsProcedure

//-----------------------------------------------------------------------------
// Name: C3DRenderX::SetCamera()
// Desc: Connect To Camera Object
//-----------------------------------------------------------------------------
void C3DRenderX::SetCamera(bool State)
{  
  if (Camera != NULL) Camera->UseCamera(State);
} //EndSetCameraProcedure

//-----------------------------------------------------------------------------
// Name: C3DRenderX::Render()
// Desc: Render The Scene And Objects
//-----------------------------------------------------------------------------
void C3DRenderX::Render()
{  
  if (m_pDevice != NULL && Camera != NULL) 
  {
	//Begin Rendering D3D Scene
    BeginScene();

    //Transform Matrices
    D3DXMATRIXA16 mWorld,mView,mProj,mOrtho,mWorldViewProjection;
         
    //Update Camera Coordinates
    Camera->UpdateCamera(); 

    //Get The Projection & View Matrix From The Camera Class
    mWorld = Camera->WorldMatrix();
    mView = Camera->ViewMatrix();
    mProj = Camera->ProjMatrix();
	mOrtho = Camera->OrthoMatrix();
	mWorldViewProjection = (mWorld*mView*mProj);
	
	//Render The SkyBox
	/*if (SkyBox != NULL) 
	{
	  SkyBox->SetWorldViewProj(&mWorldViewProjection); //Set View And Projection Matrix For SkyBox
	  SkyBox->Render();
	}*/

	//Turn Off The Z-Buffer To Begin All 2D Rendering
	EnableZBuffer(false);

    //Render The SkyMap
	if (SkyMap != NULL) 
	{
	  EnableCulling(false);                            //Turn Off Back Face Culling
	  SkyMap->SetWorldViewProj(&mWorldViewProjection); //Set View And Projection Matrix For SkyBox
	  SkyMap->SetTransformCoord(Camera->Pos());
	  SkyMap->Render();
	}
    
	//Render The SkyPlane
	if (SkyPlane != NULL) 
	{
      //EnableCulling(true); //Turn On Back Face Culling
	  SkyPlane->SetTransformCoord(Camera->Pos());
	  SkyPlane->Render(mWorld,mView,mProj);
	}




	//Turn Off Back Face Culling
	//EnableCulling(false);                           

	//Turn On The Alpha Blending Before Rendering The Text
	EnableAlphaBlending(true);

	//Render The Perfomance Counters
	if (m_Fps)
	{
	  //Update FPS And Perfomance Counters
	  m_Fps->Update();

	  //Set The Frames Per Second
	  UpdateFps(m_Fps->GetFps());

	  //Set The Cpu Usage
      UpdateCpu(m_Fps->GetCpuUsage());
	}

	//Render The FPS Text
	if (m_FpsText != NULL) m_FpsText->Render(mWorld,mOrtho);

	//Render The CPU Text
	if (m_CpuText != NULL) m_CpuText->Render(mWorld,mOrtho);
	
    //Turn Off Alpha Blending After Rendering The Text
	EnableAlphaBlending(false);

	//Turn The Z-Buffer Back On Now That All 2D Rendering Has Completed
	EnableZBuffer(true);

	//Turn On Back Face Culling
    EnableCulling(true);

	//End Rendering D3D Scene
    EndScene();
  }
} //EndRenderProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::BeginScene()
// Desc: 
//------------------------------------------------------------------------------
void C3DRenderX::BeginScene()
{
  if (m_pImmediateContext)
  {
	//Just Clear The Backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //Red, Green, Blue, Alpha (RGBA)
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView,ClearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
  }
} //EndBeginSceneProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::EndScene()
// Desc: 
//------------------------------------------------------------------------------
void C3DRenderX::EndScene()
{
  if (m_pSwapChain) 
  {
    //Present The Back Buffer To The Screen Since Rendering Is Complete
    if (bVSyncEnabled)
    {
      //Lock To Screen Refresh Rate
	  m_pSwapChain->Present(1,0);
    }
    else
    {
	  //Present As Fast As Possible
	  m_pSwapChain->Present(0,0);
    }
  }
} //EndEndSceneProcedure

//-----------------------------------------------------------------------------
// Name: C3DRenderX::UpdateFps()
// Desc: 
//-----------------------------------------------------------------------------
bool C3DRenderX::UpdateFps(int Fps)
{
  char TempString[16];
  char FpsString[16];
  float Red,Green,Blue;
  bool HRes;
  
  //Truncate The FPS To Below 10,000.
  if (Fps > 9999)
  {
	Fps = 9999;
  }

  //Convert The Fps Integer To String Format
  _itoa_s(Fps,TempString,10);

  //Setup The Fps String
  strcpy_s(FpsString,"Fps: ");
  strcat_s(FpsString,TempString);

  //If Fps Is 60 Or Above Set The Fps Color To Green
  if (Fps >= 60)
  {
	Red = 0.0f;
	Green = 1.0f;
	Blue = 0.0f;
  }

  //If Fps Is Below 60 Set The Fps Color To Yellow
  if (Fps < 60)
  {
	Red = 1.0f;
	Green = 1.0f;
	Blue = 0.0f;
  }

  //If Fps Is Below 30 Set The Fps Color To Red
  if (Fps < 30)
  {
	Red = 1.0f;
	Green = 0.0f;
	Blue = 0.0f;
  }

  //Update The FPS Information Text
  HRes = m_FpsText->SetText(FpsString,20,20,Red,Green,Blue);
  if (!HRes) return false;
 
  return true;
} //EndUpdateFpsFunction

//-----------------------------------------------------------------------------
// Name: C3DRenderX::UpdateCpu()
// Desc: 
//-----------------------------------------------------------------------------
bool C3DRenderX::UpdateCpu(int Cpu)
{
  char TempString[16];
  char CpuString[16];
  bool HRes;

  //Convert The Cpu Integer To String Format
  _itoa_s(Cpu,TempString,10);

  //Setup the Cpu String
  strcpy_s(CpuString,"Cpu: "); 
  strcat_s(CpuString,TempString);
  strcat_s(CpuString,"%");

  //Update The CPU Information Text
  HRes = m_CpuText->SetText(CpuString,20,40,0.0f,1.0f,0.0f);
  if (!HRes) return false;
 
  return true;
} //EndUpdateCpuFunction

//------------------------------------------------------------------------------
// Name: C3DRenderX::GetVideoCardInfo()
// Desc: 
//-----------------------------------------------------------------------------
void C3DRenderX::GetVideoCardInfo(char* CardName,int& CardMemory)
{
  strcpy_s(CardName,128,m_VideoCardDescription);
  CardMemory = m_VideoCardMemory;
} //EndGetVideoCardInfoProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::EnableZBuffer()
// Desc: 
//------------------------------------------------------------------------------
void C3DRenderX::EnableZBuffer(bool State)
{
  if (m_pImmediateContext) 
  {
	if (State) m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState,1);
	else m_pImmediateContext->OMSetDepthStencilState(m_DepthDisabledStencilState,1);
  }
} //EndEnableZBufferProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::EnableAlphaBlending()
// Desc: 
//------------------------------------------------------------------------------
void C3DRenderX::EnableAlphaBlending(bool State)
{
  if (m_pImmediateContext)
  {
    float BlendFactor[4];

    //Setup The Blend Factor
    BlendFactor[0] = 0.0f;
    BlendFactor[1] = 0.0f;
    BlendFactor[2] = 0.0f;
    BlendFactor[3] = 0.0f;
	
    //Turn On The Alpha Blending
	if (State) m_pImmediateContext->OMSetBlendState(m_AlphaEnableBlendingState,BlendFactor,0xffffffff);
    
	//Turn Off The Alpha Blending
	else m_pImmediateContext->OMSetBlendState(m_AlphaDisableBlendingState,BlendFactor,0xffffffff);
  }
} //EndEnableAlphaBlendingProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::EnableCulling()
// Desc: 
//------------------------------------------------------------------------------
void C3DRenderX::EnableCulling(bool State)
{
  //Set The Culling Rasterizer State
  if (State) m_pImmediateContext->RSSetState(m_pRasterState); 
  //Set The No Back Face Culling Rasterizer State
  else m_pImmediateContext->RSSetState(m_pRasterStateNoCulling); 
} //EndEnableCullingProcedure

//------------------------------------------------------------------------------
// Name: C3DRenderX::Release()
// Desc: Release Direct3D Devices And Free All Engine Resources
//------------------------------------------------------------------------------
void C3DRenderX::Release()
{
  //Before Shutting Down Set To Windowed Mode Or When You Release 
  //The Swap Chain It Will Throw An Exception
  if (m_pSwapChain)
  {
    if (!bWindowed) m_pSwapChain->SetFullscreenState(false,NULL);
  }

  //Release Alpha Blend States
  ReleaseClass(m_AlphaEnableBlendingState);
  ReleaseClass(m_AlphaDisableBlendingState);

  //Release Culling State
  ReleaseClass(m_pRasterStateNoCulling);
  
  //Release Raster State
  ReleaseClass(m_pRasterState);

  //Clear The Direct3D State
  if (m_pImmediateContext) m_pImmediateContext->ClearState();
  
  //Release The Direct3D Device Resources
  ReleaseClass(m_DepthDisabledStencilState);
  
  if (m_pRenderTargetView) m_pRenderTargetView->Release();
  if (m_pDepthStencilView) m_pDepthStencilView->Release();
  if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
  if (m_pSwapChain) m_pSwapChain->Release();
  if (m_pImmediateContext) m_pImmediateContext->Release();
  if (m_pDevice) m_pDevice->Release();

  //ReleaseClass(SkyBox); //Release SkyBox Object
  ReleaseClass(SkyMap);   //Release SkyMap Object
  ReleaseClass(SkyPlane); //Release SkyPlane Object
  ReleaseClass(Camera);   //Release Camera Object

  ReleaseClass(m_Fps);     //Release FPS And Perfomance Counter
  ReleaseClass(m_FpsText); //Release Fps Text Output Object
  ReleaseClass(m_CpuText); //Release Cpu Text Output Object

} //EndReleaseProcedure

//------------------------------------------------------------------------------