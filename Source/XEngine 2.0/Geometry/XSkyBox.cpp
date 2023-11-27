/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyBox.cpp Visual Studio 2012 Version.
// It's An XEngine SkyBox Source File.
// Date Creation: 21 April 2014
// Last Changing: 07 May 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSkyBox.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXSkyBox Class
//////////////////////////////////////////////////////////////////////////////
CXSkyBox::CXSkyBox(ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
{
  //Set Default Parameters
  d_pDevice = pDevice;            //To Appropriate Device Pointer To Class
  d_pImmediateContext = pContext; //To Appropriate Context Pointer To Class

  m_pEnvironmentMap = NULL;
  m_pEnvironmentRV = NULL;
  m_pVertexShader = NULL;
  m_pPixelShader = NULL;
  m_pSam = NULL;
  m_pVertexLayout = NULL;
  m_pcbVSPerObject = NULL;
  m_pVB = NULL;
  m_pDepthStencilState = NULL;
  m_fCubeSize = 1.0f;
  
} //EndConstructionDirectives

CXSkyBox::~CXSkyBox()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CXSkyBox::Release()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyBox::Release()
{
  ReleaseClass(m_pVB);
  ReleaseClass(m_pEnvironmentMap);
  ReleaseClass(m_pEnvironmentRV);
  ReleaseClass(m_pSam);
  ReleaseClass(m_pVertexShader);
  ReleaseClass(m_pPixelShader);
  ReleaseClass(m_pVertexLayout);
  ReleaseClass(m_pcbVSPerObject);
  ReleaseClass(m_pDepthStencilState);

  d_pImmediateContext = NULL; //NULL Immediate Context Pointer
  d_pDevice = NULL; //NULL Device Pointer
} //EndReleaseProcedure

//------------------------------------------------------------------------------
// Name: CXSkyBox::Render()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyBox::Render()
{
  //Set Default Parameters
  HRESULT HRes;
    
  d_pImmediateContext->IASetInputLayout(m_pVertexLayout);

  UINT uStrides = sizeof(SKYBOX_VERTEX);
  UINT uOffsets = 0;
  ID3D11Buffer* pBuffers[1] = { m_pVB };

  d_pImmediateContext->IASetVertexBuffers(0,1,pBuffers,&uStrides,&uOffsets);
  d_pImmediateContext->IASetIndexBuffer(NULL,DXGI_FORMAT_R32_UINT,0);
  d_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  d_pImmediateContext->VSSetShader(m_pVertexShader,NULL,0);
  d_pImmediateContext->PSSetShader(m_pPixelShader,NULL,0);

  D3D11_MAPPED_SUBRESOURCE MappedResource;
  HRes = d_pImmediateContext->Map(m_pcbVSPerObject,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes)) return;
  
  CB_VS_PER_OBJECT* pVSPerObject = (CB_VS_PER_OBJECT*)MappedResource.pData;  

  D3DXMatrixInverse(&pVSPerObject->m_WorldViewProj,NULL,m_pWorldViewProj);

  d_pImmediateContext->Unmap(m_pcbVSPerObject,0);
  d_pImmediateContext->VSSetConstantBuffers(0,1,&m_pcbVSPerObject);
  d_pImmediateContext->PSSetSamplers(0,1,&m_pSam);
  d_pImmediateContext->PSSetShaderResources(0,1,&m_pEnvironmentRV);

  ID3D11DepthStencilState* pDepthStencilStateStored = NULL;
  UINT StencilRef;
  d_pImmediateContext->OMGetDepthStencilState(&pDepthStencilStateStored,&StencilRef);
  d_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState,0);
  d_pImmediateContext->Draw(4,0);
  d_pImmediateContext->OMSetDepthStencilState(pDepthStencilStateStored,StencilRef);
} //EndRenderProcedure

