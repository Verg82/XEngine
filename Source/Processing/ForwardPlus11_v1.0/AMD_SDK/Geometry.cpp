//--------------------------------------------------------------------------------------
// File: Geometry.cpp
//
// Classes for geometric primitives and collision and visibility testing
//
// Author: Gareth Thomas
//
// Copyright © AMD Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "..\\DXUT\\Core\\DXUT.h"
#include "Geometry.h"


AMD::BoundingBox AMD::TransformBounds( const BoundingBox& bounds, const D3DXMATRIX& transform )
{
	BoundingBox newBounds;
	newBounds.Reset();

	for ( int i = 0; i < 8; i++ ) 
	{
		D3DXVECTOR4 newPoint;
		D3DXVec4Transform( &newPoint, &bounds.GetPoint( i ), &transform );

		newBounds.AddPoint( newPoint );
	}
	
	return newBounds;
}