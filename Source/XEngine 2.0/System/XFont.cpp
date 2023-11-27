/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XFont.cpp Visual Studio 2012 Version.
// This XEngine Font System Source File.
// Date Creation: 21 April 2014
// Last Changing: 08 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XFont.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXFont Class
///////////////////////////////////////////////////////////////////////////////
CXFont::CXFont(ID3D11Device* Device,ID3D11DeviceContext* DevContext)
{
  //Set Default Parameters For Font
  d_pDevice = Device;         //Set D3D Device Pointer To Class 
  d_pDevContext = DevContext; //Set D3D Device Context Pointer To Class
  m_Font = NULL;
  m_Texture = NULL;

  //Set Default Parameters For Font Shader
  m_VertexShader = NULL;
  m_PixelShader = NULL;
  m_Layout = NULL;
  m_ConstantBuffer = NULL;
  m_SampleState = NULL;
  m_PixelBuffer = NULL;

} //EndConstructionDirectives

CXFont::~CXFont()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXFont::Initialize()
// Desc: 
//----------------------------------------------------------------------------
HRESULT CXFont::Initialize(char* FontFilename,WCHAR* TextureFilename)
{
  HRESULT HRes;
  
  //Font Shader Variables
  ID3D10Blob* VertexShaderBuffer;
  ID3D10Blob* PixelShaderBuffer;
  D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
  unsigned int NumElements;
  D3D11_BUFFER_DESC ConstantBufferDesc;
  D3D11_SAMPLER_DESC SamplerDesc;
  D3D11_BUFFER_DESC PixelBufferDesc;

  //Load The Texture In
  HRes = D3DX11CreateShaderResourceViewFromFile(d_pDevice,TextureFilename,NULL,NULL,&m_Texture,NULL);
  if (FAILED(HRes))
  {
	Log("CXFont::Initialize - Error Create Font Texture");
	return HRes;
  }

  //Load In The Text File Containing The Font Data
  ifstream FIn;
  char Temp;

  //Create The Font Spacing Buffer.
  m_Font = new FontType[95];
  if (!m_Font)
  {
	Log("CXFont::Initialize - Cannot Initialize Font Type");
	return E_FAIL;
  }

  //Read In The Font Size And Spacing Between Chars
  FIn.open(FontFilename);
  if (FIn.fail())
  {
	Log("CXFont::Initialize - Cannot Open Font Data File");
	return E_FAIL;
  }

  //Read In The 95 Used ASCII Characters For Text
  for (int I = 0; I < 95; I++)
  {
	FIn.get(Temp);
	while (Temp != ' ')
	{
	  FIn.get(Temp);
	}

	FIn.get(Temp);
	while (Temp != ' ')
	{
	  FIn.get(Temp);
	}

	FIn >> m_Font[I].Left;
	FIn >> m_Font[I].Right;
	FIn >> m_Font[I].Size;
  }

  //Close The File
  FIn.close();

  //Font Shader Initialization Part
  //Initialize The Pointers This Function Will Use To Null
  VertexShaderBuffer = NULL;
  PixelShaderBuffer = NULL;

  //Compile The Vertex Shader Code
  HRes = CompileShaderFromFile(L"\Effects\\Font.fx","FontVertexShader","vs_4_0",&VertexShaderBuffer);
  if (FAILED(HRes))
  {
	Log("CXFont::Initialize - Error Compile Font Vertex Shader From File");
    return HRes;
  }

  //Compile The Pixel Shader Code
  HRes = CompileShaderFromFile(L"\Effects\\Font.fx","FontPixelShader","ps_4_0",&PixelShaderBuffer);
  if (FAILED(HRes))
  {
	Log("CXFont::Initialize - Error Compile Font Pixel Shader From File");
    return HRes;
  }

  //Create The Vertex Shader From The Buffer
  HRes = d_pDevice->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(),
  VertexShaderBuffer->GetBufferSize(),NULL,&m_VertexShader);
  if (FAILED(HRes))
  {
    Log("CXFont::Initialize - Error Create Font Vertex Shader");
    return HRes; 
  }

  //Create The Vertex Shader From The Buffer
  HRes = d_pDevice->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(),
  PixelShaderBuffer->GetBufferSize(),NULL,&m_PixelShader);
  if (FAILED(HRes))
  {
    Log("CXFont::Initialize - Error Create Font Pixel Shader");
    return HRes; 
  }

  //Create The Vertex Input Layout Description
  //This Setup Needs To Match The VertexType Stucture In The ModelClass And In The Shader
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
  HRes = d_pDevice->CreateInputLayout(PolygonLayout,NumElements,VertexShaderBuffer->GetBufferPointer(), 
  VertexShaderBuffer->GetBufferSize(),&m_Layout);
  if (FAILED(HRes))
  {
    Log("CXFont::Initialize - Error Create Font Input Layout");
    return HRes; 
  }

  //Release The Vertex Shader Buffer And Pixel Shader Buffer Since They Are No Longer Needed
  VertexShaderBuffer->Release();
  VertexShaderBuffer = NULL;
  PixelShaderBuffer->Release();
  PixelShaderBuffer = NULL;

  //Setup The Description Of The Dynamic Constant Buffer That Is In The Vertex Shader
  ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  ConstantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
  ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  ConstantBufferDesc.MiscFlags = 0;
  ConstantBufferDesc.StructureByteStride = 0;

  //Create The Constant Buffer Pointer So We Can Access The Vertex Shader 
  //Constant Buffer From Within This Class
  HRes = d_pDevice->CreateBuffer(&ConstantBufferDesc,NULL,&m_ConstantBuffer);
  if (FAILED(HRes))
  {
    Log("CXFont::Initialize - Error Create Font Constant Buffer");
    return HRes; 
  }

  //Create A Texture Sampler State Description.
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

  //Create The Texture Sampler State.
  HRes = d_pDevice->CreateSamplerState(&SamplerDesc,&m_SampleState);
  if (FAILED(HRes))
  {
	Log("CXFont::Initialize - Error Create Font Sampler State");
    return HRes;
  }

  //Setup The Description Of The Dynamic Pixel Constant Buffer That Is In The Pixel Shader
  PixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  PixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
  PixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  PixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  PixelBufferDesc.MiscFlags = 0;
  PixelBufferDesc.StructureByteStride = 0;

  //Create The Pixel Constant Buffer Pointer So We Can Access The Pixel Shader Constant 
  //Buffer From Within This Class
  HRes = d_pDevice->CreateBuffer(&PixelBufferDesc,NULL,&m_PixelBuffer);
  if (FAILED(HRes))
  {
    Log("CXFont::Initialize - Error Create Font Constant Pixel Buffer");
	return HRes;
  }

  return S_OK;
} //EndInitializeFunction

