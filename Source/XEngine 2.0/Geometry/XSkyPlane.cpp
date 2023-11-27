/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XSkyPlane.cpp Visual Studio 2012 Version.
// It's An XEngine SkyPlane Perturbed Clouds Implementation Source File.
// Date Creation: 21 April 2014
// Last Changing: 24 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XSkyPlane.h>

//////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXSkyPlane Class
//////////////////////////////////////////////////////////////////////////////
CXSkyPlane::CXSkyPlane(ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
{
  //Set Default Parameters
  d_pDevice = pDevice;            //To Appropriate Device Pointer To Class
  d_pImmediateContext = pContext; //To Appropriate Context Pointer To Class

  //Set SkyPlane Defaults
  m_SkyPlane = NULL;
  m_VertexBuffer = NULL;
  m_IndexBuffer = NULL;
  m_CloudTexture = NULL;
  m_PerturbTexture = NULL;

  //Set SkyPlane Shader Defaults
  m_VertexShader = NULL;
  m_PixelShader = NULL;
  m_Layout = NULL;
  m_SampleState = NULL;
  m_MatrixBuffer = NULL;
  m_SkyBuffer = NULL;

} //EndConstructionDirectives

CXSkyPlane::~CXSkyPlane()
{  
} //EndDestructionDirectives

//------------------------------------------------------------------------------
// Name: CXSkyPlane::Release()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyPlane::Release()
{
  //Release The Sky Constant Buffer
  ReleaseClass(m_SkyBuffer);

  //Release The Matrix Constant Buffer
  ReleaseClass(m_MatrixBuffer);
  
  //Release The Sampler States
  ReleaseClass(m_SampleState);

  //Release The Alpha Blend State
  ReleaseClass(m_AlphaBlendState);

  //Release The Layout
  ReleaseClass(m_Layout);

  //Release The Pixel Shader
  ReleaseClass(m_PixelShader);

  //Release The Vertex Shader
  ReleaseClass(m_VertexShader);

  //Release The Texture Objects
  ReleaseClass(m_PerturbTexture);
  ReleaseClass(m_CloudTexture);

  //Release The Index Buffer
  ReleaseClass(m_IndexBuffer);

  //Release The Vertex Buffer
  ReleaseClass(m_VertexBuffer);

  //Release The Sky Plane Array
  DeleteArray(m_SkyPlane);

  //NULL Immediate Context Pointer
  d_pImmediateContext = NULL;

  //NULL Device Pointer
  d_pDevice = NULL;               
} //EndReleaseProcedure

//------------------------------------------------------------------------------
// Name: CXSkyPlane::Render()
// Desc: 
//------------------------------------------------------------------------------
void CXSkyPlane::Render(D3DXMATRIX WorldMatrix,D3DXMATRIX ViewMatrix,D3DXMATRIX ProjMatrix)
{
  //Set Default Parameters
  HRESULT HRes;
  unsigned int Stride;
  unsigned int Offset;
  float BlendFactor[4];

  //Increment The Texture Translation Value Each Frame
  m_Translation+= 0.000045f;
  if (m_Translation > 1.0f) m_Translation-= 1.0f;

  //Setup The Blend Factor.
  BlendFactor[0] = 0.0f;
  BlendFactor[1] = 0.0f;
  BlendFactor[2] = 0.0f;
  BlendFactor[3] = 0.0f;
	
  //Turn On The Alpha Blending
  d_pImmediateContext->OMSetBlendState(m_AlphaBlendState,BlendFactor,0xffffffff);

  //Set Vertex Buffer Stride And Offset
  Stride = sizeof(VertexType); 
  Offset = 0;
    
  //Set The Vertex Buffer To Active In The Input Assembler So It Can Be Rendered
  d_pImmediateContext->IASetVertexBuffers(0,1,&m_VertexBuffer,&Stride,&Offset);

  //Set The Index Buffer To Active In The Input Assembler So It Can Be Rendered
  d_pImmediateContext->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32_UINT,0);

  //Set The Type Of Primitive That Should Be Rendered From This Vertex Buffer, In This Case Triangles
  d_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //Set SkyPlane Shader Parameters
  D3D11_MAPPED_SUBRESOURCE MappedResource;
  MatrixBufferType* DataPtr;
  SkyBufferType* DataPtr2;
  unsigned int BufferNumber;
  D3DXMATRIX PosMatrix;

  //Make Sure The SkyPlane Is Always Centered Around Camera
  D3DXMatrixTranslation(&PosMatrix,VecPos.x,VecPos.y,VecPos.z);

  //Transpose The Matrices To Prepare Them For The Shader
  D3DXMatrixTranspose(&WorldMatrix,&WorldMatrix);
  D3DXMatrixTranspose(&PosMatrix,&PosMatrix);
  D3DXMatrixTranspose(&ViewMatrix,&ViewMatrix);
  D3DXMatrixTranspose(&ProjMatrix,&ProjMatrix);

  //Lock The Constant Buffer So It Can Be Written To
  HRes = d_pImmediateContext->Map(m_MatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes)) return;
  
  //Get A Pointer To The Data In The Constant Buffer
  DataPtr = (MatrixBufferType*)MappedResource.pData;

  //Copy The Matrices Into The Constant Buffer
  DataPtr->World = WorldMatrix;
  DataPtr->Pos = PosMatrix;
  DataPtr->View = ViewMatrix;
  DataPtr->Projection = ProjMatrix;

  //Unlock The Constant Buffer
  d_pImmediateContext->Unmap(m_MatrixBuffer,0);

  //Set The Position Of The Constant Buffer In The Vertex Shader
  BufferNumber = 0;

  //Finally Set The Constant Buffer In The Vertex Shader With The Updated Values
  d_pImmediateContext->VSSetConstantBuffers(BufferNumber,1,&m_MatrixBuffer);
	
  //Lock The Sky Constant Buffer So It Can Be Written To
  HRes = d_pImmediateContext->Map(m_SkyBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes)) return;

  //Get A Pointer To The Data In The Sky Constant Buffer
  DataPtr2 = (SkyBufferType*)MappedResource.pData;

  //Copy The Data Into The Sky Constant Buffer
  DataPtr2->Translation = m_Translation;
  DataPtr2->Scale = m_Scale;
  DataPtr2->Brightness = m_Brightness;
  DataPtr2->Padding = 0.0f;

  //Unlock The Sky Constant Buffer
  d_pImmediateContext->Unmap(m_SkyBuffer,0);

  //Set The Position Of The Sky Constant Buffer In The Pixel Shader
  BufferNumber = 0;

  //Now Set The Sky Constant Buffer In The Pixel Shader With The Updated Values
  d_pImmediateContext->PSSetConstantBuffers(BufferNumber,1,&m_SkyBuffer);
	
  //Set The Shader Texture Resources In The Pixel Shader
  d_pImmediateContext->PSSetShaderResources(0,1,&m_CloudTexture);
  d_pImmediateContext->PSSetShaderResources(1,1,&m_PerturbTexture);

  //Set The Vertex Input Layout
  d_pImmediateContext->IASetInputLayout(m_Layout);

  //Set The Vertex And Pixel Shaders That Will Be Used To Render The Triangles
  d_pImmediateContext->VSSetShader(m_VertexShader,NULL,0);
  d_pImmediateContext->PSSetShader(m_PixelShader,NULL,0);

  //Set The Sampler State In The Pixel Shader
  d_pImmediateContext->PSSetSamplers(0,1,&m_SampleState);

  //Render The Triangles
  d_pImmediateContext->DrawIndexed(m_IndexCount,0,0);
} //EndRenderProcedure

