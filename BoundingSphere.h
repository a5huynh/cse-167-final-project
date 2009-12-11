/*
 *  BoundingSphere.h
 *  Project 5
 *
 *  Created by Andrew Huynh on 11/12/09.
 */
 
#pragma once

#include "Vector3.h"

class BoundingSphere {
	public:
		BoundingSphere();
		
		float getRadius();
		Vector3& getCenter();
				
		void setRadius( float );
		void setCenter( float, float, float );
		
		float distance( Vector3& );
		
	private:
		Vector3 center;
		float radius;
};