//-----------------------------------------------------------------------------
// Name: CXFont::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CXFont::Release()
{
  //Release The Font Data Array
  DeleteArray(m_Font);

  //Release The Texture Object
  ReleaseClass(m_Texture);

  //Release The Pixel Constant Buffer
  ReleaseClass(m_PixelBuffer);
  
  //Release The Sampler State
  ReleaseClass(m_SampleState);

  //Release The Constant Buffer
  ReleaseClass(m_ConstantBuffer);

  //Release The Layout
  ReleaseClass(m_Layout);
  
  //Release The Pixel Shader
  ReleaseClass(m_PixelShader);
  
  //Release The Vertex Shader
  ReleaseClass(m_VertexShader);	

  //NULL D3D Device Context Pointer
  d_pDevContext = NULL; 

  //NULL D3D Device Pointer
  d_pDevice = NULL;  
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CXFont::GetTexture()
// Desc:
//-----------------------------------------------------------------------------
ID3D11ShaderResourceView* CXFont::GetTexture()
{
  return m_Texture;
} //EndGetTextureFunction

//-----------------------------------------------------------------------------
// Name: CXFont::BuildVertexArray()
// Desc:
//----------------------------------------------------------------------------
void CXFont::BuildVertexArray(void* Vertices,char* Sentence,float DrawX,float DrawY)
{
  VertexType* VertexPtr;
  int NumLetters,Index,Letter;

  //Coerce The Input Vertices Into A VertexType Structure
  VertexPtr = (VertexType*)Vertices;

  //Get The Number Of Letters In The Sentence
  NumLetters = (int)strlen(Sentence);

  //Initialize The Index To The Vertex Array
  Index = 0;

  //Draw Each Letter Onto A Quad
  for (int I = 0; I < NumLetters; I++)
  {
    Letter = ((int)Sentence[I])-32;

	//If The Letter Is A Space Then Just Move Over Three Pixels
	if (Letter == 0)
	{
	  DrawX = DrawX+3.0f;
	}
	else
	{
	  //First Triangle In Quad
	  VertexPtr[Index].Position = D3DXVECTOR3(DrawX,DrawY,0.0f); //Top Left
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Left,0.0f);
	  Index++;

	  VertexPtr[Index].Position = D3DXVECTOR3((DrawX+m_Font[Letter].Size),(DrawY-16),0.0f); //Bottom Right
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Right,1.0f);
	  Index++;

	  VertexPtr[Index].Position = D3DXVECTOR3(DrawX,(DrawY-16),0.0f); //Bottom Left
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Left,1.0f);
	  Index++;

	  //Second Triangle In Quad
	  VertexPtr[Index].Position = D3DXVECTOR3(DrawX,DrawY,0.0f); //Top Left
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Left,0.0f);
	  Index++;

	  VertexPtr[Index].Position = D3DXVECTOR3(DrawX+m_Font[Letter].Size,DrawY,0.0f); //Top Right
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Right,0.0f);
	  Index++;

	  VertexPtr[Index].Position = D3DXVECTOR3((DrawX+m_Font[Letter].Size),(DrawY-16),0.0f); //Bottom Right
	  VertexPtr[Index].Texture = D3DXVECTOR2(m_Font[Letter].Right,1.0f);
	  Index++;

	  //Update The x Location For Drawing By The Size Of The Letter And One Pixel
	  DrawX = DrawX+m_Font[Letter].Size+1.0f;
	}
  }
  
  return;
} //EndBuildVertexArrayProcedure

