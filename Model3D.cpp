/*
 *  Model3D.cpp
 *  Project 5.2
 *
 *  Created by Andrew Huynh on 11/12/09.
 */
#include <stdio.h>
#include <GLUT/glut.h>
#include <algorithm>

#include "Model3D.h"
#include "objreader.h"
#include <math.h>

Material::Material() {
	r = .7;
	g = .7;
	b = .7;
}

Material::Material( float rc, float gc, float bc ) {
	r = rc;
	g = gc;
	b = bc;
}

void Material::setName( const char *str ) {
	strcpy( name, str );
}
char* Material::getName() { return name; }

void Material::setR( float f ) { r = f; }
void Material::setG( float f ) { g = f; }
void Material::setB( float f ) { b = f; }
float Material::getR() { return r; }
float Material::getG() { return g; }
float Material::getB() { return b; }

void Material::apply() {
	glColor3f( r, g, b );
}

Face::Face() {}

void Face::addVertex( Vector3& v ) {
	vertices.push_back( new Vector3( v ) );
}

void Face::addNormal( Vector3& v ) {
	normals.push_back( new Vector3( v ) );
}

void Face::setMaterial( Material& m ) {
	material.setR( m.getR() );
	material.setG( m.getG() );
	material.setB( m.getB() );
}

vector<Vector3*>* Face::getVertices() { return &vertices; }

void Face::draw() {
	
	material.apply();
	
	if( vertices.size() == 3 ) {
		glBegin( GL_TRIANGLES );
	} else if ( vertices.size() == 4 ) {
		glBegin( GL_QUADS );
	}
	
	for( int i = 0; i < vertices.size(); i++ ) {
		glNormal3f( normals[i]->getX(), normals[i]->getY(), normals[i]->getZ() );
		glVertex3f( vertices[i]->getX(), vertices[i]->getY(), vertices[i]->getZ() );
	}
	
	
	glEnd();
}

Model3D::Model3D( const char* model ) {
	readModel(model);
	normalize();
	isCompiled = false;
}

Model3D::~Model3D() {
	for( int i = 0; i < faces.size(); i++ )
		delete faces[i];
		
	faces.clear();
	glDeleteLists( dList, 1 );
}

BoundingSphere* Model3D::getBoundingSphere() {
	BoundingSphere *bs = new BoundingSphere();
	bs->setRadius( radius );
	
	bs->setCenter( 0, 0, 0 );
	return bs;
}

void Model3D::readMaterial( const char* mat, vector<Material*> &materials ) {

	FILE *mp = fopen( mat, "r" );
	
	if( mp == NULL ) 
		printf( "can't find material file: %s\n", mat );
		
	Material *currMaterial = 0;
	char line[81];
	while( fgets( line, 80, mp ) != NULL ) {
	
		if( strlen( line ) == 0 )
			continue;
			
		if( strstr( line, "newmtl" ) != 0 ) {
		
			char name[80];
			strncpy( name, line + 7, strlen(line) - 8 );
			name[ strlen(line) - 8 ] = '\0';
			
			if( currMaterial != NULL ) {
				materials.push_back( currMaterial );
				currMaterial = NULL;
			}
			
			currMaterial = new Material();
			currMaterial->setName( name );

		} else if( strstr( line, "Kd" )  ) {
		
			char values[80];
			strncpy( values, line + 3, strlen( line ) - 4 );
			values[ strlen(line) - 4 ] = '\0';
			
			float fval[3];
			int i = 0;
			char *tok = strtok( values, " " );
			while( tok != NULL && i < 3 ) {

				fval[ i ] = atof( tok );
				i++;

				tok = strtok( NULL, " " );
			}
			
			currMaterial->setR( fval[0] );
			currMaterial->setG( fval[1] );
			currMaterial->setB( fval[2] );
		}
	}
	
	if( currMaterial != NULL ) {
		materials.push_back( currMaterial );
	}
	
	fclose( mp );
	
}

