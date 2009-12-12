/*
 *  Universe.cpp
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/9/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include "Universe.h"
#include "ppmLoader.h"
#include "BasicMath.h"

Universe::Universe() {

	numDivs = 50;
	isCompiled = false;

	int w, h;

	unsigned char * textureData = loadPPM( "stars.ppm", w, h );
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	

	glActiveTexture(GL_TEXTURE0);

	dList = glGenLists( 1 );
	generate();
	
}

void Universe::draw() {
	glEnable(GL_TEXTURE_2D);

	if (!isCompiled) {
		glNewList( dList, GL_COMPILE_AND_EXECUTE );
		glColor3f(1, 1, 1);
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture );
		glBegin( GL_QUADS );
		for( int i = 0; i < numDivs; i++ ) {
			for( int j = 0; j < numDivs; j++ ) {
			
				int i1 = i * (numDivs+1) + j;
				int i2 = (i+1) * (numDivs+1) + j;
				int i3 = (i+1) * (numDivs+1) + j+1;
				int i4 = i * (numDivs+1) + j+1;

				Vector3 *v1 = vertices[i1];
				Vector3 *v2 = vertices[i2];
				Vector3 *v3 = vertices[i3];
				Vector3 *v4 = vertices[i4];

				Vector3 *n1 = normals[i1];
				Vector3 *n2 = normals[i2];
				Vector3 *n3 = normals[i3];
				Vector3 *n4 = normals[i4];

				Vector3 *t1 = texCoords[i1];
				Vector3 *t2 = texCoords[i2];
				Vector3 *t3 = texCoords[i3];
				Vector3 *t4 = texCoords[i4];
			
				glNormal3f( n4->getX(), n4->getY(), n4->getZ() ); 
				glTexCoord2f( t4->getX(), t4->getY() );
				glVertex3f( v4->getX(), v4->getY(), v4->getZ() ); 				

				glNormal3f( n3->getX(), n3->getY(), n3->getZ() ); 
				glTexCoord2f( t3->getX(), t3->getY() );
				glVertex3f( v3->getX(), v3->getY(), v3->getZ() );

				glNormal3f( n2->getX(), n2->getY(), n2->getZ() ); 
				glTexCoord2f( t2->getX(), t2->getY() );
				glVertex3f( v2->getX(), v2->getY(), v2->getZ() );

				glNormal3f( n1->getX(), n1->getY(), n1->getZ() ); 
				glTexCoord2f( t1->getX(), t1->getY() );
				glVertex3f( v1->getX(), v1->getY(), v1->getZ() ); 

			}
		}
		glEnd();
		glEndList();
		isCompiled = true;
	}
	else {
		glCallList( dList );
	}

	glDisable(GL_TEXTURE_2D);
}

void Universe::generate() {

	float gammaStep = 2*PI / numDivs;
	float thetaStep = PI / numDivs;
	float gamma = 0, theta = 0;
	
	float s = 0, t = 0;
	float sStep = 5.f / numDivs;
	float tStep = 5.f / numDivs;
	
	for( int i = 0; i <= numDivs; i++ ) {

		gamma = i * gammaStep;
		s = i * sStep;
	
		for( int j = 0; j <= numDivs; j++ ) {

			theta = j * thetaStep;
			t = j * tStep;

			Vector3 *v = new Vector3();
			
			spherePoint( *v, 800, theta, gamma );
			vertices.push_back( v );

			Vector3 *n = new Vector3( *v );
			n->normalize();
			n->scale(-1);
			normals.push_back( n );

			texCoords.push_back( new Vector3( s, t, 0 ) );
		}
	}
		
}