//-----------------------------------------------------------------------------
// Name: CXFont::Render()
// Desc:
//-----------------------------------------------------------------------------
void CXFont::Render(int IndexCount,D3DXVECTOR4 PixelColor)
{
  HRESULT HRes;
  D3D11_MAPPED_SUBRESOURCE MappedResource;
  ConstantBufferType* DataPtr;
  unsigned int BufferNumber;
  PixelBufferType* DataPtr2;

  //Lock The Constant Buffer So It Can Be Written To
  HRes = d_pDevContext->Map(m_ConstantBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes))
  {
    Log("CXFont::Render - Error Map Shader Font Constant Buffer");
    return;
  }

  //Get A Pointer To The Data In The Constant Buffer
  DataPtr = (ConstantBufferType*)MappedResource.pData;

  //Transpose The Matrices To Prepare Them For The Shader
  D3DXMatrixTranspose(&m_WorldMatrix,&m_WorldMatrix);
  D3DXMatrixTranspose(&m_ViewMatrix,&m_ViewMatrix);
  D3DXMatrixTranspose(&m_ProjectionMatrix,&m_ProjectionMatrix);

  //Copy The Matrices Into The Constant Buffer
  DataPtr->World = m_WorldMatrix;
  DataPtr->View = m_ViewMatrix;
  DataPtr->Projection = m_ProjectionMatrix;

  //Unlock The Constant Buffer
  d_pDevContext->Unmap(m_ConstantBuffer,0);

  //Set The Position Of The Constant Buffer In The Vertex Shader
  BufferNumber = 0;

  //Now Set The Constant Buffer In The Vertex Shader With The Updated Values
  d_pDevContext->VSSetConstantBuffers(BufferNumber,1,&m_ConstantBuffer);

  //Set Shader Texture Resource In The Pixel Shader
  d_pDevContext->PSSetShaderResources(0,1,&m_Texture);

  //Lock The Pixel Constant Buffer So It Can Be Written To
  HRes = d_pDevContext->Map(m_PixelBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes))
  {
    Log("CXFont::Render - Error Map Shader Font Pixel Buffer");
	return;
  }

  //Get A Pointer To The Data In The Pixel Constant Buffer
  DataPtr2 = (PixelBufferType*)MappedResource.pData;

  //Copy The Pixel Color Into The Pixel Constant Buffer
  DataPtr2->PixelColor = PixelColor;

  //Unlock The Pixel Constant Buffer
  d_pDevContext->Unmap(m_PixelBuffer,0);

  //Set The Position Of The Pixel Constant Buffer In The Pixel Shader
  BufferNumber = 0;

  //Now Set The Pixel Constant Buffer In The Pixel Shader With The Updated Value
  d_pDevContext->PSSetConstantBuffers(BufferNumber,1,&m_PixelBuffer);

  //Set The Vertex Input Layout
  d_pDevContext->IASetInputLayout(m_Layout);

  //Set The Vertex And Pixel Shaders That Will Be Used To Render The Triangles
  d_pDevContext->VSSetShader(m_VertexShader,NULL,0);
  d_pDevContext->PSSetShader(m_PixelShader,NULL,0);

  //Set The Sampler State In The Pixel Shader
  d_pDevContext->PSSetSamplers(0,1,&m_SampleState);

  //Render The Triangles
  d_pDevContext->DrawIndexed(IndexCount,0,0);
} //EndRenderProcedure

//----------------------------------------------------------------------------