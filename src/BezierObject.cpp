/*
 *  BezierObject.cpp
 *  Project 6
 *
 *  Created by Andrew Huynh on 11/19/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include "BezierObject.h"

#include <iostream>
#include <GLUT/glut.h>

#include "bezier.h"
#include "ppmLoader.h"

#define DRAW_NORMALS 0

void BezierObject::draw() {	
	
	//glBindTexture( GL_TEXTURE_2D, textureID[0] );
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);	// Use The Good Calculations
	glEnable (GL_LINE_SMOOTH);		
	
	glCullFace( GL_FRONT );
	glPolygonMode( GL_BACK, GL_LINE );
	glLineWidth( 8.0 );
	glDepthMask( GL_FALSE );
	glEnable( GL_COLOR_MATERIAL );

	glColor4f( 0.0, 0.0, 0.0, 1.0 );
	glBegin( GL_TRIANGLE_STRIP );
		for( int i = 0; i < objPoints.size(); i++ ) {
			glNormal3f(	objNormals[i].getX(), objNormals[i].getY(), objNormals[i].getZ() );
			glTexCoord2f( objTexCoords[i].getY(), objTexCoords[i].getX() );
			glVertex3f(	objPoints[i].getX(), objPoints[i].getY(), objPoints[i].getZ() ); 
		}
	glEnd();	
	
	glDepthMask( GL_TRUE );
	glCullFace( GL_BACK );	
	glPolygonMode( GL_FRONT, GL_FILL );
	glDisable( GL_COLOR_MATERIAL );
	
	glMaterialfv( GL_FRONT, GL_AMBIENT,	ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE,	diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR,	specular );
	glMaterialfv( GL_FRONT, GL_SHININESS,	shininess );
	
	glBegin( GL_TRIANGLE_STRIP );
		for( int i = 0; i < objPoints.size(); i++ ) {
			glNormal3f(	objNormals[i].getX(), objNormals[i].getY(), objNormals[i].getZ() );
			glTexCoord2f( objTexCoords[i].getY(), objTexCoords[i].getX() );
			glVertex3f(	objPoints[i].getX(), objPoints[i].getY(), objPoints[i].getZ() ); 
		}
	glEnd();	
	
	if( DRAW_NORMALS ) {
		glDisable( GL_TEXTURE_2D );
		glColor3f( 0, 1, 0 );
		glBegin( GL_LINES );
			for( int i = 0; i < objPoints.size(); i++ ) {
				glVertex3f(	objPoints[i].getX(), objPoints[i].getY(), objPoints[i].getZ() ); 
				glVertex3f(	objPoints[i].getX() + objNormals[i].getX(), 
							objPoints[i].getY() + objNormals[i].getY(), 
							objPoints[i].getZ() + objNormals[i].getZ() );

			}
		glEnd();
		glColor3f( .6, .6, .6 );
		glEnable( GL_TEXTURE_2D );
	}
}

void BezierObject::print() { printf("Bezier Object\n"); }

Apple::Apple() {
	Vector3 p0 = Vector3( 0, 0, 0 );		Vector3 p1 = Vector3( -.2, -1, 0 ); 
	Vector3 p2 = Vector3( -1.5, -.5, 0 );	Vector3 p3 = Vector3( -2, 1, 0 );
	Vector3 p4 = Vector3( -3, 2.5, 0 );		Vector3 p5 = Vector3( -.5, 4, 0 );
	Vector3 p6 = Vector3( 0, 2, 0 );

	controlPoints.push_back( p0 ); controlPoints.push_back( p1 );
	controlPoints.push_back( p2 ); controlPoints.push_back( p3 );
	controlPoints.push_back( p4 ); controlPoints.push_back( p5 );
	controlPoints.push_back( p6 );
	
	bezierCurve( 10, 10, controlPoints, objPoints, objNormals, objTexCoords );
	
	ambient[0] = 1.; ambient[1] = .0f; ambient[2] = .0f; ambient[3] = 1.0f;
	diffuse[0] = 1.f; diffuse[1] = .0f; diffuse[2] = .0f; diffuse[3] = 1.0f;
	specular[0] = 1.; specular[1] = 1.; specular[2] = 1.; specular[3] = 1.0f;
	shininess[0] = 128;	
	
	int w = 256;
	texture = loadPPM( "apple.ppm", w, w );
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, textureID[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, w, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}

Apple::~Apple() {
	glDeleteTextures( 1, textureID );
}


Bowl::Bowl() {
	Vector3 p0 = Vector3( 0, 0, 0 );		Vector3 p1 = Vector3( -3, .3, 0 );
	Vector3 p2 = Vector3( -5, .3, 0 );		Vector3 p3 = Vector3( -5, 4, 0 );
	Vector3 p4 = Vector3( -4.8, 5, 0 );		Vector3 p5 = Vector3( -4.8, 3, 0 );
	Vector3 p6 = Vector3( -3.5, 2, 0 );		Vector3 p7 = Vector3( -2.f, .5f, 0.f );
	Vector3 p8 = Vector3( -1.f, .5f, 0.f ); Vector3 p9 = Vector3( 0.f, .5, 0.f );
	
	controlPoints.push_back( p0 ); controlPoints.push_back( p1 );
	controlPoints.push_back( p2 ); controlPoints.push_back( p3 );
	controlPoints.push_back( p4 ); controlPoints.push_back( p5 );
	controlPoints.push_back( p6 ); controlPoints.push_back( p7 );
	controlPoints.push_back( p8 ); controlPoints.push_back( p9 );
	
	bezierCurve( 10, 100, controlPoints, objPoints, objNormals, objTexCoords );
	
	ambient[0] = .3f; ambient[1] = .3f; ambient[2] = .3f; ambient[3] = 1.0f;
	diffuse[0] = .8f; diffuse[1] = .5f; diffuse[2] = .24f; diffuse[3] = 1.0f;
	specular[0] = 0; specular[1] = 0; specular[2] = 0; specular[3] = 1.0f;
	shininess[0] = 0;		

	int w = 256;
	texture = loadPPM( "bowl.ppm", w, w );
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, textureID[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, w, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

}

Bowl::~Bowl() {
	glDeleteTextures( 1, textureID );
}

Table::Table() {

	Vector3 p0 = Vector3( 0, -10, 0 );
	Vector3 p1 = Vector3( -5, -10, 0 );
	Vector3 p2 = Vector3( -2, -3, 0 );
	Vector3 p3 = Vector3( -2, 0, 0 );
	Vector3 p4 = Vector3( -2, 0, 0 );
	Vector3 p5 = Vector3( -10, 0, 0 );		
	Vector3 p6 = Vector3( -15, 1, 0 );
	Vector3 p7 = Vector3( -10, 1, 0 );		
	Vector3 p8 = Vector3( -2, 1, 0 );
	Vector3 p9 = Vector3( 0,  1, 0 );
	
	controlPoints.push_back( p0 ); controlPoints.push_back( p1 );
	controlPoints.push_back( p2 ); controlPoints.push_back( p3 );
	controlPoints.push_back( p4 ); controlPoints.push_back( p5 );
	controlPoints.push_back( p6 ); controlPoints.push_back( p7 );
	controlPoints.push_back( p8 ); controlPoints.push_back( p9 ); 
	
	bezierCurve( 10, 20, controlPoints, objPoints, objNormals, objTexCoords );

	ambient[0] = .3f; ambient[1] = .3f; ambient[2] = .3f; ambient[3] = 1.0f;
	diffuse[0] = .5f; diffuse[1] = .5f; diffuse[2] = .5f; diffuse[3] = 1.0f;
	specular[0] = .5; specular[1] = .5; specular[2] = .5; specular[3] = 1.0f;
	shininess[0] = 100;

	int w = 256;
	texture = loadPPM( "granite.ppm", w, w );
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, textureID[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, w, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

}

Mushroom::Mushroom() {
	Vector3 p0 = Vector3( 0, 0, 0 );		Vector3 p1 = Vector3( -1, .2, 0 );
	Vector3 p2 = Vector3( -1.1, .5, 0 );	Vector3 p3 = Vector3( -1, 1, 0 );
	Vector3 p4 = Vector3( -.9, 1.5, 0 );	Vector3 p5 = Vector3( -2, 1.5, 0 );
	Vector3 p6 = Vector3( -2.5f, 1.2f, 0 );	Vector3 p7 = Vector3( -2.8f, 1.f, 0.f );
	Vector3 p8 = Vector3( -3.f, 2.f, 0.f );	Vector3 p9 = Vector3( -2.5f, 2.5f, 0.f );
	Vector3 p10 = Vector3( -2.f, 3.f, 0.f );	Vector3 p11 = Vector3( -1.f, 3.5f, 0.f );
	Vector3 p12 = Vector3( 0, 3.2f, 0.f );	
	
	controlPoints.push_back( p0 ); controlPoints.push_back( p1 );
	controlPoints.push_back( p2 ); controlPoints.push_back( p3 );
	controlPoints.push_back( p4 ); controlPoints.push_back( p5 );
	controlPoints.push_back( p6 ); controlPoints.push_back( p7 );
	controlPoints.push_back( p8 ); controlPoints.push_back( p9 );
	controlPoints.push_back( p10 ); controlPoints.push_back( p11 );
	controlPoints.push_back( p12 );
	
	bezierCurve( 10, 20, controlPoints, objPoints, objNormals, objTexCoords );
	
	ambient[0] = .5f; ambient[1] = .5f; ambient[2] = .5f; ambient[3] = 1.0f;
	diffuse[0] = .82f; diffuse[1] = .41f; diffuse[2] = .12f; diffuse[3] = 1.0f;
	specular[0] = 0; specular[1] = 0; specular[2] = 0; specular[3] = 1.0f;
	shininess[0] = 0;

	int w = 256;
	texture = loadPPM( "bowl.ppm", w, w );
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, textureID[0] );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, w, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
}