//------------------------------------------------------------------------------
// Name: CXSkyPlane::Create()
// Desc: 
//------------------------------------------------------------------------------
HRESULT CXSkyPlane::Create(WCHAR* CloudTextureFilename,WCHAR* PerturbTextureFilename)
{
  //Set Default Parameters
  HRESULT HRes;
  int SkyPlaneResolution,TextureRepeat;
  float SkyPlaneWidth,SkyPlaneTop,SkyPlaneBottom;
  
  //Set The Sky Plane Parameters
  SkyPlaneResolution = 50; //50
  SkyPlaneWidth = 850.0f;   //10.0f
  SkyPlaneTop = 75.5f;      //0.5f
  SkyPlaneBottom = -15.0f;   //0.0f
  TextureRepeat = 2;

  //Set The Sky Plane Shader Related Parameters
  m_Scale = 0.9f;       //0.3f
  m_Brightness = 0.3f;  //0.5f

  //Initialize The Translation To Zero
  m_Translation = 0.0f;

  //Create The Sky Plane
  HRes = CreateSkyPlane(SkyPlaneResolution,SkyPlaneWidth,SkyPlaneTop,SkyPlaneBottom,TextureRepeat);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::Create - Error Create Sky Plane");
	return HRes;
  }

  //Load Cloud Texture 
  HRes = D3DX11CreateShaderResourceViewFromFile(d_pDevice,CloudTextureFilename,NULL,NULL,&m_CloudTexture,NULL);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::Create - Error Load Cloud Texture");
	return HRes;
  }

  //Load Perturb Texture 
  HRes = D3DX11CreateShaderResourceViewFromFile(d_pDevice,PerturbTextureFilename,NULL,NULL,&m_PerturbTexture,NULL);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::Create - Error Load Perturb Texture");
	return HRes;
  }

  //Create SkyPlane Shader
  HRes = CreateSkyPlaneShader();
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::Create - Error Create SkyPlane Shader");
	return HRes;
  }

  //Create Alpha Blend State Description
  D3D11_BLEND_DESC BlendStateDescription;
  ZeroMemory(&BlendStateDescription,sizeof(D3D11_BLEND_DESC));
  BlendStateDescription.RenderTarget[0].BlendEnable = TRUE;
  BlendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
  BlendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
  BlendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  BlendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  BlendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  BlendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

  //Create The Blend State Using The Description
  HRes = d_pDevice->CreateBlendState(&BlendStateDescription,&m_AlphaBlendState);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::Create - Error Create Blend State");
	return HRes;
  }

  return S_OK;
} //EndCreateFunction

