/*
 *  Object3D.cpp
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/11/09.
 */

#include <GLUT/glut.h>
#include <stdio.h>

#include "Object3D.h"

Object3D::Object3D() {
	// Initialize 3D object
}

Object3D::~Object3D() {}

// By default we'll draw a simply sphere
void Object3D::draw() {
	printf("drawing sphere...\n");
	glutSolidSphere(6.0, 120, 120); 
}

void Object3D::print() {
	printf("Default sphere object\n");
}

BoundingSphere* Object3D::getBoundingSphere() {
	return NULL;
}