/*
 *  Model3D.h
 *  Project 5.2
 *
 *  Created by Andrew Huynh on 11/12/09.
 */

#pragma once

#include "Object3D.h"
#include "BoundingSphere.h"
#include <GLUT/GLUT.h>
#include <vector>

using namespace std;

class Material {
	public:
		Material();
		Material( float, float, float );
		
		void apply();
		
		void setName( const char* );
		char* getName();
		
		void setR( float );
		float getR();
		
		void setG( float );
		float getG();
		
		void setB( float );
		float getB();
	
	private:
		char name[80];
		float r, g, b;
};

class Face {
	public:
		Face();
		
		void addVertex( Vector3& );
		void addNormal( Vector3& );
		
		void setMaterial( Material& );
		
		void draw();
		
		vector<Vector3*>* getVertices();

	private:
		vector<Vector3*> normals;
		vector<Vector3*> vertices;
		Material material;
};

class Model3D : public Object3D {
	public:
		Model3D( const char* );
		~Model3D();
		
		virtual void draw();
		virtual void print();
		
		virtual BoundingSphere* getBoundingSphere();
	private:
		void readMaterial( const char*, vector<Material*>& );
		void readModel( const char* );
		void getIndices( char*, int*, int*, int* );
		void normalize();
		
		GLuint dList;
		float radius;
		bool isCompiled;
		vector<Face*> faces;
};
