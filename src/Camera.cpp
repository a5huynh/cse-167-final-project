/*
 *  Camera.cpp
 *  Lab 2
 *
 *  Created by Andrew Huynh on 10/13/09.
 *
 */

#include <GLUT/GLUT.h>
#include "Camera.h"
#include "Frustum.h"

Camera::Camera() {
}

Camera::Camera( Vector3& cop, Vector3& lap, Vector3& upv ) {
	// Copy vectors
	e = Vector3( cop );
	d = Vector3( lap );
	up = Vector3( upv );
	
	// Initialize matrix
	camera = Matrix4();
	originDist = e.magnitude();
}

Matrix4& Camera::getMatrix() {

	Vector3 z = Vector3();
	z.sub( e, d );
	z.normalize();
	
	Vector3 x = Vector3();
	x.cross( up, z );
	x.normalize();
	
	Vector3 y = Vector3();
	y.cross( z, x );
	
	camera.set(0, 0, x.getX() );
	camera.set(0, 1, x.getY() );
	camera.set(0, 2, x.getZ() );
	
	camera.set(1, 0, y.getX() );
	camera.set(1, 1, y.getY() );
	camera.set(1, 2, y.getZ() );
	
	camera.set(2, 0, z.getX() );
	camera.set(2, 1, z.getY() );
	camera.set(2, 2, z.getZ() );
	
	camera.set(3, 3, 1 );
	
	return camera;
}

void Camera::setupCamera() {
	glMultMatrixf( getMatrix().getPointer() );
	glTranslatef( -e.getX(), -e.getY(), -e.getZ() );
}

void Camera::setCenterOfProjection( float x, float y, float z ) {
	e.setX( x );
	e.setY( y );
	e.setZ( z );
	originDist = e.magnitude();
}

void Camera::setCenterOfProjection( Vector3& v ) {
	e.setX( v.getX() );
	e.setY( v.getY() );
	e.setZ( v.getZ() );
	originDist = e.magnitude();
}

Vector3& Camera::getCenterOfProjection() {
	return e;
}

void Camera::setLookAtPoint( Vector3& v ) {
	d.setX( v.getX() );
	d.setY( v.getY() );
	d.setZ( v.getZ() );
}

Vector3& Camera::getLookAtPoint() {
	return d;
}

void Camera::draw() {
	setupCamera();
}

void Camera::setUp( Vector3& v ) {
	up.setX( v.getX() );
	up.setY( v.getY() );
	up.setZ( v.getZ() );
}

Vector3& Camera::getUp() {
	return up;
}

float Camera::getDistFromOrigin() {
	return originDist;
}

