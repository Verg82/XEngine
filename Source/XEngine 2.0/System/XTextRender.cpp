/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XTextRender.cpp Visual Studio 2012 Version.
// This XEngine Text Render System Source File.
// Date Creation: 21 April 2014
// Last Changing: 09 June 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XTextRender.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXText Class
///////////////////////////////////////////////////////////////////////////////
CXText::CXText(ID3D11Device* Device,ID3D11DeviceContext* DevContext)
{
  HRESULT HRes;

  //Set Default Parameters
  d_pDevice = Device;         //Set D3D Device Pointer To Class 
  d_pDevContext = DevContext; //Set D3D Device Context Pointer To Class
  m_Font = NULL;              //NULL Font Object By Default

  //Create The Font Object.
  m_Font = new CXFont(d_pDevice,d_pDevContext);
  if (!m_Font) 
  {	  
	Log("CXText::Initialize - Error Initialize Font Class"); 
	m_Font = NULL;
	return;
  }

  //Initialize The Font Object
  HRes = m_Font->Initialize("\Textures\\Fonts\\FontData.dat",L"\Textures\\Fonts\\Font.dds");
  if (FAILED(HRes))
  {
	Log("CXText::Initialize - Error Initialize Font Object");
	m_Font = NULL;
	return;
  }

  //Initialize The Sentence
  HRes = InitializeSentence(&m_Sentence,16);
  if (FAILED(HRes))
  {
	Log("CXText::Initialize - Error Initialize Text Object Sentence");
	return;
  }
} //EndConstructionDirectives

