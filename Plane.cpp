/*
 *  Plane.cpp
 *  Project 5.2
 *
 *  Created by Andrew Huynh on 11/12/09.
 */

#include "Plane.h"

Plane::Plane() {}

Plane::Plane( Vector3& p, Vector3& n, int w, int h ) {
	point = p;
	normal = n;
	width = w;
	height = h;
}

void Plane::setPoint( Vector3& p ) {
	point = p;
}

void Plane::setPoint( float x, float y, float z ) {
	point.setX( x ); point.setY( y ); point.setZ( z );
}

void Plane::setNormal( Vector3& n ) {
	normal = n;
}

void Plane::setNormal( float x, float y, float z ) {
	normal.setX( x ); normal.setY( y ); normal.setZ( z );
}

void Plane::setWidth( float w ) {
	width = w;
}

void Plane::setHeight( float h ) {
	height = h;
}

float Plane::getWidth() {
	return width;
}

float Plane::getHeight() {
	return height;
}

Vector3& Plane::getPoint() {
	return point;
}

Vector3& Plane::getNormal() {
	return normal;
}

float Plane::distance( Vector3& p ) {
	Vector3 res;
	res.sub( p, point );
	return res.dot( normal );
}