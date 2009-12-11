/*
 *  trackball.cpp
 *  Lab 2.3
 *
 *  Created by Andrew Huynh on 10/15/09.
 */

#include "trackball.h"

Matrix4 trackBallMatrix;
Vector3 last, axis;
float theta;

void initTrackBall() {
	trackBallMatrix.identity();
}

void mapPoint( Vector3& v, int x, int y ) {

	// Scale bounds
	float newX = (float)x / (768 / 2 );
	float newY = (float)y / (1024 / 2 );
	
	// Translate 0, 0 to the center
	newX = newX - 1;
	
	// Flip so +Y is up instead of don
	newY = 1 - newY;
	
	float z2 = 1 - newX*newX - newY*newY;
	float z  = z2 > 0 ? sqrt( z2 ) : 0;
	
	v.setX( newX );
	v.setY( newY );
	v.setZ( z );
	v.normalize();
}

void handleMouse( int button, int state, int x, int y ) {
	if( state == 0 ) {
		mapPoint( last, x, y );
	}
}

void handleRotate( int x, int y ) {
	Vector3 now;
	
	mapPoint( now, x, y );
	
	axis.cross( last, now );
	axis.normalize();
	
	// Make sure the dot product is within the bounds of acos
	float dot = last.dot( now );
	if( dot > 1.0 ) {
		dot = 1.0;
	} else if ( dot < -1.0 ) {
		dot = -1.0;
	}
	
	theta = acos( dot );
	
	last = now;
	
	axis = trackBallMatrix.multiply( axis );
	if( now.getZ() > 0 ) {
		axis.setZ( -axis.getZ() );
	} else {
		axis.setX( -axis.getX() );
	}	
	axis.normalize();
	
	Matrix4 *tmp = &Matrix4::makeRotateArbitrary( axis.getX(), axis.getY(), axis.getZ(), theta );
	trackBallMatrix.multiply( trackBallMatrix, *tmp );
	delete tmp;
}

Matrix4& getTrackBallMatrix() {
	return trackBallMatrix;
}