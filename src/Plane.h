/*
 *  Plane.h
 *  Project 5.2
 *
 *  Created by Andrew Huynh on 11/12/09.
 */
#pragma once

#include "Vector3.h"

class Plane {

	public:
		Plane();
		Plane( Vector3&, Vector3&, int, int );
		
		void setPoint( Vector3& );
		void setPoint( float, float, float );		
		
		void setNormal( Vector3& );
		void setNormal( float, float, float );
		
		void setWidth( float );
		void setHeight( float );

		float getWidth();
		float getHeight();
		
		Vector3& getPoint();
		Vector3& getNormal();
		
		float distance( Vector3& );
		
	private:
		float width, height;
		Vector3 point;
		Vector3 normal;
};
