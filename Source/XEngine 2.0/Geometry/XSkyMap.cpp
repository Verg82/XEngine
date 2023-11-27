/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyMap.cpp Visual Studio 2012 Version.
// It's An XEngine SkyMap Implementation Source File.
// Date Creation: 21 April 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSkyMap.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXSkyMap Class
//////////////////////////////////////////////////////////////////////////////
CXSkyMap::CXSkyMap(ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
{
  //Set Default Parameters
  d_pDevice = pDevice;            //To Appropriate Device Pointer To Class
  d_pImmediateContext = pContext; //To Appropriate Context Pointer To Class

  m_pSVB = NULL;                  //NULL SkyMap Sphere Index Buffer
  m_pSIB = NULL;                  //NULL SkyMap Sphere Vertex Buffer
  m_pSam = NULL;                  //NULL SkyMap Texture Sampler
  m_pSMResView = NULL;            //NULL SkyMap Resource View (Cube Texture)
  m_pVertexShader = NULL;         //NULL SkyMap Vertex Shader
  m_pPixelShader = NULL;          //NULL SkyMap Pixel Shader
  m_pVertexLayout = NULL;         //NULL Vertex Layout
  m_pcbVSPerObject = NULL;        //NULL Vertex Shader Per Object
  m_pDepthStencilState = NULL;    //NULL Depth Stencil State
  RSCullNone = NULL;
} //EndConstructionDirectives

CXSkyMap::~CXSkyMap()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CXSkyMap::Release()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyMap::Release()
{
  ReleaseClass(m_pSVB);               //Release SkyMap Sphere Vertex Buffer
  ReleaseClass(m_pSIB);               //Release SkyMap Sphere Index Buffer
  ReleaseClass(m_pSam);               //Release SkyMap Texture Sampler
  ReleaseClass(m_pSMResView);         //Release SkyMap Resource View (Cube Texture)
  ReleaseClass(m_pVertexShader);      //Release SkyMap Vertex Shader
  ReleaseClass(m_pPixelShader);       //Release SkyMap Pixel Shader
  ReleaseClass(m_pVertexLayout);      //Release Vertex Layout
  ReleaseClass(m_pcbVSPerObject);     //Release Vertex Shader Per Object
  ReleaseClass(m_pDepthStencilState); //Release Depth Stencil State
  ReleaseClass(RSCullNone);

  d_pImmediateContext = NULL;     //NULL Immediate Context Pointer
  d_pDevice = NULL;               //NULL Device Pointer
} //EndReleaseProcedure

//------------------------------------------------------------------------------
// Name: CXSkyMap::Render()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyMap::Render()
{
  //Set Default Parameters
  HRESULT HRes;
  
  //Transformation Matrices
  D3DXMATRIX Scale,Translation;
  D3DXMATRIX WVP;

  //Reset Sphere World
  D3DXMatrixIdentity(&m_pSphereWorld);
  
  //Define SphereWorld's World Space Matrix
  D3DXMatrixScaling(&Scale, 5.0f, 5.0f, 5.0f);
  	
  //Make Sure The Sphere Is Always Centered Around Camera
  D3DXMatrixTranslation(&Translation,VecPos.x,VecPos.y,VecPos.z);

  //Set SphereWorld's World Space Using The Transformations
  m_pSphereWorld = Scale*Translation;

  //Set the Vertex Buffer
  UINT uStrides = sizeof(SKYMAP_VERTEX);
  UINT uOffsets = 0;
  	
  //Set The Spheres Vertex Buffer
  d_pImmediateContext->IASetVertexBuffers(0,1,&m_pSVB,&uStrides,&uOffsets);

  //Set The Spheres Index Buffer
  d_pImmediateContext->IASetIndexBuffer(m_pSIB,DXGI_FORMAT_R32_UINT,0);

  //Set The WVP Matrix And Send It To The Constant Buffer In Effect File 
  //WVP = m_pSphereWorld**m_pWorldViewProj;
  D3DXMatrixMultiply(&WVP,&m_pSphereWorld,m_pWorldViewProj);
  D3DXMatrixTranspose(&pVSPerObject.m_WorldViewProj,&WVP);

  d_pImmediateContext->UpdateSubresource(m_pcbVSPerObject,0,NULL,&pVSPerObject,0,0);
  d_pImmediateContext->VSSetConstantBuffers(0,1,&m_pcbVSPerObject);

  //Send Our SkyMap Resource View To Pixel Shader
  d_pImmediateContext->PSSetShaderResources(0,1,&m_pSMResView);

  //Set The New VS And PS shaders
  d_pImmediateContext->VSSetShader(m_pVertexShader,NULL,0);
  d_pImmediateContext->PSSetShader(m_pPixelShader,NULL,0);

  //Set The New Depth/Stencil And RS States
  d_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState,0);
  d_pImmediateContext->RSSetState(RSCullNone);
  d_pImmediateContext->DrawIndexed(NumSphereFaces*3,0,0);
} //EndRenderProcedure

