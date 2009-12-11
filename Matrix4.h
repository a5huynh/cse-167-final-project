/*
 *  Matrix4.h
 *  Lab 1
 *
 *  Created by Andrew Huynh on 10/2/09.
 */
#pragma once 

#include "Vector3.h"
#include "Vector4.h"

#define NUM_ROWS 4
#define NUM_COLS 4

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class Matrix4 {
	public:
		Matrix4();
		Matrix4( const Matrix4 & );
		// Overloaded operators
		Matrix4& operator= ( const Matrix4 & );
		
		void identity();
	
		// Multiply
		void multiply( Matrix4 & );
		void multiply( Matrix4&, Matrix4& );
		
		Vector3& multiply( Vector3 & );
		Vector4& multiply( Vector4 & );
		void multiply( Vector4&, Vector4& );
		
		// Make rotation/scaling/translation matrices
		static Matrix4& makeRotate( int, float );
		static Matrix4& makeRotateArbitrary( float, float, float, float );		
		
		static Matrix4& makeScale( float x, float y, float z );
		
		static Matrix4& makeTranslation( float x, float y, float z );
		
		// Rotate functions
		void rotateX( float );
		void rotateY( float );
		void rotateZ( float );
		
		void rotate( float, float, float, float );
		
		// Scale
		
		void scale( float x, float y, float z );
	
		// Setters/Getters
		void set( int, int, float );
		float get( int, int ) const;
		
		void print();
		
		float* getPointer();
		
	private:
	
		float elements[ NUM_ROWS * NUM_COLS ];
};