void Model3D::readModel( const char* model ) {
	FILE * fp = fopen(model,"r");
	
	if( fp == NULL ) 
		printf("can't find file: %s\n", model);

	int numVerts = 0, numNorms = 0, numFaces = 0, numTex = 0;

	// Count the number of vertices/normals/etc
	char line[81];
	while( fgets( line, 80, fp ) != NULL ) {
		if( line[0] == 'v' ) {
			if( line[1] == 'n' ) {
				numNorms++;
			} else if( line[1] == 't' ) {
				numTex++;
			} else {
				numVerts++;
			}
		} else if( line[0] =='f' ) {
			numFaces++;
		}
	}
	fseek(fp, 0, 0);
	
	float *n = new float[ 3 * ( numNorms > numFaces ? numNorms : numFaces ) ];
	float *v = new float[ 3 * numVerts ];
	float *t = new float[ 2 * numTex ];
		
	int nvertices = 0;
	int nnormals = 0;
	int ntexcoords = 0;

	vector<Material*> materials;
	Material *curMaterial = NULL;
	
	while( fgets( line, 80, fp ) != NULL ) {
	
		// Handle materials
		if( strstr( line, "mtllib" ) != 0 ) {
			
			char file[81];
			strncpy( file, line + 7, strlen( line ) - 8 );
			file[ strlen( line ) - 8 ] = '\0';
			
			readMaterial( file, materials );
					
		} else if( strstr( line, "usemtl" ) != 0 ) {
		
			char matName[81];
			strncpy( matName, line + 7, strlen( line ) - 8 );
			matName[ strlen( line ) - 8 ] = '\0';
			
			// Find material to use
			for( int i = 0; i < materials.size(); i++ ) {
				if( strcmp( matName, materials[i]->getName() ) == 0 ) {
					curMaterial = materials[i];
					break;
				}
			}
		
		} if (line[0] == 'v') {
			if (line[1] == 'n') {
				float x, y, z;
				sscanf( &line[2], "%f %f %f\n", &x, &y, &z);
				float l = sqrt(x*x+y*y+z*z);
				x = x/l;
				y = y/l;
				z = z/l;
				n[nnormals]=x;
				nnormals++;
				n[nnormals]=y;
				nnormals++;
				n[nnormals]=z;
				nnormals++;
			} else if (line[1] == 't') {
				float u, v;
				sscanf( &line[2], "%f %f\n", &u, &v);
				t[ntexcoords]=u;
				ntexcoords++;
				t[ntexcoords]=v;
				ntexcoords++;
			} else {
				float x, y, z;
				sscanf( &line[1], "%f %f %f\n", &x, &y, &z);
				v[nvertices] = x;
				nvertices++;
				v[nvertices] = y;
				nvertices++;
				v[nvertices] = z;
				nvertices++;
			}
		} else if (line[0] == 'f') {
		
			Face *face = new Face();
			face->setMaterial( *curMaterial );
		
			char s1[32], s2[32], s3[32];
			int vI, tI, nI;
			sscanf( &line[1], "%s %s %s\n", s1, s2, s3);
			
			Vector3 vertex, normal;

			// Indices for first vertex
			getIndices(s1, &vI, &tI, &nI);
			
			vI = vI - 1;			
			vertex.setX( v[ 3 * vI ] );
			vertex.setY( v[ 3 * vI + 1 ] );
			vertex.setZ( v[ 3 * vI + 2 ] );
			face->addVertex( vertex );
			
			nI = nI - 1;
			normal.setX( n[ 3 * nI ] );
			normal.setY( n[ 3 * nI + 1 ] );
			normal.setZ( n[ 3 * nI + 2 ] );
			face->addNormal( normal );
			
			// Indices for second vertex
			getIndices(s2, &vI, &tI, &nI);

			vI = vI - 1;			
			vertex.setX( v[ 3 * vI ] );
			vertex.setY( v[ 3 * vI + 1 ] );
			vertex.setZ( v[ 3 * vI + 2 ] );
			face->addVertex( vertex );
			
			nI = nI - 1;
			normal.setX( n[ 3 * nI ] );
			normal.setY( n[ 3 * nI + 1 ] );
			normal.setZ( n[ 3 * nI + 2 ] );
			face->addNormal( normal );
			
			// Indices for third vertex
			getIndices(s3, &vI, &tI, &nI);
			
			vI = vI - 1;			
			vertex.setX( v[ 3 * vI ] );
			vertex.setY( v[ 3 * vI + 1 ] );
			vertex.setZ( v[ 3 * vI + 2 ] );
			face->addVertex( vertex );
			
			nI = nI - 1;
			normal.setX( n[ 3 * nI ] );
			normal.setY( n[ 3 * nI + 1 ] );
			normal.setZ( n[ 3 * nI + 2 ] );
			face->addNormal( normal );
			
			faces.push_back( face );
		}	
	}
	
}

