/*
 *  BoundingSphere.cpp
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/12/09.
 */

#include "BoundingSphere.h"

BoundingSphere::BoundingSphere() {
	radius = 0;
	center.setX( 0 );
	center.setY( 0 );
	center.setZ( 0 );
}

float BoundingSphere::getRadius() {
	return radius;
}

void BoundingSphere::setRadius( float f ) {
	radius = f;
}

Vector3& BoundingSphere::getCenter() {
	return center;
}

void BoundingSphere::setCenter( float x, float y, float z ) {
	center.setX( x );
	center.setY( y );
	center.setZ( z );
}