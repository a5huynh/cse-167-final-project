/*
 *  3DObject.h
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/11/09.
 */
#pragma once
 
#include "BoundingSphere.h"

class Object3D {

	public:
		Object3D();
		virtual ~Object3D();
		
		virtual void draw();
		
		virtual void print();
		
		virtual BoundingSphere* getBoundingSphere();
};