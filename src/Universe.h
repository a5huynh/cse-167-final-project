/*
 *  Universe.h
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/9/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */

#pragma once

#include <GLUT/GLUT.h>
#include <vector>

#include "Vector3.h"

using namespace std;

class Universe {
	public:
		Universe();
		
		void draw();
			
	private:
		void generate();

		GLuint texture, dList;
		bool isCompiled;
		vector<Vector3*> vertices;
		vector<Vector3*> normals;
		vector<Vector3*> texCoords;
		
		int numDivs;
};