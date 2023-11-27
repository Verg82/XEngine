/****************************************************************************/ 
// It's Unofficial Version Of The XEngine v.2.0.0.1
// Font.fx
// This Effect Contains The Font Implementation Shader.
// Date Creation: 14 May 2014
// Last Changing: 05 June 2014
// Author Rights By: Native Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2005-2014
/*****************************************************************************/ 
//-----------------------------------------------------------------------------
// Name: Globals
// Desc: 
//-----------------------------------------------------------------------------
Texture2D ShaderTexture;
SamplerState SampleType;

cbuffer PerFrameBuffer
{
  matrix WorldMatrix;
  matrix ViewMatrix;
  matrix ProjectionMatrix;
};

cbuffer PixelBuffer
{
  float4 PixelColor;
};

//-----------------------------------------------------------------------------
// Name: VertexInputType
// Desc: Input Structure For Font Vertex Shader
//-----------------------------------------------------------------------------
struct VertexInputType
{
  float4 Pos: POSITION;
  float2 TexCoord: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: PixelInputType
// Desc: Input Structure For Font Pixel Shader
//-----------------------------------------------------------------------------
struct PixelInputType
{
  float4 Pos: SV_POSITION;
  float2 TexCoord: TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Name: FontVertexShader()
// Desc: 
//-----------------------------------------------------------------------------
PixelInputType FontVertexShader(VertexInputType Input)
{
  PixelInputType Output;
    
  //Change The Position Vector To Be 4 Units For Proper Matrix Calculations
  Input.Pos.w = 1.0f;
  
  //Calculate The Position Of The Vertex Against The World, View, And Projection Matrices
  Output.Pos = mul(Input.Pos,WorldMatrix);
  Output.Pos = mul(Output.Pos,ViewMatrix);
  Output.Pos = mul(Output.Pos,ProjectionMatrix);
    
  //Store The Texture Coordinates For The Pixel Shader
  Output.TexCoord = Input.TexCoord;
    
  return Output;
}

//-----------------------------------------------------------------------------
// Name: FontPixelShader()
// Desc: 
//-----------------------------------------------------------------------------
float4 FontPixelShader(PixelInputType Input): SV_TARGET
{
  float4 Color;
	
  //Sample The Texture Pixel At This Location
  Color = ShaderTexture.Sample(SampleType,Input.TexCoord);
	
  //If The Color Is Black On The Texture Then Treat This Pixel As Transparent
  if (Color.r == 0.0f)
  {
    Color.a = 0.0f;
  }
	
  //If The Color Is Other Than Black On The Texture Then This Is A Pixel 
  //In The Font So Draw It Using The Font Pixel Color
  else
  {
    Color.rgb = PixelColor.rgb;
    Color.a = 1.0f;
  }

  return Color;
}

//-----------------------------------------------------------------------------