//------------------------------------------------------------------------------
// Name: CXSkyPlane::CreateSkyPlane()
// Desc: 
//------------------------------------------------------------------------------
HRESULT CXSkyPlane::CreateSkyPlane(int SkyPlaneResolution,
float SkyPlaneWidth,float SkyPlaneTop,float SkyPlaneBottom,int TextureRepeat)
{
  HRESULT HRes;
  float QuadSize,Radius,Constant,TextureDelta;
  int Index,Index1,Index2,Index3,Index4;
  float PositionX,PositionY,PositionZ,tu,tv;

  VertexType* Vertices;
  unsigned long* Indices;
  D3D11_BUFFER_DESC VertexBufferDesc,IndexBufferDesc;
  D3D11_SUBRESOURCE_DATA VertexData,IndexData;
  
  //Create The Array To Hold The Sky Plane Coordinates
  m_SkyPlane = new SkyPlaneType[(SkyPlaneResolution+1)*(SkyPlaneResolution+1)];
  if (!m_SkyPlane)
  {
    Log("CXSkyPlane::CreateSkyPlane - Error Create Sky Plane Type");
	return E_FAIL;
  }

  //Determine The Size Of Each Quad On The Sky Plane
  QuadSize = SkyPlaneWidth/(float)SkyPlaneResolution;

  //Calculate The Radius Of The Sky Plane Based On The Width
  Radius = SkyPlaneWidth/2.0f;

  //Calculate The Height Constant To Increment By
  Constant = (SkyPlaneTop-SkyPlaneBottom)/(Radius*Radius);

  //Calculate The Texture Coordinate Increment Value
  TextureDelta = (float)TextureRepeat/(float)SkyPlaneResolution;

  //Loop Through The Sky Plane And Build The Coordinates Based On The Increment Values Given
  for (int J = 0; J <= SkyPlaneResolution; J++)
  {
	for (int I = 0; I <= SkyPlaneResolution; I++)
    {
	  //Calculate The Vertex Coordinates.
	  PositionX = (-0.5f*SkyPlaneWidth)+((float)I*QuadSize);
	  PositionZ = (-0.5f*SkyPlaneWidth)+((float)J*QuadSize);
	  PositionY = SkyPlaneTop-(Constant*((PositionX*PositionX)+(PositionZ*PositionZ)));

      //Calculate The Texture Coordinates
	  tu = (float)I*TextureDelta;
	  tv = (float)J*TextureDelta;

	  //Calculate The Index Into The Sky Plane Array To Add This Coordinate
	  Index = J*(SkyPlaneResolution+1)+I;

	  //Add The Coordinates To The Sky Plane Array
	  m_SkyPlane[Index].x = PositionX;
	  m_SkyPlane[Index].y = PositionY;
	  m_SkyPlane[Index].z = PositionZ;
	  m_SkyPlane[Index].tu = tu;
	  m_SkyPlane[Index].tv = tv;
    }
  }
 
  //Calculate The Number Of Vertices In The Sky Plane Mesh
  m_VertexCount = (SkyPlaneResolution+1)*(SkyPlaneResolution+1)*6;

  //Set The Index Count To The Same As The Vertex Count
  m_IndexCount = m_VertexCount;
		
  //Create The Vertex Array
  Vertices = new VertexType[m_VertexCount];
  if (!Vertices)
  {
	Log("CXSkyPlane::CreateSkyPlane - Error Create Sky Plane Vertex Type");
	return E_FAIL;
  }

  //Create The Index Array
  Indices = new unsigned long[m_IndexCount];
  if (!Indices)
  {
    Log("CXSkyPlane::CreateSkyPlane - Error Create Sky Plane Index Type");
	return E_FAIL;
  }

  //Initialize The Index Into The Vertex Array
  Index = 0;

  //Load The Vertex And Index Array With The Sky Plane Array Data
  for (int J = 0; J < SkyPlaneResolution; J++)
  {
	for (int I = 0; I < SkyPlaneResolution; I++)
	{
	  Index1 = J*(SkyPlaneResolution+1)+I;
	  Index2 = J*(SkyPlaneResolution+1)+(I+1);
	  Index3 = (J+1)*(SkyPlaneResolution+1)+I;
	  Index4 = (J+1)*(SkyPlaneResolution+1)+(I+1);

	  //Triangle 1 - Upper Left
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index1].x,m_SkyPlane[Index1].y,m_SkyPlane[Index1].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index1].tu,m_SkyPlane[Index1].tv);
	  Indices[Index] = Index;
	  Index++;

	  //Triangle 1 - Upper Right
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index2].x,m_SkyPlane[Index2].y,m_SkyPlane[Index2].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index2].tu,m_SkyPlane[Index2].tv);
	  Indices[Index] = Index;
	  Index++;

	  //Triangle 1 - Bottom Left
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index3].x,m_SkyPlane[Index3].y,m_SkyPlane[Index3].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index3].tu,m_SkyPlane[Index3].tv);
	  Indices[Index] = Index;
	  Index++;

	  //Triangle 2 - Bottom Left
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index3].x,m_SkyPlane[Index3].y,m_SkyPlane[Index3].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index3].tu,m_SkyPlane[Index3].tv);
	  Indices[Index] = Index;
	  Index++;

	  //Triangle 2 - Upper Right
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index2].x,m_SkyPlane[Index2].y,m_SkyPlane[Index2].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index2].tu,m_SkyPlane[Index2].tv);
	  Indices[Index] = Index;
	  Index++;

	  //Triangle 2 - Bottom Right
	  Vertices[Index].Pos = D3DXVECTOR3(m_SkyPlane[Index4].x,m_SkyPlane[Index4].y,m_SkyPlane[Index4].z);
	  Vertices[Index].Texture = D3DXVECTOR2(m_SkyPlane[Index4].tu,m_SkyPlane[Index4].tv);
	  Indices[Index] = Index;
	  Index++;
	}
  }

  //Set Up The Description Of The Vertex Buffer
  VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  VertexBufferDesc.ByteWidth = sizeof(VertexType)*m_VertexCount;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = 0;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;

  //Give The Subresource Structure A Pointer To The Vertex Data
  VertexData.pSysMem = Vertices;
  VertexData.SysMemPitch = 0;
  VertexData.SysMemSlicePitch = 0;

  //Now Finally Create The Vertex Buffer
  HRes = d_pDevice->CreateBuffer(&VertexBufferDesc,&VertexData,&m_VertexBuffer);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::CreateSkyPlane - Error Create Sky Plane Vertex Buffer");
	return HRes;
  }

  //Set Up The Description Of The Index Buffer
  IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  IndexBufferDesc.ByteWidth = sizeof(unsigned long)*m_IndexCount;
  IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  IndexBufferDesc.CPUAccessFlags = 0;
  IndexBufferDesc.MiscFlags = 0;
  IndexBufferDesc.StructureByteStride = 0;

  //Give The Subresource Structure A Pointer To The Index Data
  IndexData.pSysMem = Indices;
  IndexData.SysMemPitch = 0;
  IndexData.SysMemSlicePitch = 0;

  //Create The Index Buffer
  HRes = d_pDevice->CreateBuffer(&IndexBufferDesc,&IndexData,&m_IndexBuffer);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::CreateSkyPlane - Error Create Sky Plane Index Buffer");
	return HRes;
  }

  //Release The Arrays Now That The Vertex And Index Buffers Have Been Created And Loaded
  DeleteArray(Vertices);
  DeleteArray(Indices);

  return S_OK;
} //EndCreateSkyPlaneFunction

