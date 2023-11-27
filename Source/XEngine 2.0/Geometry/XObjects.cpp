/****************************************************************************/ 
// It's An Unofficial Version XEngine v.2.0.0.1
// XObjects.cpp Visual Studio 2012 Version.
// It's The XEngine Object Loader Interface.
// Made Specially For Implementing Loading And Rendering 3D Objects.
// Date Creation: 21 April 2014
// Last Changing: 02 May 2014
// Author Rights By: (C)DDD Digital Dreams Development And (C)3D Studios Inc., 2014
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
// Interfaces And Headers 
///////////////////////////////////////////////////////////////////////////////
#include <XObjects.h>
#include <XMethods.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction For CXCubeObj Class
///////////////////////////////////////////////////////////////////////////////
CXCubeObj::CXCubeObj()
{
} //EndConstructionDirectives

CXCubeObj::~CXCubeObj()
{ 
} //EndDestructionDirectives

//-----------------------------------------------------------------------------
// Name: CXCubeObj::Create()
// Desc: 
//-----------------------------------------------------------------------------
void CXCubeObj::Create(ID3D10Device* pDevice)
{
  //Set Default Parameters
  g_pEffect = NULL;
  g_pTechnique = NULL;
  g_pVertexLayout = NULL;
  g_pVertexBuffer = NULL;
  g_pIndexBuffer = NULL;
  g_pWorldVariable = NULL;
  g_pViewVariable = NULL;
  g_pProjectionVariable = NULL;

  d_pDevice = pDevice;      //Apropritate Device Pointer To Class
 
  //Create The Effect
  DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
  //#if defined(DEBUG) || defined(_DEBUG)
  //Set The D3D10_SHADER_DEBUG Flag To Embed Debug Information In The Shaders.
  //Setting This Flag Improves The Shader Debugging Experience, But Still Allows 
  //The Shaders To Be Optimized And To Run Exactly The Way They Will Run In 
  //The Release Configuration Of This Program.
  //dwShaderFlags |= D3D10_SHADER_DEBUG;
  //#endif

  if (FAILED(D3DX10CreateEffectFromFile(L"\Shaders\\ObjectFX.fx",NULL,NULL,"fx_4_0",dwShaderFlags,0, 
  d_pDevice,NULL,NULL,&g_pEffect,NULL,NULL)))
  {
    ShowMessage(L"The FX file cannot be located. Please run this executable from the directory that contains the FX file.");
	return;
  }

  //Obtain The Technique
  g_pTechnique = g_pEffect->GetTechniqueByName("Render");

  //Obtain The Variables
  g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
  g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
  g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();

  //Define The Input Layout
  D3D10_INPUT_ELEMENT_DESC Layout[] =
  {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT NumElements = sizeof(Layout)/sizeof(Layout[0]);

  //Create The Input Layout
  D3D10_PASS_DESC PassDesc;
  g_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
  if (FAILED(d_pDevice->CreateInputLayout(Layout,NumElements,PassDesc.pIAInputSignature,
  PassDesc.IAInputSignatureSize,&g_pVertexLayout)))
  {
    ShowMessage(L"Error To Create Input Layout");
	return;
  }

  //Set The Input Layout
  d_pDevice->IASetInputLayout(g_pVertexLayout);

  //Create Vertex Buffer
  SimpleVertex Vertices[] =
  {
    { D3DXVECTOR3(-1.0f, 1.0f, -1.0f),  D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(1.0f, 1.0f, -1.0f),   D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3(1.0f, 1.0f, 1.0f),    D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f, 1.0f, 1.0f),   D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(1.0f, -1.0f, -1.0f),  D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f) },
    { D3DXVECTOR3(1.0f, -1.0f, 1.0f),   D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f) },
    { D3DXVECTOR3(-1.0f, -1.0f, 1.0f),  D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f) },
  };
  
  D3D10_BUFFER_DESC BufDesc;
  BufDesc.Usage = D3D10_USAGE_DEFAULT;
  BufDesc.ByteWidth = sizeof(SimpleVertex)*8;
  BufDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  BufDesc.CPUAccessFlags = 0;
  BufDesc.MiscFlags = 0;
  D3D10_SUBRESOURCE_DATA InitData;
  InitData.pSysMem = Vertices;
  if (FAILED(d_pDevice->CreateBuffer(&BufDesc,&InitData,&g_pVertexBuffer)))
  {
    ShowMessage(L"Error To Create Vertex Buffer"); 
  }

  //Set Vertex Buffer
  UINT Stride = sizeof(SimpleVertex);
  UINT Offset = 0;
  d_pDevice->IASetVertexBuffers(0,1,&g_pVertexBuffer,&Stride,&Offset);

  //Create Index Buffer
  DWORD Indices[] =
  {
    3,1,0,
    2,1,3,

    0,5,4,
    1,5,0,

    3,4,7,
    0,4,3,

    1,6,5,
    2,6,1,

    2,7,6,
    3,7,2,

    6,4,5,
    7,4,6,
  };
    
  BufDesc.Usage = D3D10_USAGE_DEFAULT;
  BufDesc.ByteWidth = sizeof(DWORD)*36; //36 Vertices Needed For 12 Triangles In A Triangle List
  BufDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
  BufDesc.CPUAccessFlags = 0;
  BufDesc.MiscFlags = 0;
  InitData.pSysMem = Indices;
  if (FAILED(d_pDevice->CreateBuffer(&BufDesc,&InitData,&g_pIndexBuffer)))
  {
    ShowMessage(L"Error To Create Index Buffer"); 
  }
    
  //Set Index Buffer
  d_pDevice->IASetIndexBuffer(g_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);

  //Set Primitive Topology
  d_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  //Initialize The World Matrix
  D3DXMatrixIdentity(&g_World);

  //Initialize The View Matrix
  D3DXVECTOR3 Eye(0.0f, 1.0f, -5.0f);
  D3DXVECTOR3 At(0.0f, 1.0f, 0.0f);
  D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH(&g_View,&Eye,&At,&Up);

} //EndCreateProcedure

//-----------------------------------------------------------------------------
// Name: CXCubeObj::Release()
// Desc: 
//-----------------------------------------------------------------------------
void CXCubeObj::Release()
{
  //Free Object
  if (g_pVertexBuffer) g_pVertexBuffer->Release();
  if (g_pIndexBuffer) g_pIndexBuffer->Release();
  if (g_pVertexLayout) g_pVertexLayout->Release();
  if (g_pEffect) g_pEffect->Release();
} //EndReleaseProcedure

//-----------------------------------------------------------------------------
// Name: CXCubeObj::Render()
// Desc: 
//-----------------------------------------------------------------------------
void CXCubeObj::Render()
{
  //Update Variables
  g_pWorldVariable->SetMatrix((float*)&g_World);
  g_pViewVariable->SetMatrix((float*)&g_View);
  g_pProjectionVariable->SetMatrix((float*)&g_Projection);
   
  //Renders A Triangle
  D3D10_TECHNIQUE_DESC TechDesc;
  g_pTechnique->GetDesc(&TechDesc);
  for(UINT I = 0; I < TechDesc.Passes; ++I)
  {
    g_pTechnique->GetPassByIndex(I)->Apply(0);
    d_pDevice->DrawIndexed(36, 0, 0); //36 Vertices Needed For 12 Triangles In A Triangle List
  }
} //EndRenderProcedure

//-----------------------------------------------------------------------------