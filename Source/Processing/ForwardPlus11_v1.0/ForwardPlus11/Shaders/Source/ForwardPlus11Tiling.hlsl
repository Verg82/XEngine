//--------------------------------------------------------------------------------------
// File: ForwardPlus11.hlsl
//
// HLSL file for the ForwardPlus11 sample. Tiled light culling.
// 
// Author: Jason Stewart
// 
// Copyright © AMD Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


#include "ForwardPlus11Common.hlsl"

#define FLT_MAX         3.402823466e+38F

//-----------------------------------------------------------------------------------------
// Textures and Buffers
//-----------------------------------------------------------------------------------------
Buffer<float4> g_PointLightBufferCenterAndRadius : register( t0 );
Buffer<float4> g_SpotLightBufferCenterAndRadius : register( t1 );

#if ( USE_DEPTH_CULLING == 1 )   // non-MSAA
Texture2D<float> g_DepthTexture : register( t2 );
#elif ( USE_DEPTH_CULLING == 2 ) // MSAA
Texture2DMS<float> g_DepthTexture : register( t2 );
#endif

RWBuffer<uint> g_PerTileLightIndexBufferOut : register( u0 );

//-----------------------------------------------------------------------------------------
// Group Shared Memory (aka local data share, or LDS)
//-----------------------------------------------------------------------------------------
#if ( USE_DEPTH_CULLING == 1 || USE_DEPTH_CULLING == 2 )
groupshared uint ldsZMax;
groupshared uint ldsZMin;
#endif

groupshared uint ldsLightIdxCounter;
groupshared uint ldsLightIdx[MAX_NUM_LIGHTS_PER_TILE];

//-----------------------------------------------------------------------------------------
// Helper functions
//-----------------------------------------------------------------------------------------

// this creates the standard Hessian-normal-form plane equation from three points, 
// except it is simplified for the case where the first point is the origin
float4 CreatePlaneEquation( float4 b, float4 c )
{
    float4 n;

    // normalize(cross( b.xyz-a.xyz, c.xyz-a.xyz )), except we know "a" is the origin
    n.xyz = normalize(cross( b.xyz, c.xyz ));

    // -(n dot a), except we know "a" is the origin
    n.w = 0;

    return n;
}

// point-plane distance, simplified for the case where 
// the plane passes through the origin
float GetSignedDistanceFromPlane( float4 p, float4 eqn )
{
    // dot( eqn.xyz, p.xyz ) + eqn.w, , except we know eqn.w is zero 
    // (see CreatePlaneEquation above)
    return dot( eqn.xyz, p.xyz );
}

// calculate the number of tiles in the horizontal direction
uint GetNumTilesX()
{
    return (uint)( ( g_uWindowWidth + TILE_RES - 1 ) / (float)TILE_RES );
}

// calculate the number of tiles in the vertical direction
uint GetNumTilesY()
{
    return (uint)( ( g_uWindowHeight + TILE_RES - 1 ) / (float)TILE_RES );
}

// convert a point from post-projection space into view space
float4 ConvertProjToView( float4 p )
{
    p = mul( p, g_mProjectionInv );
    p /= p.w;
    return p;
}

// convert a depth value from post-projection space into view space
float ConvertProjDepthToView( float z )
{
    z = 1.f / (z*g_mProjectionInv._34 + g_mProjectionInv._44);
    return z;
}

#if ( USE_DEPTH_CULLING == 1 || USE_DEPTH_CULLING == 2 )
void CalculateMinMaxDepthInLds( uint3 globalThreadIdx, uint depthBufferSampleIdx )
{
#if ( USE_DEPTH_CULLING == 1 )   // non-MSAA
    float depth = g_DepthTexture.Load( uint3(globalThreadIdx.x,globalThreadIdx.y,0) ).x;
#elif ( USE_DEPTH_CULLING == 2 ) // MSAA
    float depth = g_DepthTexture.Load( uint2(globalThreadIdx.x,globalThreadIdx.y), depthBufferSampleIdx ).x;
#endif
    float viewPosZ = ConvertProjDepthToView( depth );
    uint z = asuint( viewPosZ );
    if( depth != 0.f )
    {
        InterlockedMax( ldsZMax, z );
        InterlockedMin( ldsZMin, z );
    }
}
#endif

