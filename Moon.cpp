/*
 *  Moon.cpp
 *  BumpMap
 *
 *  Created by Andrew Huynh on 12/10/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include "Moon.h"
#include "BasicMath.h"
#include "ppmLoader.h"

Moon::Moon( float r ) {
	radius = r;
	numDivs = 50;
	shaderOn = true;
	generate();
	setupShader();	
	compile();
}

void Moon::draw() {
	if( shaderOn )
		shader->bind();
		
	glCallList( dList );
	
	glUseProgram( 0 );
}

void Moon::compile() {
	dList = glGenLists( 1 );
	
	glNewList( dList, GL_COMPILE );
	GLint loc = glGetAttribLocationARB(shader->pid, "tangent");
											
	glColor3f( .5, .5, .5 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, bumpMap );
	
	glBegin( GL_QUADS );
		for( int i = 0; i < numDivs; i++ ) {
			for( int j = 0; j < numDivs; j++ ) {
			
				int i1 = i * (numDivs+1) + j;
				int i2 = (i+1) * (numDivs+1) + j;
				int i3 = (i+1) * (numDivs+1) + j+1;
				int i4 = i * (numDivs+1) + j+1;

				Vector3 *v1 = bmVertices[i1];
				Vector3 *v2 = bmVertices[i2];
				Vector3 *v3 = bmVertices[i3];
				Vector3 *v4 = bmVertices[i4];

				Vector3 *n1 = bmNormals[i1];
				Vector3 *n2 = bmNormals[i2];
				Vector3 *n3 = bmNormals[i3];
				Vector3 *n4 = bmNormals[i4];

				Vector3 *t1 = bmTangents[i1];
				Vector3 *t2 = bmTangents[i2];
				Vector3 *t3 = bmTangents[i3];
				Vector3 *t4 = bmTangents[i4];

				Vector3 *tex1 = bmTex[i1];
				Vector3 *tex2 = bmTex[i2];
				Vector3 *tex3 = bmTex[i3];
				Vector3 *tex4 = bmTex[i4];
			
				glVertexAttrib3f( loc, t1->getX(), t1->getY(), t1->getZ() );
				glNormal3f( n1->getX(), n1->getY(), n1->getZ() );
				glTexCoord2f( tex1->getX(), tex1->getY() );
				glVertex3f( v1->getX(), v1->getY(), v1->getZ() ); 
				
				glVertexAttrib3f( loc, t2->getX(), t2->getY(), t2->getZ() );
				glNormal3f( n2->getX(), n2->getY(), n2->getZ() );
				glTexCoord2f( tex2->getX(), tex2->getY() );
				glVertex3f( v2->getX(), v2->getY(), v2->getZ() ); 

				glVertexAttrib3f( loc, t3->getX(), t3->getY(), t3->getZ() );
				glNormal3f( n3->getX(), n3->getY(), n3->getZ() );
				glTexCoord2f( tex3->getX(), tex3->getY() );
				glVertex3f( v3->getX(), v3->getY(), v3->getZ() ); 

				glVertexAttrib3f( loc, t4->getX(), t4->getY(), t4->getZ() );
				glNormal3f( n4->getX(), n4->getY(), n4->getZ() );
				glTexCoord2f( tex4->getX(), tex4->getY() );
				glVertex3f( v4->getX(), v4->getY(), v4->getZ() ); 
			}
		}
	glEnd();
	glEndList();
}

void Moon::setupShader() {
	int w, h;
	unsigned char * texture = loadPPM( "normalMap.ppm", w, h );
	glGenTextures( 1, &bumpMap );
	
	glBindTexture( GL_TEXTURE_2D, bumpMap );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
	
	shader = new Shader("../../bump_map.vert", "../../bump_map.frag", true );
	shader->bind();

	GLint loc1 = glGetUniformLocationARB( shader->pid, "normalMap" );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, bumpMap );
	glUniform1iARB(loc1, 0 );

}

void Moon::generate() {
	float gammaStep = 2*PI / numDivs;
	float thetaStep = PI / numDivs;
	float gamma = 0, theta = 0;
	
	float s = 0, t = 0;
	float sStep = 1.0 / numDivs;
	float tStep = 1.0 / numDivs;

	// Generate planet vertices
	for( int i = 0; i <= numDivs; i++ ) {

		gamma = i * gammaStep;
		s = i * sStep;
	
		for( int j = 0; j <= numDivs; j++ ) {

			theta = j * thetaStep;
			t = j * tStep;
		
			Vector3 *v = new Vector3();
			
			spherePoint( *v, radius, theta, gamma );
			bmVertices.push_back( v );

			Vector3 *n = new Vector3( *v );
			n->normalize();
			bmNormals.push_back( n );

			bmTex.push_back( new Vector3( s, t, 0 ) );
		}
	}	
	
	for( int i = 0; i <= numDivs; i++ ) {
		
		s = i * sStep;
		
		for( int j = 0; j <= numDivs; j++ ) {
			
			t = j * tStep;
			
			Vector3 v1, v2, v3, v4;
			Vector3 tmp, tmp2;
			Vector3 st1, st2, tangent;

			v1 = *bmVertices[ i * (numDivs+1) + j ];
			v2 = *bmVertices[ ((i+1) % numDivs) * (numDivs+1) + j ];

			st1.setX( s ); st1.setY( t + tStep );
			st2.setX( s + sStep ); st2.setY( t );

			if (j < numDivs) {
				v3 = *bmVertices[ ((i+1) % numDivs) * (numDivs+1) + j+1 ];	
				v4 = *bmVertices[ i * (numDivs+1) + j+1 ];
			}
			else {
				v3 = *bmVertices[ ((i+1) % numDivs) * (numDivs+1) + j-1 ];	
				v4 = *bmVertices[ i * (numDivs+1) + j-1 ];
				st1.setX( s ); st1.setY( t - tStep );
			}

			tmp = v2 - v1;
			tmp2 = v4 - v1;
													
			generateTangent( tmp, tmp2, st1, st2, tangent );
			bmTangents.push_back( new Vector3( tangent ) );
		}
	}	
}

void Moon::generateTangent( Vector3& v1, Vector3& v2, Vector3& st1, Vector3& st2, Vector3& tangent ) {

	float coef = 1.0 / ( st1.getX() * st2.getY() - st2.getX() * st1.getY() );

	tangent.setX( coef * ( ( v1.getX() * st2.getY() ) + v2.getX() * -st1.getY() ) );
	tangent.setY( coef * ( ( v1.getY() * st2.getY() ) + v2.getY() * -st1.getY() ) );
	tangent.setZ( coef * ( ( v1.getZ() * st2.getY() ) + v2.getZ() * -st1.getY() ) );
	tangent.normalize();	
}

void Moon::toggleShader(void) {
	shaderOn = !shaderOn;
}
