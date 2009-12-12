/*
 *  Tree.h
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/6/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */
#pragma once

#include "Object3D.h"
#include <GLUT/GLUT.h>

class Tree : public Object3D {
	public:
		Tree();
		~Tree();
		
		virtual void draw();
		virtual void print();
		
		BoundingSphere* getBoundingSphere();

	private:
		BoundingSphere boundingSphere;
	
		GLuint dl;
		void startTree();
		void generateTree( float, float, float, int );
};