//-----------------------------------------------------------------------------------------
// Parameters for the light culling shader
//-----------------------------------------------------------------------------------------
#define NUM_THREADS_X TILE_RES
#define NUM_THREADS_Y TILE_RES
#define NUM_THREADS_PER_TILE (NUM_THREADS_X*NUM_THREADS_Y)

//-----------------------------------------------------------------------------------------
// Light culling shader
//-----------------------------------------------------------------------------------------
[numthreads(NUM_THREADS_X, NUM_THREADS_Y, 1)]
void CullLightsCS( uint3 globalIdx : SV_DispatchThreadID, uint3 localIdx : SV_GroupThreadID, uint3 groupIdx : SV_GroupID )
{
    uint localIdxFlattened = localIdx.x + localIdx.y*NUM_THREADS_X;
    uint tileIdxFlattened = groupIdx.x + groupIdx.y*GetNumTilesX();

    if( localIdxFlattened == 0 )
    {
#if ( USE_DEPTH_CULLING == 1 || USE_DEPTH_CULLING == 2 )
        ldsZMin = 0xffffffff;
        ldsZMax = 0;
#endif
        ldsLightIdxCounter = 0;
    }

    float4 frustumEqn[4];
    {   // construct frustum for this tile
        uint pxm = TILE_RES*groupIdx.x;
        uint pym = TILE_RES*groupIdx.y;
        uint pxp = TILE_RES*(groupIdx.x+1);
        uint pyp = TILE_RES*(groupIdx.y+1);

        uint uWindowWidthEvenlyDivisibleByTileRes = TILE_RES*GetNumTilesX();
        uint uWindowHeightEvenlyDivisibleByTileRes = TILE_RES*GetNumTilesY();

        // four corners of the tile, clockwise from top-left
        float4 frustum[4];
        frustum[0] = ConvertProjToView( float4( pxm/(float)uWindowWidthEvenlyDivisibleByTileRes*2.f-1.f, (uWindowHeightEvenlyDivisibleByTileRes-pym)/(float)uWindowHeightEvenlyDivisibleByTileRes*2.f-1.f,1.f,1.f) );
        frustum[1] = ConvertProjToView( float4( pxp/(float)uWindowWidthEvenlyDivisibleByTileRes*2.f-1.f, (uWindowHeightEvenlyDivisibleByTileRes-pym)/(float)uWindowHeightEvenlyDivisibleByTileRes*2.f-1.f,1.f,1.f) );
        frustum[2] = ConvertProjToView( float4( pxp/(float)uWindowWidthEvenlyDivisibleByTileRes*2.f-1.f, (uWindowHeightEvenlyDivisibleByTileRes-pyp)/(float)uWindowHeightEvenlyDivisibleByTileRes*2.f-1.f,1.f,1.f) );
        frustum[3] = ConvertProjToView( float4( pxm/(float)uWindowWidthEvenlyDivisibleByTileRes*2.f-1.f, (uWindowHeightEvenlyDivisibleByTileRes-pyp)/(float)uWindowHeightEvenlyDivisibleByTileRes*2.f-1.f,1.f,1.f) );

        // create plane equations for the four sides of the frustum, 
        // with the positive half-space outside the frustum (and remember, 
        // view space is left handed, so use the left-hand rule to determine 
        // cross product direction)
        for(uint i=0; i<4; i++)
            frustumEqn[i] = CreatePlaneEquation( frustum[i], frustum[(i+1)&3] );
    }

    GroupMemoryBarrierWithGroupSync();

    // calculate the min and max depth for this tile, 
    // to form the front and back of the frustum

#if ( USE_DEPTH_CULLING == 1 || USE_DEPTH_CULLING == 2 )
    float minZ = FLT_MAX;
    float maxZ = 0.f;

#if ( USE_DEPTH_CULLING == 1 )   // non-MSAA
    CalculateMinMaxDepthInLds( globalIdx, 0 );
#elif ( USE_DEPTH_CULLING == 2 ) // MSAA
    uint depthBufferWidth, depthBufferHeight, depthBufferNumSamples;
    g_DepthTexture.GetDimensions( depthBufferWidth, depthBufferHeight, depthBufferNumSamples );
    for( uint sampleIdx=0; sampleIdx<depthBufferNumSamples; sampleIdx++ )
    {
        CalculateMinMaxDepthInLds( globalIdx, sampleIdx );
    }
#endif

    GroupMemoryBarrierWithGroupSync();
    maxZ = asfloat( ldsZMax );
    minZ = asfloat( ldsZMin );
#endif

    // loop over the lights and do a sphere vs. frustum intersection test
    uint uNumPointLights = g_uNumLights & 0xFFFFu;
    for(uint i=0; i<uNumPointLights; i+=NUM_THREADS_PER_TILE)
    {
        uint il = localIdxFlattened + i;
        if( il < uNumPointLights )
        {
            float4 center = g_PointLightBufferCenterAndRadius[il];
            float r = center.w;
            center.xyz = mul( float4(center.xyz, 1), g_mWorldView ).xyz;

            // test if sphere is intersecting or inside frustum
#if ( USE_DEPTH_CULLING != 0 )
            if( -center.z + minZ < r && center.z - maxZ < r )
#else
            if( -center.z < r )
#endif
            {
                if( ( GetSignedDistanceFromPlane( center, frustumEqn[0] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[1] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[2] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[3] ) < r ) )
                {
                    // do a thread-safe increment of the list counter 
                    // and put the index of this light into the list
                    uint dstIdx = 0;
                    InterlockedAdd( ldsLightIdxCounter, 1, dstIdx );
                    ldsLightIdx[dstIdx] = il;
                }
            }
        }
    }

    GroupMemoryBarrierWithGroupSync();

    // and again for spot lights
    uint uNumPointLightsInThisTile = ldsLightIdxCounter;
    uint uNumSpotLights = (g_uNumLights & 0xFFFF0000u) >> 16;
    for(uint j=0; j<uNumSpotLights; j+=NUM_THREADS_PER_TILE)
    {
        uint jl = localIdxFlattened + j;
        if( jl < uNumSpotLights )
        {
            float4 center = g_SpotLightBufferCenterAndRadius[jl];
            float r = center.w;
            center.xyz = mul( float4(center.xyz, 1), g_mWorldView ).xyz;

            // test if sphere is intersecting or inside frustum
#if ( USE_DEPTH_CULLING != 0 )
            if( -center.z + minZ < r && center.z - maxZ < r )
#else
            if( -center.z < r )
#endif
            {
                if( ( GetSignedDistanceFromPlane( center, frustumEqn[0] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[1] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[2] ) < r ) &&
                    ( GetSignedDistanceFromPlane( center, frustumEqn[3] ) < r ) )
                {
                    // do a thread-safe increment of the list counter 
                    // and put the index of this light into the list
                    uint dstIdx = 0;
                    InterlockedAdd( ldsLightIdxCounter, 1, dstIdx );
                    ldsLightIdx[dstIdx] = jl;
                }
            }
        }
    }

    GroupMemoryBarrierWithGroupSync();

    {   // write back
        uint startOffset = g_uMaxNumLightsPerTile*tileIdxFlattened;

        for(uint i=localIdxFlattened; i<uNumPointLightsInThisTile; i+=NUM_THREADS_PER_TILE)
        {
            // per-tile list of light indices
            g_PerTileLightIndexBufferOut[startOffset+i] = ldsLightIdx[i];
        }

        for(uint j=(localIdxFlattened+uNumPointLightsInThisTile); j<ldsLightIdxCounter; j+=NUM_THREADS_PER_TILE)
        {
            // per-tile list of light indices
            g_PerTileLightIndexBufferOut[startOffset+j+1] = ldsLightIdx[j];
        }

        if( localIdxFlattened == 0 )
        {
            // mark the end of each per-tile list with a sentinel (point lights)
            g_PerTileLightIndexBufferOut[startOffset+uNumPointLightsInThisTile] = LIGHT_INDEX_BUFFER_SENTINEL;

            // mark the end of each per-tile list with a sentinel (spot lights)
            g_PerTileLightIndexBufferOut[startOffset+ldsLightIdxCounter+1] = LIGHT_INDEX_BUFFER_SENTINEL;
        }
    }
}


