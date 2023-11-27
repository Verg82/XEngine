//--------------------------------------------------------------------------------------
// File: ForwardPlus11Common.hlsl
//
// HLSL file for the ForwardPlus11 sample. Common code.
// 
// Author: Jason Stewart
// 
// Copyright © AMD Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer cbPerObject : register( b0 )
{
    matrix  g_mWorldViewProjection     : packoffset( c0 );
    matrix  g_mWorldView               : packoffset( c4 );
    matrix  g_mWorld                   : packoffset( c8 );
    float4  g_MaterialAmbientColorUp   : packoffset( c12 );
    float4  g_MaterialAmbientColorDown : packoffset( c13 );
}

cbuffer cbPerFrame : register( b1 )
{
    matrix              g_mProjection           : packoffset( c0 );
    matrix              g_mProjectionInv        : packoffset( c4 );
    float3              g_vCameraPos            : packoffset( c8 );
    float               g_fAlphaTest            : packoffset( c8.w );
    uint                g_uNumLights            : packoffset( c9 );
    uint                g_uWindowWidth          : packoffset( c9.y );
    uint                g_uWindowHeight         : packoffset( c9.z );
    uint                g_uMaxNumLightsPerTile  : packoffset( c9.w );
};

//--------------------------------------------------------------------------------------
// Miscellaneous constants
//--------------------------------------------------------------------------------------
#define LIGHT_INDEX_BUFFER_SENTINEL 0x7fffffff

//--------------------------------------------------------------------------------------
// Light culling constants.
// These must match their counterparts in ForwardPlusUtil.h
//--------------------------------------------------------------------------------------
#define TILE_RES 16
#define MAX_NUM_LIGHTS_PER_TILE 544

//-----------------------------------------------------------------------------------------
// Helper functions
//-----------------------------------------------------------------------------------------
uint GetTileIndex(float2 ScreenPos)
{
    float fTileRes = (float)TILE_RES;
    uint nNumCellsX =  (g_uWindowWidth + TILE_RES - 1)/TILE_RES;
    uint nTileIdx = floor(ScreenPos.x/fTileRes)+floor(ScreenPos.y/fTileRes)*nNumCellsX;
    return nTileIdx;
}

