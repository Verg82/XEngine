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

#include "PSSMPartitions.h"
#include <d3dx11.h>
#include <cmath>


PSSMPartitions::PSSMPartitions(ID3D11Device *d3dDevice, int partitions)
    : mPartitions(partitions)
{
    // Create partition buffer
    {
        UINT structSize = sizeof(Partition);

        CD3D11_BUFFER_DESC desc(
            structSize * mPartitions,            
            D3D11_BIND_SHADER_RESOURCE,
            D3D11_USAGE_DYNAMIC,
            D3D11_CPU_ACCESS_WRITE,
            D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
            structSize);
        d3dDevice->CreateBuffer(&desc, 0, &mPartitionBuffer);

        CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDesc(
            mPartitionBuffer,
            DXGI_FORMAT_UNKNOWN,
            0, mPartitions, 0);
        d3dDevice->CreateShaderResourceView(mPartitionBuffer, &shaderResourceDesc, &mPartitionSRV);
    }
}


PSSMPartitions::~PSSMPartitions()
{
    mPartitionSRV->Release();
    mPartitionBuffer->Release();
}


ID3D11ShaderResourceView* PSSMPartitions::ComputePartitions(
    ID3D11DeviceContext *d3dDeviceContext,
    const D3DXMATRIXA16& cameraViewInv,
    const D3DXMATRIXA16& cameraProj,
    float minZ, float maxZ,
    const D3DXMATRIXA16& lightViewProj,
    float lambda,
    const D3DXVECTOR3& lightSpaceBorder)
{
    // Compute split distances
    float range = maxZ - minZ;
    float ratio = maxZ / minZ;

    mPSSMDistances.resize(0);
    for (int i = 0; i < mPartitions; ++i) {
        float p = i / static_cast<float>(mPartitions);
        float log = minZ * std::pow(ratio, p);
        float uniform = minZ + range * p;
        float d = lambda * (log - uniform) + uniform;
        mPSSMDistances.push_back(d);
    }
    mPSSMDistances.push_back(maxZ);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    d3dDeviceContext->Map(mPartitionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    Partition* partitions = static_cast<Partition *>(mappedResource.pData);

    for (int i = 0; i < mPartitions; ++i) {
        D3DXVECTOR3 min, max;
        ComputeFrustumExtents(cameraViewInv, cameraProj, mPSSMDistances[i], mPSSMDistances[i + 1],
                              lightViewProj, &min, &max);

        // Clamp to valid range
        min.x = std::min(1.0f, std::max(-1.0f, min.x));
        min.y = std::min(1.0f, std::max(-1.0f, min.y));
        min.z = std::min(1.0f, std::max( 0.0f, min.z));
        max.x = std::min(1.0f, std::max(-1.0f, max.x));
        max.y = std::min(1.0f, std::max(-1.0f, max.y));
        max.z = std::min(1.0f, std::max( 0.0f, max.z));

        // Accomodate edge border
        min -= lightSpaceBorder;
        max += lightSpaceBorder;

        // Extents are in clip space - convert to scale/bias in texture coordinate space
        // Compute scale/bias in shader texture coordinate space
        min.x =  0.5f * min.x + 0.5f;
        min.y = -0.5f * min.y + 0.5f;
        max.x =  0.5f * max.x + 0.5f;
        max.y = -0.5f * max.y + 0.5f;

        // -ve swaps min/max
        std::swap(min.y, max.y);

        // NOTE: Z scale/bias will be the identity for now...
        D3DXVECTOR3 scale;
        scale.x = 1.0f / (max.x - min.x);
        scale.y = 1.0f / (max.y - min.y);
        scale.z = 1.0f / (max.z - min.z);

        D3DXVECTOR3 bias;
        bias.x = -min.x * scale.x;
        bias.y = -min.y * scale.y;
        bias.z = -min.z * scale.z;

        partitions[i].intervalBegin = mPSSMDistances[i];
        partitions[i].intervalEnd = mPSSMDistances[i + 1];
        partitions[i].scale = scale;
        partitions[i].bias = bias;
    }
    
    d3dDeviceContext->Unmap(mPartitionBuffer, 0);

    return mPartitionSRV;
}