//------------------------------------------------------------------------------
// Name: CXSkyMap::CreateSkyMap()
// Desc: 
//------------------------------------------------------------------------------
HRESULT CXSkyMap::CreateSkyMap(WCHAR* SkyTexture)
{
  //Set Default Parameters
  HRESULT HRes;
  UINT SupportCaps = 0;
  UINT NumVertLayout = ARRAYSIZE(g_aSkyMapVertexLayout);

  //Update SkyMap Sphere
  UpdateSkyMap(10,10);

  d_pDevice->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT,&SupportCaps);
  if (SupportCaps & D3D11_FORMAT_SUPPORT_TEXTURECUBE &&
      SupportCaps & D3D11_FORMAT_SUPPORT_RENDER_TARGET &&
      SupportCaps & D3D11_FORMAT_SUPPORT_TEXTURE2D)
  {
    //Tell D3D We Will Be Loading A Cube Texture
	D3DX11_IMAGE_LOAD_INFO LoadInfo;
	LoadInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//Load The Cube Texture
	ID3D11Texture2D* SMTexture = 0;
	HRes = D3DX11CreateTextureFromFile(d_pDevice,SkyTexture,&LoadInfo,0,(ID3D11Resource**)&SMTexture,0);
	if (FAILED(HRes))
    {
      Log("CXSkyMap::CreateSkyMap - Error Load Cube Texture");
      return HRes;
    }
	
	//Create The Textures Description
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	//Tell D3D We Have A Cube Texture, Which Is An Array Of 2D Textures
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	//Create The Resource View
	HRes = d_pDevice->CreateShaderResourceView(SMTexture,&SMViewDesc,&m_pSMResView);
	if (FAILED(HRes))
    {
      Log("CXSkyMap::CreateSkyMap - Error Create Shader Resource View");
      return HRes;
    }
  }
  else
  {
    Log("CXSkyMap::CreateSkyMap - Sorry Cube Textures Do Not Supported Of Your Graphics Device");
    return E_FAIL;
  } 

  //Create And Compile The Shaders
  ID3DBlob* pBlobVS = NULL;
  ID3DBlob* pBlobPS = NULL;

  HRes = CompileShaderFromFile(L"\Effects\\SkyMap.fx","SKYMAP_VS","vs_4_0",&pBlobVS);
  if (FAILED(HRes))
  {
	Log("CXSkyMap::CreateSkyMap - Error Compile Vertex Shader From File");
    return HRes;
  }

  HRes = CompileShaderFromFile(L"\Effects\\SkyMap.fx","SKYMAP_PS","ps_4_0",&pBlobPS);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Compile Pixel Shader From File");
    return HRes;
  }

  HRes = d_pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(),pBlobVS->GetBufferSize(),NULL,&m_pVertexShader);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Vertex Shader");
    return HRes; 
  }

  HRes = d_pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(),pBlobPS->GetBufferSize(),NULL,&m_pPixelShader);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Pixel Shader");
    return HRes; 
  }
 
  //Create An Input Layout
  HRes = d_pDevice->CreateInputLayout(g_aSkyMapVertexLayout,NumVertLayout,pBlobVS->GetBufferPointer(),
  pBlobVS->GetBufferSize(),&m_pVertexLayout);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Input Layout");
    return HRes; 
  }

  //Set The Input Layout
  d_pImmediateContext->IASetInputLayout(m_pVertexLayout);

  //Set Primitive Topology
  d_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  ReleaseClass(pBlobVS);
  ReleaseClass(pBlobPS);

  //Create The Buffer To Send To The Constant Buffer In Effect File
  D3D11_BUFFER_DESC BufDesc;	
  ZeroMemory(&BufDesc,sizeof(D3D11_BUFFER_DESC));
  BufDesc.Usage = D3D11_USAGE_DEFAULT;
  BufDesc.ByteWidth = sizeof(CB_VS_PER_OBJECT);
  BufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  BufDesc.CPUAccessFlags = 0;
  BufDesc.MiscFlags = 0;
  HRes = d_pDevice->CreateBuffer(&BufDesc,NULL,&m_pcbVSPerObject);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Constant Buffer");
    return HRes; 
  }

  //Query Support For Linear Filtering On DXGI_FORMAT_R32G32B32A32
  UINT FormatSupport = 0;
  HRes = d_pDevice->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT,&FormatSupport);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Check Format Support");
    return HRes; 
  }

  //Setup Linear Or Point Sampler According To The Format Query Result
  D3D11_SAMPLER_DESC SampDesc;
  ZeroMemory(&SampDesc,sizeof(SampDesc));
  SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;    
  SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  SampDesc.MinLOD = 0;
  SampDesc.MaxLOD = D3D11_FLOAT32_MAX;

  //Create the Sample State
  HRes = d_pDevice->CreateSamplerState(&SampDesc,&m_pSam);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Sampler State");
    return HRes; 
  }

  //Rasterizer State
  D3D11_RASTERIZER_DESC CullMDesc;
  ZeroMemory(&CullMDesc,sizeof(D3D11_RASTERIZER_DESC));
  CullMDesc.FillMode = D3D11_FILL_SOLID;
  CullMDesc.CullMode = D3D11_CULL_NONE;
  HRes = d_pDevice->CreateRasterizerState(&CullMDesc,&RSCullNone);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Rasteriser State");
    return HRes; 
  }

  //Depth Stencil State
  D3D11_DEPTH_STENCIL_DESC DssDesc;
  ZeroMemory(&DssDesc,sizeof(D3D11_DEPTH_STENCIL_DESC));
  DssDesc.DepthEnable = true;
  DssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  HRes = d_pDevice->CreateDepthStencilState(&DssDesc,&m_pDepthStencilState);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::CreateSkyMap - Error Create Depth Stencil State");
    return HRes; 
  }

  return S_OK;
} //EndCreateSkyMapFunction

