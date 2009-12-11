/*
 *  Transforms.h
 *  FinalProject
 *
 *  Created by Andrew Huynh on 12/7/09.
 *  Copyright 2009 ATHLabs. All rights reserved.
 *
 */
#pragma once

class Transform {
	public:
		virtual void apply() = 0;
};

class Scale : public Transform {
	public:
		Scale( float, float, float );
		virtual void apply();
	private:
		float sx, sy, sz;
};

class Rotate : public Transform {
	public:
		Rotate( float, float, float, float );
		virtual void apply();
		
		void set( float, float, float, float );
		float getAngle();
		float getX();
		float getY();
		float getZ();
		
	private:
		float angle, x, y, z;
};

class Translate : public Transform {
	public:
		Translate( float, float, float );
		virtual void apply();
		
		void set( float, float, float );
		
		float getX();
		float getY();
		float getZ();
	private:
		float x, y, z;
};