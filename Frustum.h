/*
 *  Frustum.h
 *  Lab 2
 *
 *  Created by Andrew Huynh on 10/13/09.
 *
 */

#include <math.h>

#include "Matrix4.h"
#include "Camera.h"
#include "Plane.h"

enum FrustumTypes { NEAR, FAR, LEFT, RIGHT, TOP, BOTTOM };

class Frustum {
	public:
		Frustum();
		Frustum( float, float, float, float );
		void setProjectionMatrix();
		
		Matrix4& getMatrix();
		
		float getFOV();
		float getAspect();
		float getNear();
		float getFar();
		
		void generateFrustumPlanes( Camera*, Plane* );
		
	private:
		float fov, aspect, near, far;
		Matrix4 projection;
};
