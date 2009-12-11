/*
 *  BezierObject.h
 *  Project 6
 *
 *  Created by Andrew Huynh on 11/19/09.
 */

#include <vector>

#include <GLUT/glut.h>

#include "Vector3.h"
#include "Object3D.h"

using namespace std;

class BezierObject : public Object3D {
	public:
		void draw();
		void print();
		
	protected:
		vector<Vector3> controlPoints;
		vector<Vector3> objPoints;
		vector<Vector3> objNormals;
		vector<Vector3> objTexCoords;
		unsigned char* texture;
		GLuint textureID[1];		
		float ambient[4];  
		float diffuse[4];  
		float specular[4]; 
		float shininess[1];
		
};

class Apple : public BezierObject {
	public:
		Apple();
		~Apple();		
};

class Bowl : public BezierObject {
	public:
		Bowl();
		~Bowl();
};

class Table : public BezierObject {
	public:
		Table();
		~Table();
};

class Mushroom : public BezierObject {
	public:
		Mushroom();
		~Mushroom();
};