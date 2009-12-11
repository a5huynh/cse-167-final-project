/*
 *  Transforms.cpp
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/7/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include "Transforms.h"

#include <GLUT/GLUT.h>

Scale::Scale( float x, float y, float z ) {
	sx = x;
	sy = y;
	sz = z;
}

void Scale::apply() {
	glScalef( sx, sy, sz );
}

Rotate::Rotate( float a, float fx, float fy, float fz ) {
	angle = a;
	x = fx;
	y = fy;
	z = fz;
}

void Rotate::apply() {
	glRotatef( angle, x, y, z );
}

void Rotate::set( float a, float fx, float fy, float fz ) {
	angle = a;
	x = fx;
	y = fy;
	z = fz;
}

float Rotate::getAngle() { return angle; }
float Rotate::getX() { return x; }
float Rotate::getY() { return x; }
float Rotate::getZ() { return x; }

Translate::Translate( float fx, float fy, float fz ) {
	x = fx;
	y = fy;
	z = fz;
}

void Translate::apply() {
	glTranslatef( x, y, z );
}

void Translate::set( float fx, float fy, float fz ) {
	x = fx;
	y = fy;
	z = fz;
}

float Translate::getX() { return x; }
float Translate::getY() { return y; }
float Translate::getZ() { return z; }