void Model3D::getIndices( char* word, int *vindex, int *tindex, int *nindex ) {
	char *null = " ";
	char *ptr;
	char *tp;
	char *np;

	/* by default, the texture and normal pointers are set to the null string */
	tp = null;
	np = null;

	/* replace slashes with null characters and cause tp and np to point */
	/* to character immediately following the first or second slash */

	for (ptr = word; *ptr != '\0'; ptr++) {
		if (*ptr == '/') {
			if (tp == null)
				tp = ptr + 1;
			else
				np = ptr + 1;

			*ptr = '\0';
		}
	}

	*vindex = atoi(word);
	*tindex = atoi(tp);
	*nindex = atoi(np);
}

void Model3D::draw() {

	if( !isCompiled ) {
		dList = glGenLists( 1 );
	
		glNewList( dList, GL_COMPILE_AND_EXECUTE );
			glBegin( GL_TRIANGLES );
				for(int i = 0; i < faces.size(); i++ ) {
					faces[i]->draw();
				}
			glEnd();
		glEndList();
		
		isCompiled = true;
		
	} else {
		glCallList( dList );
	}
}

void Model3D::print() {
}

void Model3D::normalize() {	
	float minx, miny, minz;
	float maxx, maxy, maxz;
	
	vector<Vector3*> *vertices = faces[0]->getVertices();
	
	minx = maxx = (*vertices)[0]->getX();
	miny = maxy = (*vertices)[0]->getY();
	minz = maxz = (*vertices)[0]->getZ();	
	
	// Discover the min/max x, y, x
	for( int i = 0; i < faces.size(); i++ ) {
		
		vertices = faces[i]->getVertices();
		for( int vi = 0; vi < (*vertices).size(); vi++ ) {
			Vector3 *v = (*vertices)[vi];
			
			if( v->getX() < minx ) {
				minx = v->getX();
			}
			
			if( v->getX() > maxx ) {
				maxx = v->getX();
			}

			if ( v->getY() < miny ) {
				miny = v->getY();
			}
			if( v->getY() > maxy ) {
				maxy = v->getY();
			}

			if ( v->getZ() < minz ) {
				minz = v->getZ();
			}
			if( v->getZ() > maxz ) {
				maxz = v->getZ();
			}		
		}
	}
	
	printf("maxX, minX: %f, %f\n", maxx, minx );
	printf("maxY, minY: %f, %f\n", maxy, miny );
	printf("maxZ, minZ: %f, %f\n", maxz, minz );
	
	// Find origin
	float midx = (maxx - minx)/2 + minx;
	float midy = (maxy - miny)/2 + miny;
	float midz = (maxz - minz)/2 + minz;

	printf("midX: %f\n", midx );	
	printf("midY: %f\n", midy );
	printf("midZ: %f\n", midz );

	// We want it to fit in a unit box of 1x1x1
	float scaleFactor = 1 / max( maxx - minx, max( maxy - miny, maxz - minz) );
	printf("scale: %f\n", scaleFactor );
	
	// Center at origin & scale to unit size
	for( int i = 0; i < faces.size(); i++ ) {
		vertices = faces[i]->getVertices();
		for( int vi = 0; vi < (*vertices).size(); vi++ ) {
		
			Vector3 *v = (*vertices)[vi];
			
			v->setX( ( v->getX() - midx ) * scaleFactor );
			v->setY( ( v->getY() - midy ) * scaleFactor );
			v->setZ( ( v->getZ() - midz ) * scaleFactor );
		
		}
	}
	
	radius = 0;
	// Loop through all vertices
	for( int i = 0; i < faces.size(); i++ ) {
		vertices = faces[i]->getVertices();
		for( int vi = 0; vi < (*vertices).size(); vi++ ) {
			Vector3 *v = (*vertices)[vi];
		
			// Find the distance of the vertex from the origin
			float dist = sqrt( ( pow( v->getX(), 2 ) + pow( v->getY(), 2 )  + 
								 pow( v->getZ(), 2 ) ) );
							
			// Save the the largest distance
			if( dist > radius ) {
				radius = dist;
			}
		}
	}
}