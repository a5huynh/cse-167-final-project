/*
 *  Matrix4.cpp
 *  Lab 1
 *
 *  Created by Andrew Huynh on 10/2/09.
 */

#include <math.h>
#include <stdio.h>
#include "Matrix4.h"

Matrix4::Matrix4() {
	for( int i = 0; i < NUM_ROWS * NUM_COLS; i++ ) {
		elements[ i ] = 0.0f;
	}
}

Matrix4::Matrix4( const Matrix4 &m ) {
	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
			this->set( row, col, m.get( row, col ) );
		}
	}
}

Matrix4& Matrix4::operator= ( const Matrix4 &m ) {


	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
			this->set( row, col, m.get( row, col ) );
		}
	}

	return (*this);
}

void Matrix4::identity() {
	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
			if( row == col ) {
				this->set( row, col, 1 );
			} else {
				this->set( row, col, 0 );
			}
		}
	}
}

void Matrix4::multiply( Matrix4 &m ) {
	
	Matrix4 copy  = Matrix4( *this );
	Matrix4 mCopy = Matrix4( m );

	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
		
			// Find C
			float c = 0.0f;
			for( int i = 0; i < NUM_ROWS; i++ ) {
				c += copy.get( row, i ) * mCopy.get( i, col );
			}
		
			this->set( row, col, c );
		}
	}	
}

void Matrix4::multiply( Matrix4 &m1, Matrix4 &m2 ) {

	// Incase m1 or m2 refer to the current matrix
	Matrix4 m1Copy = Matrix4( m1 );
	Matrix4 m2Copy = Matrix4( m2 );

	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
		
			// Find C
			float c = 0.0f;
			for( int i = 0; i < NUM_ROWS; i++ ) {
				c += m1Copy.get( row, i ) * m2Copy.get( i, col );
			}
		
			this->set( row, col, c );
		}
	}	
}

Vector4& Matrix4::multiply( Vector4 &v ) {

	Vector4 *result = new Vector4();

	for( int row = 0; row < NUM_ROWS; row++ ) {
		
		// Find C
		float c = 0.0f;
		for( int i = 0; i < NUM_ROWS; i++ ) {
			c += this->get( row, i ) * v.get( i );
		}
	
		result->set( row, c );	
	}
	
	return *result;
}

void Matrix4::multiply( Vector4& v, Vector4& result ) {
	for( int row = 0; row < NUM_ROWS; row++ ) {
		
		// Find C
		float c = 0.0f;
		for( int i = 0; i < NUM_ROWS; i++ ) {
			c += this->get( row, i ) * v.get( i );
		}
	
		result.set( row, c );	
	}
}

Vector3& Matrix4::multiply( Vector3 &v ) {

	Vector4 *tmp = new Vector4( v.getX(), v.getY(), v.getZ(), 1 );

	Vector4 tmp2;
	multiply( *tmp, tmp2 );

	delete tmp;
	
	tmp2.dehomogenize();
	
	Vector3 *result = new Vector3( tmp2.getX1(), tmp2.getX2(), tmp2.getX3() );
	
	return *result;
}

Matrix4& Matrix4::makeRotate( int axis, float angle ) {
	Matrix4 *rot = new Matrix4();
	
	float c = cos( angle );
	float s = sin( angle );	
	
	if( axis == AXIS_X ) {	
	
		rot->set( 0, 0, 1.0f );
		rot->set( 3, 3, 1.0f );
			
		rot->set( 1, 1, c );
		rot->set( 2, 1, s );
		
		rot->set( 1, 2, -s );
		rot->set( 2, 2, c );
		
	} else if ( axis == AXIS_Y ) {
	
		rot->set( 1, 1, 1.0f );
		rot->set( 3, 3, 1.0f );
			
		rot->set( 0, 0, c );
		rot->set( 0, 2, s );
			
		rot->set( 2, 0, -s );
		rot->set( 2, 2, c );	
		
	} else if ( axis == AXIS_Z ) {
	
		rot->set( 2, 2, 1.0f );
		rot->set( 3, 3, 1.0f );
		
		rot->set( 0, 0, c );
		rot->set( 0, 1, -s );
			
		rot->set( 1, 0, s );
		rot->set( 1, 1, c );	
		
	}
	
	return *rot;
}

Matrix4& Matrix4::makeRotateArbitrary( float x, float y, float z, float angle ) {
	Matrix4 *rot = new Matrix4();
		
	float c = cos( angle );
	float s = sin( angle );	
	
	// Row 1
	rot->set( 0, 0, 1 + ( 1 - c ) * ( x*x - 1 ) );
	rot->set( 0, 1, -z * s + ( 1 - c ) * x * y );
	rot->set( 0, 2, y * s + ( 1 - c ) * x * z );
	
	// Row 2
	rot->set( 1, 0, z * s + ( 1 - c ) * y * z );
	rot->set( 1, 1, 1 + ( 1 - c ) * ( y*y - 1 ) );
	rot->set( 1, 2, -x * s + ( 1- c ) * y * z );
	
	// Row 3
	rot->set( 2, 0, -y * s + ( 1 - c ) * z * x );
	rot->set( 2, 1, x * s + ( 1 - c ) * z * y );
	rot->set( 2, 2, 1 + ( 1 - c ) * ( z*z - 1 ) );
	
	// Row 4
	rot->set( 3, 3, 1.0f );
	
	return *rot;
}

Matrix4& Matrix4::makeScale( float x, float y, float z ) {
	Matrix4 *scale = new Matrix4();
	
	scale->set( 0, 0, x );
	scale->set( 1, 1, y );
	scale->set( 2, 2, z );
	scale->set( 3, 3, 1 );
	
	return *scale;
}

Matrix4& Matrix4::makeTranslation( float x, float y, float z ) {
	Matrix4 *translation = new Matrix4();
	
	translation->set( 0, 0, 1 );
	translation->set( 1, 1, 1 );
	translation->set( 2, 2, 1 );
	translation->set( 3, 3, 1 );
	
	translation->set( 0, 3, x );
	translation->set( 1, 3, y );
	translation->set( 2, 3, z );
	
	return *translation;

}

void Matrix4::rotateX( float angle ) {
	Matrix4 rot = makeRotate( AXIS_X, angle );
	this->multiply( rot );
}

void Matrix4::rotateY( float angle ) {	
	Matrix4 rot = makeRotate( AXIS_Y, angle );
	this->multiply( rot );
}

void Matrix4::rotateZ( float angle ) {
	Matrix4 rot = makeRotate( AXIS_Z, angle );
	this->multiply( rot );
}

void Matrix4::rotate( float x, float y, float z, float angle ) {	
	Matrix4 rot = makeRotateArbitrary( x, y, z, angle);
	this->multiply( rot );
}

void Matrix4::scale( float x, float y, float z ) {	
	Matrix4 scale = makeScale( x, y, z );
	this->multiply( scale );
}

void Matrix4::set( int row, int col, float val ) {
	elements[ row + NUM_COLS * col ] = val;
}

float Matrix4::get( int row, int col ) const {
	return elements[ row + NUM_COLS * col ];
}

float* Matrix4::getPointer() {
	return &elements[0];
}

void Matrix4::print() {

	for( int row = 0; row < NUM_ROWS; row++ ) {
		for( int col = 0; col < NUM_COLS; col++ ) {
		
			printf( "%d %d: %f\n", row, col, get( row, col ) );
		}
	}
}

