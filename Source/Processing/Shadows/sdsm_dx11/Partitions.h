// Copyright 2010 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#pragma once

#include "DXUT.h"

__declspec(align(16))
struct Partition
{
    float intervalBegin;
    float intervalEnd;
    D3DXVECTOR3 scale;
    D3DXVECTOR3 bias;
};

enum PartitionScheme {
    KMEANS_PARTITIONS = 0,
    PSSM_PARTITIONS,
    LOG_PARTITIONS,
    LOG_PARTITIONS_REDUCE,
    ADAPTIVE_LOG_PARTITIONS
};

// Handy utility function
// NOTE: Currently assumes light is a directional light (ortho matrix)
void ComputeFrustumExtents(const D3DXMATRIXA16& cameraViewInv,
                           const D3DXMATRIXA16& cameraProj,
                           float nearZ, float farZ,
                           const D3DXMATRIXA16& lightViewProj,
                           D3DXVECTOR3* outMin,
                           D3DXVECTOR3* outMax);

// Computes the distance between the given point and the furthest AABB corner
float ComputeMaxDistanceToAABBCorner(const D3DXVECTOR3& p,
                                     const D3DXVECTOR3& aabbMin,
                                     const D3DXVECTOR3& aabbMax);
