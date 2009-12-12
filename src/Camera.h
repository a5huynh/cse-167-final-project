/*
 *  Camera.h
 *  Lab 2
 *
 *  Created by Andrew Huynh on 10/13/09.
 *
 */
#pragma once

#include "Matrix4.h"
#include "Vector3.h"
#include "Object3D.h"

class Camera : public Object3D {
	public:
		Camera();
		Camera( Vector3&, Vector3&, Vector3& );
	
		Vector3& getCenterOfProjection();
		void setCenterOfProjection( Vector3& );
		void setCenterOfProjection( float, float, float );
		
		Vector3& getLookAtPoint();
		void setLookAtPoint( Vector3& );
		
		Vector3& getUp();
		void setUp( Vector3& );
		
		virtual void draw();
		
		void setupCamera();
		Matrix4& getMatrix();
		
		float getDistFromOrigin();
		
	private:
		Vector3 e;		// Center of Projection
		Vector3 d;		// Look at Point
		Vector3 up;		// Up Vector
		
		float originDist;
		
		Matrix4 camera;
};