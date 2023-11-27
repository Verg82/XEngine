/****************************************************************************/ 
// It's Unofficial Version Of The XEngine v.2.0.0.1
// SkyMap.fx
// This Effect Contains The Sky Map Implementation Shader.
// Date Creation: 14 May 2014
// Last Changing: 14 May 2014
// Author Rights By: Native Soft And Lucky's Flash Studio Inc.,
// Licensed By: (C)DDD Digital Dreams Development Inc., 2005-2014
/*****************************************************************************/ 

cbuffer cbPerObject
{
  float4x4 WVP;
};

SamplerState ObjSamplerState;
TextureCube SkyMap;

//-----------------------------------------------------------------------------
// Name: SKYMAP_VS_OUTPUT
// Desc: Output Structure For SkyMap Vertex Shader
//-----------------------------------------------------------------------------
struct SKYMAP_VS_OUTPUT
{
  float4 Pos: SV_POSITION;
  float3 TexCoord: TEXCOORD;
};

//-----------------------------------------------------------------------------
// Name: SKYMAP_VS()
// Desc: 
//-----------------------------------------------------------------------------
SKYMAP_VS_OUTPUT SKYMAP_VS(float3 InPos: POSITION,float2 InTexCoord: TEXCOORD,float3 Normal: NORMAL)
{
  SKYMAP_VS_OUTPUT Output = (SKYMAP_VS_OUTPUT)0;

  //Set Pos To xyww Instead Of xyzw, So That z Will Always Be 1 (Furthest From Camera)
  Output.Pos = mul(float4(InPos,1.0f),WVP).xyww;
  Output.TexCoord = InPos;
  return Output;
}

//-----------------------------------------------------------------------------
// Name: SKYMAP_PS()
// Desc: 
//-----------------------------------------------------------------------------
float4 SKYMAP_PS(SKYMAP_VS_OUTPUT Input): SV_Target
{
  return SkyMap.Sample(ObjSamplerState,Input.TexCoord);
}

//-----------------------------------------------------------------------------