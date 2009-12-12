/*
 *  Tree.cpp
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/6/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include "Tree.h"

#include <stdio.h>

Tree::Tree() {
	
	dl = glGenLists(1);
	
	boundingSphere.setCenter( 0, 0, 0 );
	boundingSphere.setRadius( 15 );

	glNewList( dl, GL_COMPILE );
		startTree();
	glEndList();
}

Tree::~Tree() {
	glDeleteLists( dl, 1 );
}

BoundingSphere* Tree::getBoundingSphere() {
	return &boundingSphere;
}

void Tree::generateTree( float offset, float ra, float ra2, int stop ) {

	if ( stop == 0 )
		return;

	glColor3f( .67, .44, .23 );
	glTranslatef( 0, 0, offset );
	glRotatef( ra, 0, 1, 0 );
	glRotatef( ra2, 1, 0, 0 );
	glScalef( .6, .6, .6 );
	glutSolidCone( 2, 20, 10, 10 );

	glPushMatrix();
	generateTree( 7, 45, -30, stop - 1);
	glPopMatrix();

	glPushMatrix();
	generateTree( 7, -45, -30, stop - 1);
	glPopMatrix();

	glPushMatrix();
	generateTree( 7, 30, 60, stop - 1 );
	glPopMatrix();

	glPushMatrix();
	glColor3f( 0, 1, 0 );
	glTranslatef( 0, 0, 20 );
	glutSolidSphere( 8, 10, 10 );
	glPopMatrix();
	
}

void Tree::startTree() {
	// Draw branch
	glColor3f( .67, .44, .23 );
	glRotatef( -90, 1, 0, 0 );
	glutSolidCone( 2, 20, 10, 10 );

	// Fractal
	glPushMatrix();
	generateTree( 7, 45, -30, 2 );
	glPopMatrix();
	
	glPushMatrix();
	generateTree( 7, -45, -30, 2 );
	glPopMatrix();

	glPushMatrix();
	generateTree( 7, 30, 60, 2 );
	glPopMatrix();


	glColor3f( 0, 1, 0 );
	glTranslatef( 0, 0, 20 );
	glutSolidSphere( 6, 10, 10 );
}

void Tree::draw() {
	glCallList( dl );
}

void Tree::print() {
	printf("Fractal Tree\n");
}