//------------------------------------------------------------------------------
// Name: CXSkyPlane::CreateSkyPlaneShader()
// Desc: 
//------------------------------------------------------------------------------
HRESULT CXSkyPlane::CreateSkyPlaneShader()
{
  HRESULT HRes;
  D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
  unsigned int NumElements;
 
  //Create And Compile The Shaders
  ID3DBlob* pBlobVS = NULL;
  ID3DBlob* pBlobPS = NULL;

  HRes = CompileShaderFromFile(L"\Effects\\SkyPlane.fx","SkyPlaneVertexShader","vs_4_0",&pBlobVS);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::CreateSkyPlaneShader - Error Compile Vertex Shader From File");
    return HRes;
  }

  HRes = CompileShaderFromFile(L"\Effects\\SkyPlane.fx","SkyPlanePixelShader","ps_4_0",&pBlobPS);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::CreateSkyPlaneShader - Error Compile Pixel Shader From File");
    return HRes;
  } 
  
  //Create The Vertex Shader From The Buffer
  HRes = d_pDevice->CreateVertexShader(pBlobVS->GetBufferPointer(),pBlobVS->GetBufferSize(),NULL,&m_VertexShader);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Vertex Shader Buffer");
	return HRes;
  }

  //Create The Pixel Shader From The Buffer
  HRes = d_pDevice->CreatePixelShader(pBlobPS->GetBufferPointer(),pBlobPS->GetBufferSize(),NULL,&m_PixelShader);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Pixel Shader Buffer");
	return HRes;
  }

  //Create The Vertex Input Layout Description
  PolygonLayout[0].SemanticName = "POSITION";
  PolygonLayout[0].SemanticIndex = 0;
  PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  PolygonLayout[0].InputSlot = 0;
  PolygonLayout[0].AlignedByteOffset = 0;
  PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayout[0].InstanceDataStepRate = 0;

  PolygonLayout[1].SemanticName = "TEXCOORD";
  PolygonLayout[1].SemanticIndex = 0;
  PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
  PolygonLayout[1].InputSlot = 0;
  PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  PolygonLayout[1].InstanceDataStepRate = 0;

  //Get A Count Of The Elements In The Layout
  NumElements = sizeof(PolygonLayout)/sizeof(PolygonLayout[0]);

  //Create The Vertex Input Layout
  HRes = d_pDevice->CreateInputLayout(PolygonLayout,NumElements,
  pBlobVS->GetBufferPointer(),pBlobVS->GetBufferSize(),&m_Layout);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Shader Input Layout");
	return HRes;
  }

  //Release The Vertex Shader Buffer And Pixel Shader Buffer Since They Are No Longer Needed
  ReleaseClass(pBlobVS);
  ReleaseClass(pBlobPS);

  //Create A Texture Sampler State Description
  D3D11_SAMPLER_DESC SamplerDesc;
  SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  SamplerDesc.MipLODBias = 0.0f;
  SamplerDesc.MaxAnisotropy = 1;
  SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  SamplerDesc.BorderColor[0] = 0;
  SamplerDesc.BorderColor[1] = 0;
  SamplerDesc.BorderColor[2] = 0;
  SamplerDesc.BorderColor[3] = 0;
  SamplerDesc.MinLOD = 0;
  SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  //Create The Texture Sampler State
  HRes = d_pDevice->CreateSamplerState(&SamplerDesc,&m_SampleState);
  if (FAILED(HRes))
  {
	Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Sampler State");
	return HRes;
  }

  //Setup The Description Of The Dynamic Matrix Constant Buffer That Is In The Vertex Shader
  D3D11_BUFFER_DESC MatrixBufferDesc;
  MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  MatrixBufferDesc.MiscFlags = 0;
  MatrixBufferDesc.StructureByteStride = 0;

  //Create The Constant Buffer Pointer So We Can Access The Vertex Shader 
  //Constant Buffer From Within This Class
  HRes = d_pDevice->CreateBuffer(&MatrixBufferDesc,NULL,&m_MatrixBuffer);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Constant Vertex Buffer Pointer");
	return HRes;
  }

  //Setup The Description Of The Dynamic Sky Constant Buffer That Is In The Pixel Shader
  D3D11_BUFFER_DESC SkyBufferDesc;
  SkyBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  SkyBufferDesc.ByteWidth = sizeof(SkyBufferType);
  SkyBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  SkyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  SkyBufferDesc.MiscFlags = 0;
  SkyBufferDesc.StructureByteStride = 0;

  //Create The Sky Buffer Pointer So We Can Access The Pixel Shader Constant Buffer From Within This Class
  HRes = d_pDevice->CreateBuffer(&SkyBufferDesc,NULL,&m_SkyBuffer);
  if (FAILED(HRes))
  {
    Log("CXSkyPlane::CreateSkyPlaneShader - Error Create Constant Pixel Buffer Pointer");
	return HRes;
  }

  return S_OK;
} //EndCreateSkyPlaneShaderFunction

//-----------------------------------------------------------------------------