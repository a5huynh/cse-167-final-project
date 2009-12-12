/*
 *  Frustum.cpp
 *  Lab 2
 *
 *  Created by Andrew Huynh on 10/13/09.
 */

#include "Frustum.h"
#include "BasicMath.h"

#include <GLUT/glut.h>

Frustum::Frustum() {
}

Matrix4& Frustum::getMatrix() {
	return projection;
}

Frustum::Frustum( float FOV, float aspectRatio, float nearPlane, float farPlane ) {
	fov = FOV;
	aspect = aspectRatio;
	near = nearPlane;
	far  = farPlane;

	projection = Matrix4();
	
	float f = 1.0 / tan( deg2rad( FOV ) / 2.0 );
	
	projection.set( 0, 0, f / 1 );
	projection.set( 1, 1, f );
	projection.set( 2, 2, ( near + far ) / ( near - far ) );
	projection.set( 2, 3, ( 2.0 * near * far ) / ( near - far ) );
	projection.set( 3, 2, -1 );
	
}

float Frustum::getFOV() { return fov; }
float Frustum::getAspect() { return aspect; }
float Frustum::getNear() { return near; }
float Frustum::getFar() { return far; }

void Frustum::setProjectionMatrix() {	
	glMultMatrixf( projection.getPointer() );
}

void Frustum::generateFrustumPlanes( Camera* camera, Plane* frustum ) {
	// Generate planes
	float nearHeight = 2 * tan( getFOV() / 2 ) * getNear();
	float nearWidth  = nearHeight * getAspect();
	
	//float farHeight  = 2 * tan( getFOV() / 2 ) * getFar();
	//float farWidth = farHeight * getAspect();
		
	Vector3 fc, nc, tmp;
	
	Vector3 ynh, xnw, X, Y, Z;
	
	Z = Vector3( camera->getCenterOfProjection() );
	Z.sub( Z, camera->getLookAtPoint() );
	Z.normalize();
	
	X.cross( camera->getUp(), Z );
	X.normalize();
	
	Y.cross( Z, X );
	
	ynh = Vector3( Y );
	ynh.scale( nearHeight / 2 );
	
	xnw = Vector3( X );
	xnw.scale( nearWidth / 2 );
	
	tmp = Vector3( Z );
	tmp.scale( getNear() );
	nc = Vector3( camera->getCenterOfProjection() ); 
	nc.sub( nc, tmp );
			
	tmp = Vector3( Z );
	tmp.scale( getFar() );
	fc = Vector3( camera->getCenterOfProjection() );
	fc.sub( fc, tmp );
			
	Vector3 aux, normal;
	
	frustum[ NEAR ].setNormal( Z.getX(), Z.getY(), Z.getZ() );
	frustum[ NEAR ].setPoint( nc );
		
	frustum[ FAR  ].setNormal( -Z.getX(), -Z.getY(), -Z.getZ() );	
	frustum[ FAR  ].setPoint( fc );
	
	aux = Vector3( nc );
	aux.add( aux, ynh );
	frustum[ TOP  ].setPoint( aux );
	
	aux.sub( aux, camera->getCenterOfProjection() );
	aux.normalize();
	normal.cross( aux, X );
	frustum[ TOP  ].setNormal( normal.getX(), normal.getY(), normal.getZ() );
	
	aux = Vector3( nc );
	aux.sub( aux, ynh );
	frustum[ BOTTOM ].setPoint( aux );
	
	aux.sub( aux, camera->getCenterOfProjection() );
	aux.normalize();
	normal.cross( X, aux );
	frustum[ BOTTOM ].setNormal( normal.getX(), normal.getY(), normal.getZ() );
	
	aux = Vector3( nc );
	aux.sub( aux, xnw );
	frustum[ LEFT ].setPoint( aux );
	
	aux.sub( aux, camera->getCenterOfProjection() );
	aux.normalize();
	normal.cross( aux, Y );
	frustum[ LEFT ].setNormal( normal.getX(), normal.getY(), normal.getZ() );
	
	aux = Vector3( nc );
	aux.add( aux, xnw );
	frustum[ RIGHT ].setPoint( aux );
	
	aux.sub( aux, camera->getCenterOfProjection() );
	aux.normalize();
	normal.cross( Y, aux );
	frustum[ RIGHT ].setNormal( normal.getX(), normal.getY(), normal.getZ() );
}