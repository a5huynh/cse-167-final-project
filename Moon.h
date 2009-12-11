/*
 *  Moon.h
 *  BumpMap
 *
 *  Created by Andrew Huynh on 12/10/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#include <GLUT/GLUT.h>
#include <vector>
#include "Vector3.h"
#include "shader.h"

using namespace std;

class Moon {
	public:
		Moon( float );
		
		void draw();
		void toggleShader();
		
	private:
		void setupShader();
		void compile();
		void generate();
		void generateTangent( Vector3& v1, Vector3& v2, Vector3& st1, Vector3& st2, Vector3& tangent );
		
		vector<Vector3*> bmVertices;
		vector<Vector3*> bmNormals;
		vector<Vector3*> bmTangents;
		vector<Vector3*> bmTex;
		GLuint dList, bumpMap;
		Shader *shader;
		
		bool shaderOn;
		int numDivs;
		int radius;
};