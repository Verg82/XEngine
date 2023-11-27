/****************************************************************************/ 
// It's Unofficial Version Of The XEngine v.2.0.0.1
// SkyPlane.fx
// This Effect Contains The SkyPlane Perturbed Clouds Implementation Shader.
// Date Creation: 16 June 2014
// Last Changing: 18 June 2014
// Author Rights By: Native Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2005-2014
/*****************************************************************************/ 
//-----------------------------------------------------------------------------
// Name: Globals
// Desc: 
//-----------------------------------------------------------------------------
Texture2D CloudTexture: register(t0);
Texture2D PerturbTexture: register(t1);
SamplerState SampleType;

cbuffer MatrixBuffer
{	
  matrix WorldMatrix;
  matrix PosMatrix;
  matrix ViewMatrix;
  matrix ProjMatrix;
};

cbuffer SkyBuffer
{
  float Translation;
  float Scale;
  float Brightness;
  float Padding;
};

//-----------------------------------------------------------------------------
// Name: VertexInputType
// Desc: Output Structure For SkyPlane Vertex Shader
//-----------------------------------------------------------------------------
struct VertexInputType
{
  float4 Pos: POSITION;
  float2 TexCoord: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: PixelInputType
// Desc: Input Structure For SkyPlane Pixel Shader
//-----------------------------------------------------------------------------
struct PixelInputType
{
  float4 Pos: SV_POSITION;
  float2 TexCoord: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: SkyPlaneVertexShader()
// Desc: 
//-----------------------------------------------------------------------------
PixelInputType SkyPlaneVertexShader(VertexInputType Input)
{
  PixelInputType Output;
    
  //Change The Position Vector To Be 4 Units For Proper Matrix Calculations
  Input.Pos.w = 1.0f;

  //Calculate The Position Of The Vertex Against The World, View, And Projection Matrices
  Output.Pos = mul(Input.Pos,WorldMatrix);
  Output.Pos = mul(Input.Pos,PosMatrix);
  Output.Pos = mul(Output.Pos,ViewMatrix);
  Output.Pos = mul(Output.Pos,ProjMatrix);

  //Store The Texture Coordinates For The Pixel Shader
  Output.TexCoord = Input.TexCoord;
	
  return Output;
}

//-----------------------------------------------------------------------------
// Name: SkyPlanePixelShader()
// Desc: 
//-----------------------------------------------------------------------------
float4 SkyPlanePixelShader(PixelInputType Input): SV_TARGET
{
  float4 PerturbValue;
  float4 CloudColor;

  //Translate The Texture Coordinate Sampling Location By The Translation Value
  Input.TexCoord.x = Input.TexCoord.x+Translation;

  //Sample The Texture Value From The Perturb Texture Using The Translated Texture Coordinates
  PerturbValue = PerturbTexture.Sample(SampleType,Input.TexCoord);

  //Multiply The Perturb Value By The Perturb Scale
  PerturbValue = PerturbValue*Scale;

  //Add The Texture Coordinates As Well As The Translation Value To Get The Perturbed 
  //Texture Coordinate Sampling Location
  PerturbValue.xy = PerturbValue.xy+Input.TexCoord.xy+Translation;

  //Now Sample The Color From The Cloud Texture Using The Perturbed Sampling Coordinates
  CloudColor = CloudTexture.Sample(SampleType,PerturbValue.xy);

  //Reduce The Color Cloud By The Brightness Value
  CloudColor = CloudColor*Brightness;

  return CloudColor;
}

//-----------------------------------------------------------------------------