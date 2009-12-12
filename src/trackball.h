/*
 *  trackball.h
 *  Lab 2.3
 *
 *  Created by Andrew Huynh on 10/15/09.
 */

#include <GLUT/GLUT.h>
#include <math.h>

#include "Matrix4.h"
#include "Vector3.h"

void initTrackBall();

void handleMouse( int, int, int, int );
void handleRotate( int, int );

Matrix4& getTrackBallMatrix();