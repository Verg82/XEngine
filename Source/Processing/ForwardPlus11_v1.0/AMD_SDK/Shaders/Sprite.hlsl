//--------------------------------------------------------------------------------------
// File: Sprite.hlsl
//
// Simple screen space shaders for plotting various sprite types.
// 
// Author: Jon Story
// 
// Copyright � AMD Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------

struct VsSpriteInput
{
    float3 v3Pos : POSITION; 
    float2 v2Tex : TEXTURE0; 
};

struct PsSpriteInput
{
    float4 v4Pos : SV_Position; 
    float2 v2Tex : TEXTURE0;
};

struct VsSpriteBorderInput
{
    float3 v3Pos : POSITION; 
};

struct PsSpriteBorderInput
{
    float4 v4Pos : SV_Position; 
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D				g_SpriteTexture         : register( t0 );
Texture2DMS<float4, 4>  g_SpriteTextureMS       : register( t1 );
Texture2DMS<float, 8>   g_SpriteDepthTextureMS  : register( t2 );

Texture3D<float>		g_VolumeTexture         : register( t0 );

SamplerState        g_Sampler  : register( s0 );


//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------

cbuffer cbSprite
{
    float4	g_f4ViewportSize;
    float4  g_f4PlotParams;
    float4  g_f4TextureSize;
    float4  g_f4DepthRange;
	float4  g_f4SpriteColor;
    float4  g_f4BorderColor;
    float4  g_f4SampleIndex;
};


//--------------------------------------------------------------------------------------
// Vertex Shaders
//--------------------------------------------------------------------------------------

PsSpriteInput VsSprite( VsSpriteInput I )
{
    PsSpriteInput O = (PsSpriteInput)0;

    // Output our final position
    O.v4Pos.x = ( ( g_f4PlotParams.z * I.v3Pos.x + g_f4PlotParams.x ) / ( g_f4ViewportSize.x / 2.0f ) ) - 1.0f;
    O.v4Pos.y = -( ( g_f4PlotParams.w * I.v3Pos.y + g_f4PlotParams.y ) / ( g_f4ViewportSize.y / 2.0f ) ) + 1.0f;
    O.v4Pos.z = I.v3Pos.z;
    O.v4Pos.w = 1.0f;
    
    // Propogate texture coordinate
    O.v2Tex = I.v2Tex;
     
    return O;
}

PsSpriteBorderInput VsSpriteBorder( VsSpriteBorderInput I )
{
    PsSpriteBorderInput O = (PsSpriteBorderInput)0;

    // Output our final position
    O.v4Pos.x = ( ( g_f4PlotParams.z * I.v3Pos.x + g_f4PlotParams.x ) / ( g_f4ViewportSize.x / 2.0f ) ) - 1.0f;
    O.v4Pos.y = -( ( g_f4PlotParams.w * I.v3Pos.y + g_f4PlotParams.y ) / ( g_f4ViewportSize.y / 2.0f ) ) + 1.0f;
    O.v4Pos.z = I.v3Pos.z;
    O.v4Pos.w = 1.0f;
     
    return O;
}


//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------

float4 PsSprite( PsSpriteInput I ) : SV_Target
{
    return g_f4SpriteColor * g_SpriteTexture.Sample( g_Sampler, I.v2Tex );
}


float4 PsSpriteVolume( PsSpriteInput I ) : SV_Target
{
	int3 texCoord;
    texCoord.x = (int)(I.v2Tex.x * g_f4TextureSize.x);
    texCoord.y = (int)(I.v2Tex.y * g_f4TextureSize.y);
	texCoord.z = g_f4TextureSize.z;
	float value = g_VolumeTexture.Load( int4( texCoord, 0) );
    return g_f4SpriteColor * value;
}



float4 PsSpriteUntextured( PsSpriteInput I ) : SV_Target
{
    return g_f4SpriteColor;
}

float4 PsSpriteMS( PsSpriteInput I ) : SV_Target
{
    int2 n2TexCoord;
    n2TexCoord.x = (int)(I.v2Tex.x * g_f4TextureSize.x);
    n2TexCoord.y = (int)(I.v2Tex.y * g_f4TextureSize.y);
    
    float4 v4Color;
    v4Color.x = 0.0f;    
    v4Color.y = 0.0f;
    v4Color.z = 0.0f;
    v4Color.w = 0.0f;
    
    switch( g_f4SampleIndex.x )
    {
    case 0:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 0 );
        break;
    case 1:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 1 );
        break;
    case 2:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 2 );
        break;
    case 3:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 3 );
        break;
    case 4:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 4 );
        break;
    case 5:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 5 );
        break;
    case 6:
		v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 6 );
        break;
    case 7:
        v4Color = g_SpriteTextureMS.Load( int3( n2TexCoord, 0), 7 );
        break;
    }
    
    return v4Color;
}

float4 PsSpriteAsDepth( PsSpriteInput I ) : SV_Target
{
    float4 v4Color = g_SpriteTexture.Sample( g_Sampler, I.v2Tex );
    
    v4Color.x = 1.0f - v4Color.x;
    
    if( v4Color.x < g_f4DepthRange.x )
    {
        v4Color.x = g_f4DepthRange.x;
    }
    
    if( v4Color.x > g_f4DepthRange.y )
    {
        v4Color.x = g_f4DepthRange.y;
    }
    
    float fRange = g_f4DepthRange.y - g_f4DepthRange.x;
    
    v4Color.x = ( v4Color.x - g_f4DepthRange.x ) / fRange;
    
    return v4Color.xxxw;
}

float4 PsSpriteAsDepthMS( PsSpriteInput I ) : SV_Target
{
    int2 n2TexCoord;
    n2TexCoord.x = (int)(I.v2Tex.x * g_f4TextureSize.x);
    n2TexCoord.y = (int)(I.v2Tex.y * g_f4TextureSize.y);
    
    float fColor = 0.0f;
        
    switch( g_f4SampleIndex.x )
    {
    case 0:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 0 ).x;
        break;
    case 1:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 1 ).x;
        break;
    case 2:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 2 ).x;
        break;
    case 3:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 3 ).x;
        break;
    case 4:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 4 ).x;
        break;
    case 5:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 5 ).x;
        break;
    case 6:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 6 ).x;
        break;
    case 7:
        fColor = g_SpriteDepthTextureMS.Load( int3( n2TexCoord, 0), 7 ).x;
        break;
    }
    
    fColor = 1.0f - fColor;
    
    if( fColor < g_f4DepthRange.x )
    {
        fColor = g_f4DepthRange.x;
    }
    
    if( fColor > g_f4DepthRange.y )
    {
        fColor = g_f4DepthRange.y;
    }
    
    float fRange = g_f4DepthRange.y - g_f4DepthRange.x;
    
    fColor = ( fColor - g_f4DepthRange.x ) / fRange;
            
    return fColor.xxxx;

}

float4 PsSpriteBorder( PsSpriteBorderInput I ) : SV_Target
{
    return g_f4BorderColor;
}


//--------------------------------------------------------------------------------------
// EOF
//--------------------------------------------------------------------------------------