//------------------------------------------------------------------------------
// Name: CXSkyBox::CreateBox()
// Desc: 
//------------------------------------------------------------------------------
HRESULT CXSkyBox::CreateBox(float CubeSize)
{
  //Set Default Parameters
  HRESULT HRes;
  m_fCubeSize = CubeSize;

  ID3D11Texture2D* pCubeTexture = NULL;
  ID3D11ShaderResourceView* pCubeRV = NULL;
  UINT SupportCaps = 0;
  UINT NumVertLayout = ARRAYSIZE(g_aVertexLayout);

  d_pDevice->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT,&SupportCaps);
  if (SupportCaps & D3D11_FORMAT_SUPPORT_TEXTURECUBE &&
      SupportCaps & D3D11_FORMAT_SUPPORT_RENDER_TARGET &&
      SupportCaps & D3D11_FORMAT_SUPPORT_TEXTURE2D)
  {
    D3DX11_IMAGE_LOAD_INFO LoadInfo;
    LoadInfo.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    HRes = D3DX11CreateShaderResourceViewFromFile(d_pDevice,L"\Textures\\SkyMaps\\Maskonaive2.dds",&LoadInfo,NULL,&pCubeRV,NULL);
	if (FAILED(HRes))
    {
      Log("XSkyBox: Error Load Cube Texture");
      return HRes;
    }
    pCubeRV->GetResource((ID3D11Resource**)&pCubeTexture);
  }
  else
  {
    Log("XSkyBox: Sorry Cube Textures Do Not Supported Of Your Graphics Device");
    return E_FAIL;
  }  

  m_pEnvironmentMap = pCubeTexture;
  m_pEnvironmentRV = pCubeRV;

  ID3DBlob* pBlobVS = NULL;
  ID3DBlob* pBlobPS = NULL;

  //Create The Shaders
  HRes = CompileShaderFromFile(L"\Shaders\\SkyBox.hlsl","SkyboxVS","vs_4_0",&pBlobVS);
  if (FAILED(HRes))
  {
	Log("XSkyBox: Error Compile Vertex Shader From File");
    return HRes;
  }

  HRes = CompileShaderFromFile(L"\Shaders\\SkyBox.hlsl","SkyboxPS","ps_4_0",&pBlobPS);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Compile Pixel Shader From File");
    return HRes;
  }

  HRes = d_pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(),pBlobVS->GetBufferSize(),NULL,&m_pVertexShader);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Vertex Shader");
    return HRes; 
  }

  HRes = d_pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(),pBlobPS->GetBufferSize(),NULL,&m_pPixelShader);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Pixel Shader");
    return HRes; 
  }

  //Create An Input Layout
  HRes = d_pDevice->CreateInputLayout(g_aVertexLayout,NumVertLayout,pBlobVS->GetBufferPointer(),
  pBlobVS->GetBufferSize(),&m_pVertexLayout);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Input Layout");
    return HRes; 
  }

  ReleaseClass(pBlobVS);
  ReleaseClass(pBlobPS);

  //Query Support For Linear Filtering On DXGI_FORMAT_R32G32B32A32
  UINT FormatSupport = 0;
  HRes = d_pDevice->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT,&FormatSupport);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Check Format Support");
    return HRes; 
  }

  //Setup Linear Or Point Sampler According To The Format Query Result
  D3D11_SAMPLER_DESC SamDesc;
  SamDesc.Filter = (FormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE) > 0 ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
  SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SamDesc.MipLODBias = 0.0f;
  SamDesc.MaxAnisotropy = 1;
  SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
  SamDesc.MinLOD = 0;
  SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
  HRes = d_pDevice->CreateSamplerState(&SamDesc,&m_pSam);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Sampler State");
    return HRes; 
  }

  //Setup Constant Buffer
  D3D11_BUFFER_DESC BufDesc;
  BufDesc.Usage = D3D11_USAGE_DYNAMIC;
  BufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  BufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  BufDesc.MiscFlags = 0;
  BufDesc.ByteWidth = sizeof(CB_VS_PER_OBJECT);
  HRes = d_pDevice->CreateBuffer(&BufDesc,NULL,&m_pcbVSPerObject);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Buffer");
    return HRes; 
  }

  //Depth Stencil State
  D3D11_DEPTH_STENCIL_DESC DSDesc;
  ZeroMemory(&DSDesc,sizeof(D3D11_DEPTH_STENCIL_DESC));
  DSDesc.DepthEnable = FALSE;
  DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  DSDesc.DepthFunc = D3D11_COMPARISON_LESS;
  DSDesc.StencilEnable = FALSE;
  HRes = d_pDevice->CreateDepthStencilState(&DSDesc,&m_pDepthStencilState);
  if (FAILED(HRes))
  {
    Log("XSkyBox: Error Create Depth Stencil State");
    return HRes; 
  }

  return S_OK;
} //EndCreateBoxFunction

//------------------------------------------------------------------------------
// Name: CXSkyBox::UpdateBox()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyBox::UpdateBox(const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
  //Set Default Parameters
  HRESULT HRes;
  if (d_pDevice == NULL) return;

  //Fill The Vertex Buffer
  SKYBOX_VERTEX* pVertex = new SKYBOX_VERTEX[4];
  if (!pVertex) return;

  //Map Texels To Pixels 
  float fHighW = -1.0f - (1.0f/(float)pBackBufferSurfaceDesc->Width);
  float fHighH = -1.0f - (1.0f/(float)pBackBufferSurfaceDesc->Height);
  float fLowW = 1.0f + (1.0f/(float)pBackBufferSurfaceDesc->Width);
  float fLowH = 1.0f + (1.0f/(float)pBackBufferSurfaceDesc->Height);
    
  pVertex[0].Pos = D3DXVECTOR4(fLowW, fLowH, 1.0f, 1.0f);
  pVertex[1].Pos = D3DXVECTOR4(fLowW, fHighH, 1.0f, 1.0f);
  pVertex[2].Pos = D3DXVECTOR4(fHighW, fLowH, 1.0f, 1.0f);
  pVertex[3].Pos = D3DXVECTOR4(fHighW, fHighH, 1.0f, 1.0f);

  UINT uiVertBufSize = 4*sizeof(SKYBOX_VERTEX);
   
  //Vertex Buffer
  D3D11_BUFFER_DESC VBDesc;
  VBDesc.ByteWidth = uiVertBufSize;
  VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
  VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VBDesc.CPUAccessFlags = 0;
  VBDesc.MiscFlags = 0;    

  D3D11_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = pVertex;    
  HRes = d_pDevice->CreateBuffer(&VBDesc,&InitData,&m_pVB);
  if (FAILED(HRes))
  {
    Log("CXSkyBox::UpdateBox - Error Create Buffer");
	DeleteArray(pVertex);
    return; 
  }

  DeleteArray(pVertex);
} //EndUpdateBoxProcedure

//-----------------------------------------------------------------------------