CXText::~CXText()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXText::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CXText::Release()
{
  //Release Sentence
  ReleaseSentence(&m_Sentence);

  //Release The Font Object
  if (m_Font)
  {
	m_Font->Release();
	delete m_Font;
	m_Font = NULL;
  }

  d_pDevContext = NULL; //NULL D3D Device Context Pointer
  d_pDevice = NULL;     //NULL D3D Device Pointer
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CXText::ReleaseSentence()
// Desc: 
//-----------------------------------------------------------------------------
void CXText::ReleaseSentence(SentenceType** Sentence)
{
  if (*Sentence)
  {
    //Release The Sentence Vertex Buffer
	if ((*Sentence)->VertexBuffer)
	{
	  (*Sentence)->VertexBuffer->Release();
	  (*Sentence)->VertexBuffer = NULL;
	}

	//Release The Sentence Index Buffer
	if ((*Sentence)->IndexBuffer)
	{
	  (*Sentence)->IndexBuffer->Release();
	  (*Sentence)->IndexBuffer = NULL;
	}

	//Release The Sentence
	DeleteClass(*Sentence);
  }
  return;
} //EndReleaseSentenceProcedure

//-----------------------------------------------------------------------------
// Name: CXText::Initialize()
// Desc: 
//-----------------------------------------------------------------------------
void CXText::Initialize(int ScreenWidth,int ScreenHeight,D3DXMATRIX BaseViewMatrix)
{
  //Store The Screen Width And Height
  m_ScreenWidth = ScreenWidth;
  m_ScreenHeight = ScreenHeight;

  //Store The Base View Matrix
  m_BaseViewMatrix = BaseViewMatrix;
} //EndInitializeProcedure

//-----------------------------------------------------------------------------
// Name: CXText::InitializeSentence()
// Desc: 
//-----------------------------------------------------------------------------
bool CXText::InitializeSentence(SentenceType** Sentence,int MaxLength)
{
  VertexType* Vertices;
  unsigned long* Indices;
  D3D11_BUFFER_DESC VertexBufferDesc,IndexBufferDesc;
  D3D11_SUBRESOURCE_DATA VertexData,IndexData;
  HRESULT HRes;

  //Create A New Sentence Object
  *Sentence = new SentenceType;
  if (!*Sentence)
  {
	return false;
  }

  //Initialize The Sentence Buffers To Null
  (*Sentence)->VertexBuffer = NULL;
  (*Sentence)->IndexBuffer = NULL;

  //Set The Maximum Length Of The Sentence
  (*Sentence)->MaxLength = MaxLength;

  //Set The Number Of Vertices In The Vertex Array
  (*Sentence)->VertexCount = 6*MaxLength;

  //Set The Number Of Indexes In The Index Array
  (*Sentence)->IndexCount = (*Sentence)->VertexCount;

  //Create The Vertex Array
  Vertices = new VertexType[(*Sentence)->VertexCount];
  if (!Vertices)
  {
	return false;
  }

  //Create The Index Array
  Indices = new unsigned long[(*Sentence)->IndexCount];
  if (!Indices)
  {
	return false;
  }

  //Initialize Vertex Array To Zeros At First
  memset(Vertices,0,(sizeof(VertexType)*(*Sentence)->VertexCount));

  //Initialize The Index Array
  for (int I = 0; I < (*Sentence)->IndexCount; I++)
  {
	Indices[I] = I;
  }

  //Set Up The Description Of The Dynamic Vertex Buffer
  VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  VertexBufferDesc.ByteWidth = sizeof(VertexType)*(*Sentence)->VertexCount;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;

  //Give The Subresource Structure A Pointer To The Vertex Data
  VertexData.pSysMem = Vertices;
  VertexData.SysMemPitch = 0;
  VertexData.SysMemSlicePitch = 0;

  //Create The Vertex Buffer
  HRes = d_pDevice->CreateBuffer(&VertexBufferDesc,&VertexData,&(*Sentence)->VertexBuffer);
  if (FAILED(HRes))
  {
	return false;
  }

  //Set Up The Description Of The Static Index Buffer
  IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  IndexBufferDesc.ByteWidth = sizeof(unsigned long)*(*Sentence)->IndexCount;
  IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  IndexBufferDesc.CPUAccessFlags = 0;
  IndexBufferDesc.MiscFlags = 0;
  IndexBufferDesc.StructureByteStride = 0;

  //Give The Subresource Structure A Pointer To The Index Data
  IndexData.pSysMem = Indices;
  IndexData.SysMemPitch = 0;
  IndexData.SysMemSlicePitch = 0;

  //Create The Index Buffer
  HRes= d_pDevice->CreateBuffer(&IndexBufferDesc,&IndexData,&(*Sentence)->IndexBuffer);
  if (FAILED(HRes))
  {
	return false;
  }

  //Release The Vertex Array As It Is No Longer Needed
  DeleteArray(Vertices);

  //Release The Index Array As It Is No Longer Needed
  DeleteArray(Indices);

  return true;
} //EndInitializeSentenceFunction

//-----------------------------------------------------------------------------
// Name: CXText::UpdateSentence()
// Desc: 
//-----------------------------------------------------------------------------
bool CXText::UpdateSentence(SentenceType* Sentence,char* Text,
int PositionX,int PositionY,float Red,float Green,float Blue)
{
  int NumLetters;
  VertexType* Vertices;
  float DrawX,DrawY;
  HRESULT HRes;
  D3D11_MAPPED_SUBRESOURCE MappedResource;
  VertexType* VerticesPtr;

  //Store The Color Of The Sentence.
  Sentence->Red = Red;
  Sentence->Green = Green;
  Sentence->Blue = Blue;

  //Get The Number Of Letters In The Sentence
  NumLetters = (int)strlen(Text);

  //Check For Possible Buffer Overflow
  if (NumLetters > Sentence->MaxLength)
  {
	return false;
  }

  //Create The Vertex Array
  Vertices = new VertexType[Sentence->VertexCount];
  if (!Vertices)
  {
	return false;
  }

  //Initialize Vertex Array To Zeros At First
  memset(Vertices,0,(sizeof(VertexType)*Sentence->VertexCount));

  //Calculate The X And Y Pixel Position On The Screen To Start Drawing To
  DrawX = (float)(((m_ScreenWidth/2)*-1)+PositionX);
  DrawY = (float)((m_ScreenHeight/2)-PositionY);

  //Use The Font Class To Build The Vertex Array From The Sentence Text And Sentence Draw Location
  m_Font->BuildVertexArray((void*)Vertices,Text,DrawX,DrawY);

  //Lock The Vertex Buffer So It Can Be Written To
  HRes = d_pDevContext->Map(Sentence->VertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&MappedResource);
  if (FAILED(HRes))
  {
	return false;
  }

  //Get A Pointer To The Data In The Vertex Buffer
  VerticesPtr = (VertexType*)MappedResource.pData;

  //Copy The Data Into The Vertex Buffer
  memcpy(VerticesPtr,(void*)Vertices,(sizeof(VertexType)*Sentence->VertexCount));

  //Unlock The Vertex Buffer
  d_pDevContext->Unmap(Sentence->VertexBuffer,0);

  //Release The Vertex Array As It Is No Longer Needed
  DeleteArray(Vertices);
  
  return true;
} //EndUpdateSentenceFunction

//-----------------------------------------------------------------------------
// Name: CXText::Render()
// Desc: 
//-----------------------------------------------------------------------------
bool CXText::Render(D3DXMATRIX WorldMatrix,D3DXMATRIX OrthoMatrix)
{
  bool HRes;

  //Draw Sentence
  if (m_Sentence)
  {
	HRes = RenderSentence(m_Sentence,WorldMatrix,OrthoMatrix);
    if (!HRes) return false;
  }
  
  return true;
} //EndRenderFunction

//-----------------------------------------------------------------------------
// Name: CXText::RenderSentence()
// Desc: 
//-----------------------------------------------------------------------------
bool CXText::RenderSentence(SentenceType* Sentence,D3DXMATRIX WorldMatrix,D3DXMATRIX OrthoMatrix)
{
  unsigned int Stride,Offset;
  D3DXVECTOR4 PixelColor;
  bool HRes;

  //Set Vertex Buffer Stride And Offset
  Stride = sizeof(VertexType); 
  Offset = 0;

  //Set The Vertex Buffer To Active In The Input Assembler So It Can Be Rendered
  d_pDevContext->IASetVertexBuffers(0,1,&Sentence->VertexBuffer,&Stride,&Offset);

  //Set The Index Buffer To Active In The Input Assembler So It Can Be Rendered
  d_pDevContext->IASetIndexBuffer(Sentence->IndexBuffer,DXGI_FORMAT_R32_UINT,0);

  //Set The Type Of Primitive That Should Be Rendered From This Vertex Buffer, In This Case Triangles
  d_pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //Create A Pixel Color Vector With The Input Sentence Color
  PixelColor = D3DXVECTOR4(Sentence->Red,Sentence->Green,Sentence->Blue,1.0f);

  //Render The Text Using The Font Shader
  m_Font->SetWorldMat(WorldMatrix);
  m_Font->SetViewMat(m_BaseViewMatrix);
  m_Font->SetProjMat(OrthoMatrix);
  m_Font->Render(Sentence->IndexCount,PixelColor);

  return true;
} //EndRenderSentenceFunction


//-----------------------------------------------------------------------------
// Name: CXText::SetText()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CXText::SetText(char* Text,int PositionX,int PositionY,float Red,float Green,float Blue)
{
  HRESULT HRes;

  //Update The Sentence Vertex Buffer With The New String Information
  HRes = UpdateSentence(m_Sentence,Text,PositionX,PositionY,Red,Green,Blue);
  if (FAILED(HRes))
  {
	Log("CXText::Initialize - Error Update Text Object Sentence");
	return HRes;
  }

  return S_OK;
} //EndSetTextFinction

//-----------------------------------------------------------------------------