//------------------------------------------------------------------------------
// Name: CXSkyMap::UpdateSkyMap()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyMap::UpdateSkyMap(int LatLines,int LongLines)
{
  //Set Default Parameters
  HRESULT HRes;
  if (d_pDevice == NULL) return;

  //Create SkyMap Sphere
  D3DXMATRIX RotationX,RotationY,RotationZ;

  NumSphereVertices = ((LatLines-2)*LongLines)+2;
  NumSphereFaces  = ((LatLines-3)*(LongLines)*2)+(LongLines*2);

  float SphereYaw = 0.0f;
  float SpherePitch = 0.0f;

  std::vector<SKYMAP_VERTEX> Vertices(NumSphereVertices);
  D3DXVECTOR3 CurrVertPos = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f);
  D3DXVECTOR3 NullVecPos = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f);

  Vertices[0].Pos.x = 0.0f;
  Vertices[0].Pos.y = 0.0f;
  Vertices[0].Pos.z = 1.0f;

  for (DWORD I = 0; I < LatLines-2; ++I)
  {
	SpherePitch = (I+1)*(3.14/(LatLines-1));
	D3DXMatrixRotationX(&RotationX,SpherePitch);
	for (DWORD J = 0; J < LongLines; ++J)
	{
	  SphereYaw = J*(6.28/(LongLines));
	  D3DXMatrixRotationZ(&RotationY,SphereYaw);
	  D3DXVec3TransformNormal(&CurrVertPos,&NullVecPos,&(RotationX*RotationY));
	  D3DXVec3Normalize(&CurrVertPos,&CurrVertPos);
	  Vertices[I*LongLines+J+1].Pos.x = CurrVertPos.x;  
	  Vertices[I*LongLines+J+1].Pos.y = CurrVertPos.y;
	  Vertices[I*LongLines+J+1].Pos.z = CurrVertPos.z;
	}
  }
	
  Vertices[NumSphereVertices-1].Pos.x =  0.0f;
  Vertices[NumSphereVertices-1].Pos.y =  0.0f;
  Vertices[NumSphereVertices-1].Pos.z = -1.0f;

  D3D11_BUFFER_DESC VertexBufferDesc;
  ZeroMemory(&VertexBufferDesc,sizeof(VertexBufferDesc));
  VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  VertexBufferDesc.ByteWidth = sizeof(SKYMAP_VERTEX)*NumSphereVertices;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = 0;
  VertexBufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA VertexBufferData; 
  ZeroMemory(&VertexBufferData,sizeof(VertexBufferData));
  VertexBufferData.pSysMem = &Vertices[0];
  HRes = d_pDevice->CreateBuffer(&VertexBufferDesc,&VertexBufferData,&m_pSVB);
  if (FAILED(HRes))
  {
    Log("CXSkyMap::UpdateSkyMap - Error Create SkyMap Sphere Vertex Buffer");
	//DeleteArray(Vertices);
    return; 
  }

  std::vector<DWORD> Indices(NumSphereFaces*3);
  int K = 0;

  for (DWORD L = 0; L < LongLines-1; ++L)
  {
    Indices[K] = 0;
	Indices[K+1] = L+1;
	Indices[K+2] = L+2;
	K+= 3;
  }

  Indices[K] = 0;
  Indices[K+1] = LongLines;
  Indices[K+2] = 1;
  K+= 3;

  for (DWORD I = 0; I < LatLines-3; ++I)
  {
	for (DWORD J = 0; J < LongLines-1; ++J)
	{
	  Indices[K] = I*LongLines+J+1;
	  Indices[K+1] = I*LongLines+J+2;
	  Indices[K+2] = (I+1)*LongLines+J+1;
      Indices[K+3] = (I+1)*LongLines+J+1;
	  Indices[K+4] = I*LongLines+J+2;
	  Indices[K+5] = (I+1)*LongLines+J+2;
      K+= 6; //Next Quad
	}

	Indices[K] = (I*LongLines)+LongLines;
	Indices[K+1] = (I*LongLines)+1;
	Indices[K+2] = ((I+1)*LongLines)+LongLines;
    Indices[K+3] = ((I+1)*LongLines)+LongLines;
	Indices[K+4] = (I*LongLines)+1;
	Indices[K+5] = ((I+1)*LongLines)+1;
    K+= 6;
  }

  for (DWORD L = 0; L < LongLines-1; ++L)
  {
	Indices[K] = NumSphereVertices-1;
	Indices[K+1] = (NumSphereVertices-1)-(L+1);
	Indices[K+2] = (NumSphereVertices-1)-(L+2);
	K+= 3;
  }

  Indices[K] = NumSphereVertices-1;
  Indices[K+1] = (NumSphereVertices-1)-LongLines;
  Indices[K+2] = NumSphereVertices-2;

  D3D11_BUFFER_DESC IndexBufferDesc;
  ZeroMemory(&IndexBufferDesc,sizeof(IndexBufferDesc));
  IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  IndexBufferDesc.ByteWidth = sizeof(DWORD)*NumSphereFaces*3;
  IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  IndexBufferDesc.CPUAccessFlags = 0;
  IndexBufferDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA iInitData;
  iInitData.pSysMem = &Indices[0];
  HRes = d_pDevice->CreateBuffer(&IndexBufferDesc,&iInitData,&m_pSIB); 
  if (FAILED(HRes))
  {
    Log("CXSkyMap::UpdateSkyMap - Error Create SkyMap Sphere Index Buffer");
    return; 
  }
} //EndUpdateSkyMapProcedure

//-----------------------------------------------------------------------------